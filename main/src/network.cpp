#include "network.h"

#include <esp_log.h>
#include <esp_wifi.h>
#include <string.h>
#include <strings.h>

using namespace walle;

static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;

static const int MAX_RETRY_COUNT = 3;
static const char *TAG = "🤖 NET";

network::network(EventGroupHandle_t e) : event_group(e) {}

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

void network::handle_wifi_event(int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_STA_START) {
    xTaskCreate(smart_config_task_static, "smartconfig_task", 4096, this, 3,
                NULL);
  } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
    esp_wifi_connect();
    xEventGroupClearBits(this->event_group, CONNECTED_BIT);

    // if (this->retry_count < MAX_RETRY_COUNT) {
    //   ESP_LOGW(TAG, "retry (%d/%d) connecting..", (this->retry_count + 1),
    //            MAX_RETRY_COUNT);
    //   esp_wifi_connect();
    //   this->retry_count++;
    // } else {
    //   xEventGroupSetBits(this->event_group, WIFI_FAIL_BIT);
    // }
    // ESP_LOGE(TAG, "wifi disconnected");
  }
}

void network::handle_ip_event(int32_t event_id, void *event_data) {
  if (event_id == IP_EVENT_STA_GOT_IP) {
    xEventGroupSetBits(this->event_group, CONNECTED_BIT);
  }
}

void network::handle_sc_event(int32_t event_id, void *event_data) {
  switch (event_id) {
  case SC_EVENT_SCAN_DONE:
    ESP_LOGI(TAG, "scan done");
    break;
  case SC_EVENT_FOUND_CHANNEL:
    ESP_LOGI(TAG, "found channel");
    break;
  case SC_EVENT_GOT_SSID_PSWD:
    ESP_LOGI(TAG, "got SSID and password");
    connect_form_sc((smartconfig_event_got_ssid_pswd_t *)event_data);
    break;
  case SC_EVENT_SEND_ACK_DONE:
    xEventGroupSetBits(this->event_group, ESPTOUCH_DONE_BIT);
    break;
  default:
    break;
  }
}

void network::connect_form_sc(smartconfig_event_got_ssid_pswd_t *e) {
  wifi_config_t wifi_config;
  uint8_t ssid[33] = {0};
  uint8_t password[65] = {0};
  uint8_t rvd_data[33] = {0};

  bzero(&wifi_config, sizeof(wifi_config_t));
  memcpy(wifi_config.sta.ssid, e->ssid, sizeof(wifi_config.sta.ssid));
  memcpy(wifi_config.sta.password, e->password,
         sizeof(wifi_config.sta.password));
  memcpy(ssid, e->ssid, sizeof(e->ssid));
  memcpy(password, e->password, sizeof(e->password));

  ESP_LOGI(TAG, "got wifi info: SSID:%s, PASSWORD:%s", ssid, password);

  if (e->type == SC_TYPE_ESPTOUCH_V2) {
    ESP_ERROR_CHECK(esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)));
    ESP_LOGI(TAG, "got RVD_DATA:");
    for (int i = 0; i < 33; i++) {
      printf("%02x ", rvd_data[i]);
    }
    printf("\n");
  }

  ESP_ERROR_CHECK(esp_wifi_disconnect());
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  esp_wifi_connect();
}

void network::event_handler(esp_event_base_t event_base, int32_t event_id,
                            void *event_data) {
  if (event_base == WIFI_EVENT) {
    handle_wifi_event(event_id, event_data);
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    handle_ip_event(event_id, event_data);
  } else if (event_base == SC_EVENT) {
    handle_sc_event(event_id, event_data);
  }
}

void network::smart_config_task_static(void *pvParameters) {
  network *self = static_cast<network *>(pvParameters);
  if (self) {
    self->smart_config_task();
  } else {
    ESP_LOGE(TAG, "failed to cast *arg -> network instance, network handler "
                  "will not work!");
  }
}

void network::smart_config_task() {
  EventBits_t uxBits;
  ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
  smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
  cfg.enable_log = true;

  ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
  while (1) {
    ESP_LOGI(TAG, "waiting for wifi connection events...");
    uxBits = xEventGroupWaitBits(this->event_group,
                                 CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false,
                                 portMAX_DELAY);
    if (uxBits & CONNECTED_BIT) {
      ESP_LOGI(TAG, "wifi connected!");
    }
    if (uxBits & ESPTOUCH_DONE_BIT) {
      ESP_LOGI(TAG, "smartconfig finished");
      esp_smartconfig_stop();
      vTaskDelete(NULL);
    }
  }
}

void network::init_wifi() {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  esp_event_handler_instance_t inst1, inst2, inst3;

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler_static, this, &inst1));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler_static, this, &inst2));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      SC_EVENT, ESP_EVENT_ANY_ID, &event_handler_static, this, &inst3));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
}

bool network::block_for_connection() {
  // EventBits_t bits =
  //     xEventGroupWaitBits(this->event_group, WIFI_CONNECTED_BIT |
  //     WIFI_FAIL_BIT,
  //                         pdFALSE, pdFALSE, portMAX_DELAY);
  // if (bits & WIFI_CONNECTED_BIT) {
  //   ESP_LOGI(TAG, "connected to ap");
  //   return true;
  // } else if (bits & WIFI_FAIL_BIT) {
  //   ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
  //            CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
  //   return false;
  // } else {
  //   ESP_LOGE(TAG, "UNEXPECTED EVENT");
  //   return false;
  // }

  return false;
}