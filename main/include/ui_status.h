#ifndef __UI_INFO_H_
#define __UI_INFO_H_

#include <cstdint>

enum class Screen : uint8_t { Splash, Main };
enum class WifiStatus : uint8_t { Off, On };
enum class BluetoothStatus : uint8_t { Off, On };
enum class RadarStatus : uint8_t { Off, On };
enum class GearPosition : uint8_t { Park, Neutral, Sport, Reverse };
enum class TurnSignal : uint8_t { None, Left, Right };
enum class WarningStatus : uint8_t { None, Warning };

struct UiStatus {
  Screen screen;

  // Battery
  uint8_t battery_percent; // 0-100

  // Icons
  WifiStatus wifi;
  BluetoothStatus bluetooth;
  RadarStatus radar;

  // Gear
  GearPosition gear;

  // Turn signals
  TurnSignal turn;

  // Warning
  WarningStatus warning;

  // Status text
  char status_text[32];

  UiStatus()
      : screen(Screen::Splash), battery_percent(0), wifi(WifiStatus::Off),
        bluetooth(BluetoothStatus::Off), radar(RadarStatus::Off),
        gear(GearPosition::Park), turn(TurnSignal::None),
        warning(WarningStatus::None) {
    status_text[0] = '\0';
  }
};

#endif