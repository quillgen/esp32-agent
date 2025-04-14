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

void network::event_handler(esp_event_base_t event_base, int32_t event_id,
                            void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    ESP_LOGI(TAG, "connecting to wifi...");
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (this->retry_count < MAX_RETRY_COUNT) {
      ESP_LOGW(TAG, "retry (%d/%d) connecting..", (this->retry_count + 1),
               MAX_RETRY_COUNT);
      esp_wifi_connect();
      this->retry_count++;
    } else {
      xEventGroupSetBits(this->event_group, BIT_WIFI_FAILED);
    }
    ESP_LOGE(TAG, "wifi disconnected");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    this->retry_count = 0;
    xEventGroupSetBits(this->event_group, BIT_WIFI_CONNECTED);
  }
}

void network::init_wifi() {
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

  EventBits_t bits = xEventGroupWaitBits(this->event_group,
                                         BIT_WIFI_CONNECTED | BIT_WIFI_FAILED,
                                         pdFALSE, pdFALSE, portMAX_DELAY);
  if (bits & BIT_WIFI_CONNECTED) {
    ESP_LOGI(TAG, "connected to ap");
    this->sync_time();

    int retry = 0;
    const int max_retry = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) ==
               ESP_ERR_TIMEOUT &&
           ++retry < retry_count) {
      ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry,
               retry_count);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    ESP_LOGI(TAG, "retry =%d", retry);

    time_t now;
    struct tm timeinfo;
    time(&now);
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    // esp_netif_sntp_deinit();
  } else if (bits & BIT_WIFI_FAILED) {
    ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
             CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
  } else {
    ESP_LOGE(TAG, "UNEXPECTED EVENT");
  }
}

void time_sync_notification_cb(struct timeval *tv) {
  ESP_LOGI(TAG, "sntp time sync done!");
}

void network::sync_time() {
  ESP_LOGI(TAG, "initialize SNTP...");
  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER_1);
  config.sync_cb = time_sync_notification_cb;
  esp_netif_sntp_init(&config);
}