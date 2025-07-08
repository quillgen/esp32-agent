#include "network.h"

#include "event.h"
#include <esp_log.h>
#include <esp_netif_sntp.h>
#include <esp_sntp.h>
#include <esp_wifi.h>

using namespace agent;

#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.aliyun.com"

static const int MAX_RETRY_COUNT = 3;
static const char *TAG = "🤖 NET";

network::network(EventGroupHandle_t e)
    : state_(NetworkState::DISCONNECTED), retry_count_(0), event_group_(e),
      mutex_(xSemaphoreCreateMutex()) {}

network::~network() {
  esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                               &event_handler_static);
  esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                               &event_handler_static);
  vEventGroupDelete(event_group_);
}

void network::event_handler_static(void *arg, esp_event_base_t base,
                                   int32_t event_id, void *event_data) {
  network *self = static_cast<network *>(arg);
  if (self) {
    self->event_handler(base, event_id, event_data);
  } else {
    ESP_LOGE(TAG, "failed to cast *arg -> network instance, network handler "
                  "will not work!");
  }
}

void network::event_handler(esp_event_base_t event_base, int32_t event_id,
                            void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI(TAG, "connecting to wifi...");
    set_state(NetworkState::CONNECTING);
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP) {
    set_state(NetworkState::DISCONNECTED);
    ESP_LOGE(TAG, "wifi disconnected");
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (this->retry_count_ < MAX_RETRY_COUNT) {
      ESP_LOGW(TAG, "retry (%d/%d) connecting..", (this->retry_count_ + 1),
               MAX_RETRY_COUNT);
      esp_wifi_connect();
      this->retry_count_++;
    } else {
      set_state(NetworkState::NETWORK_ERROR);
    }
    ESP_LOGE(TAG, "wifi connect failed");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    this->retry_count_ = 0;
    set_state(NetworkState::CONNECTED);
  }
}

void network::init() {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_static, this,
      &instance_any_id));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler_static, this,
      &instance_got_ip));

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = CONFIG_ESP_WIFI_SSID,
              .password = CONFIG_ESP_WIFI_PASSWORD,
              .threshold = {.authmode = WIFI_AUTH_WPA_WPA2_PSK},
              // H2E is more safe, to use old WPA2 device, use WPA3_SAE_PWE_BOTH
              .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
          },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "wifi initialized");
}

void network::set_state(NetworkState newState) {
  if (xSemaphoreTake(mutex_, pdMS_TO_TICKS(20)) == pdTRUE) {
    if (state_ != newState) {
      state_ = newState;
      ESP_LOGI(TAG, "wifi state changed ->%d", state_);
      xEventGroupSetBits(event_group_, Events::NETWORK_STATE_CHANGED);
    }
    xSemaphoreGive(mutex_);
  }
}