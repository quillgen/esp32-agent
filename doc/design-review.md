# Design Review & Refactor Proposal

Scope: ESP32-S3 firmware in `main/` (reviewed at commit `aa1478e`).
Status: Phase 0 implemented on `refactor-phase0-hygiene` (includes the
ESP-IDF 6.1 port); Phases 1-4 pending.

## 1. Summary

The firmware works as a bring-up prototype but is organized as a single god
object with ad-hoc concurrency. Before adding the STM32 vehicle link and the
audio/LLM pipeline, refactor toward: layered drivers/services, an event bus,
a pure state machine, RAII resource ownership, and dependency injection.

## 2. Problems found

### 2.1 Correctness bugs

| # | Location | Issue |
|---|---|---|
| 1 | `network.cpp:26` | `~network()` deletes the event group it received from `Application`; `Application::~Application()` deletes it again (double delete). Modules must not free resources they don't own. |
| 2 | `rgb_led.cpp:111` | `esp_timer_start_periodic(strip_timer_, interval_ms * 500)` — period is in µs; `blink(..., 500)` actually blinks at 250 ms. Should be `interval_ms * 1000`. |
| 3 | `application.cpp:142-156` | `else if` between `APP_STATE_CHANGED` and `NETWORK_STATE_CHANGED` drops the network event whenever both bits are set at once. |
| 4 | `application.cpp:221` | A new `sync_time_task` is spawned on every `CONNECTED` event; no guard against duplicate `esp_netif_sntp_init()` on reconnect. |
| 5 | `application.cpp:67` | `speaker_->test()` runs on every boot and plays hardcoded WAVs — debug code in the production path. |
| 6 | `application.cpp:206,231` | `xQueueSend(ui_queue, ..., portMAX_DELAY)` from the state-machine task can block the orchestrator on a slow/busy UI task. UI updates should use a latest-value mailbox (`xQueueOverwrite`, 1 slot). |

### 2.2 Design smells

- **God object** — `Application` allocates every peripheral, owns the state
  machine, event loop, UI queue, and time sync. No injection, no interfaces.
- **Globals and dead code in `speaker.cpp`** — file-scope `wav_header`,
  `header_parsed`, `total_bytes_read`; unused `http_stream_task`,
  `play_wav_task`; declared-but-missing `play_mp3_task`; empty
  `init_speaker()`; I2S re-initialized per playback by free functions.
- **Mixed sync primitives** — `RgbLed` uses `std::mutex`; `Application` uses
  raw FreeRTOS semaphores; `mutex.h` (an RAII wrapper) exists but is unused.
- **Secrets in Kconfig** — real-looking SSID and password defaults committed
  in `Kconfig.projbuild`. Runtime values belong in NVS.
- **Unused heavyweight dependencies** — `esp-sr` and `esp_audio_codec` are
  declared in `idf_component.yml` and built, but nothing uses them. Remove
  until MP3/ASR work actually lands.
- **Inconsistent naming** — lowercase `network`, `speaker` vs PascalCase
  `Application`, `RgbLed`; mixed Chinese/English comments.
- **Repo hygiene** — `pytest_hello_world.py` is an unused template; personal
  absolute paths committed in `.vscode/settings.json`; README references
  ESP-ADF although the project uses `esp_audio_codec`; `write_spiffs_image.sh`
  builds a 2 MiB image for a 1 MiB partition.

## 3. Target architecture

Layer strictly; each task owns one domain:

```
main.cpp (composition root: create drivers -> services -> app, inject)
  |- drivers/     dumb hardware access, no domain logic
  |    led, display (SSD1306 + LVGL glue), audio (I2S), mic,
  |    storage (NVS + SPIFFS), wifi
  |- services/    one concern each, emit events on the bus
  |    NetworkService   (connect state machine, SNTP, retries)
  |    AudioPlayer      (playback queue, WAV/MP3 decoders)
  |    LedController    (blink/breathe patterns)
  |    UiController     (screens; owns the LVGL task)
  |    Vehicle          (STM32 UART link, protocol framing)  [future]
  |- core/        RAII FreeRTOS wrappers, events, StateMachine
  |- app/         Application = thin orchestrator
  |- main.cpp
```

### 3.1 Event bus

Replace the ad-hoc event group with ESP-IDF `esp_event` (network already uses
it). Define one app event base:

```cpp
ESP_EVENT_DECLARE_BASE(APP_EVENTS);
enum {
  APP_STATE_CHANGED,   // arg: AppState
  WIFI_STATE_CHANGED,  // arg: NetworkState
  TIME_SYNCED,         // arg: time_t
  AUDIO_FINISHED,      // arg: result
  VEHICLE_TELEMETRY,   // arg: TelemetryFrame*   [future]
};
```

Services register their own handlers. Cross-task commands (e.g. "play file",
"show screen") go through per-service queues owned by the service.

### 3.2 State machine

Keep the state machine pure (no FreeRTOS, no hardware) so it can be unit
tested on the host:

```cpp
class AppStateMachine {
 public:
  enum class State { Boot, Connecting, Idle, Active, Error };
  Result handle(const AppEvent& e);  // pure, deterministic
  State state() const;
};
```

Explicit transition table instead of commented-out handlers
(`handle_idle_state()` etc. are currently empty).

### 3.3 Ownership rules

- `std::unique_ptr` for all owned objects; RAII wrappers for FreeRTOS handles
  (queue, mutex, event group, task). Delete `mutex.h` or replace it with a
  generic `freertos/` wrapper set.
- A class never destroys a handle it was passed; the owner frees it.
- No raw `new` outside the composition root.
- Use `std::mutex` only for short, non-ISR critical sections; FreeRTOS
  primitives for task synchronization and IPC. Pick one policy per module.

### 3.4 Concurrency model

| Task | Owns | Communication |
|---|---|---|
| `app_task` | state machine, orchestration | app event bus + queues |
| `ui_task` | LVGL objects | latest-wins `UiUpdate` mailbox |
| `audio_task` | I2S TX + decoders | `PlayRequest` queue → `AUDIO_FINISHED` event |
| esp_timer callbacks | LED patterns, LVGL tick | no blocking, no allocation |
| `time_task` (one-shot) | SNTP sync | guarded by task handle |

Boot order should be explicit: init storage → drivers → publish state →
start tasks. Avoid publishing events before consumers exist (current code
sets `BOOTING` before tasks are created — fragile ordering).

### 3.5 Vehicle link (future STM32 work)

Define the protocol before writing firmware on either side:

```cpp
// shared with the STM32 firmware
struct VehicleFrame {
  uint8_t  magic;         // 0xAA
  uint8_t  type;          // command / telemetry / ack / heartbeat
  uint8_t  seq;           // rolling sequence + ACK field
  uint16_t payload_len;
  uint8_t  payload[64];
  uint16_t crc16;
};
class VehicleLink {          // UART transport
  Result send(const DriveCommand&);
  void on_frame(const Frame&);   // -> posts VEHICLE_TELEMETRY on the bus
};
```

The `UiStatus` gear enum already anticipates this — move it into the shared
protocol header eventually.

### 3.6 Configuration

- Pins: Kconfig entries with defaults (today they are `#define`s in
  `device.h`; keep one source of truth).
- Secrets (WiFi): NVS at runtime; keep `menuconfig` override for development,
  but commit empty/placeholder defaults.
- Feature flags: `CONFIG_AGENT_BOOT_SOUND`, `CONFIG_AGENT_AUTOCONNECT`, …

## 4. Proposed directory layout

```
main/
  CMakeLists.txt
  Kconfig.projbuild
  main.cpp                    # composition root only
  include/
    app/application.h
    app/events.h              # esp_event ids + payload types
    app/state_machine.h
    app/app_config.h          # pin mapping (from Kconfig)
    core/freertos_guard.h     # RAII: Queue, Mutex, EventGroup, Task
    drivers/led/rgb_led.h
    drivers/display/oled_display.h
    drivers/audio/i2s_speaker.h
    drivers/audio/mic.h
    drivers/storage/storage.h
    drivers/wifi/network.h
    services/audio_player.h
    services/led_controller.h
    services/ui_controller.h
    services/vehicle/vehicle.h
    services/vehicle/protocol.h
  src/ ...                    # mirrors include/
```

Keep the explicit `SOURCES` list in `CMakeLists.txt` (no globbing).

## 5. Phased migration

- **Phase 0 — hygiene, no behavior change**
  - Fix bugs #1–#6 above.
  - Delete dead code (`http_stream_task`, `play_wav_task`, `play_mp3_task`
    declaration, duplicated NTP defines, unused `mutex.h`).
  - Gate boot sound behind Kconfig; move I2S setup into
    `init_speaker()`.
  - Remove `esp-sr`/`esp_audio_codec` from `idf_component.yml` until used.
  - Unify naming and comment language; RAII the FreeRTOS handles.
  - Move personal `.vscode` paths out of the repo; update README;
    remove `pytest_hello_world.py`.
- **Phase 1 — event bus + state machine**
  - Introduce `app/events.h`, pure `AppStateMachine`.
  - Replace the `xEventGroupWaitBits` polling loop; handle all set bits.
  - Latest-wins UI mailbox; guard the time-sync task.
- **Phase 2 — services + DI**
  - Split `application.cpp` into the services above; narrow driver
    interfaces; `Application` becomes a thin orchestrator.
  - Pins to Kconfig; credentials to NVS.
- **Phase 3 — features**
  - MP3 pipeline via `esp_audio_codec` behind `AudioPlayer`.
  - `protocol.h` + `VehicleService`; LED patterns as `LedController`.
- **Phase 4 — verification**
  - Host unit tests for `AppStateMachine` and `protocol` (pure C++).
  - pytest-embedded smoke test (boot, WiFi connect, UI update).
  - CI: build + clang-format check.

## 6. Conventions going forward

- One class per file, header guards, namespace `agent`.
- FreeRTOS calls stay inside drivers/services; `app/` stays portable.
- No globals except `constexpr` config; no raw `new`/`delete`.
- Plain log tags (drop the emoji tags) or a consistent scheme.
- Any change touching `device.h` pins must update `doc/` diagrams or delete
  them (they already disagree).
