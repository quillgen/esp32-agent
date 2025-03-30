#include "led/rgb_led.h"
#include "application.h"

using namespace walle;

#define TAG "RgbLed"

RgbLed::RgbLed(gpio_num_t pin)
    : _pin(pin)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = this->_pin,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags = {
            // If the RMT hardware can't be assist by DMA, the driver will going into interrupt very frequently, thus result in a high CPU usage.
            .with_dma = true, // DMA feature is available on chips like ESP32-S3/P4
        },
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &this->_led_strip));
    led_strip_clear(this->_led_strip);
}

RgbLed::~RgbLed()
{
    if (this->_led_strip != nullptr)
    {
        led_strip_del(this->_led_strip);
    }
}

void RgbLed::onStateChanged()
{
    auto &app = Application::GetInstance();
    auto state = app.getState();
    switch (state)
    {
    case Unknown:
        show(20, 0, 0);
        break;

    default:
        break;
    }
}

void RgbLed::show(uint8_t r, uint8_t g, uint8_t b)
{
    std::lock_guard<std::mutex> lock(this->_mutex);
    led_strip_set_pixel(this->_led_strip, 0, r, g, b);
    led_strip_refresh(this->_led_strip);
}

void RgbLed::blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms)
{
}

void RgbLed::breathe()
{
}

void RgbLed::turnOff()
{
}