#ifndef __RGB_LED_H_
#define __RGB_LED_H_

#include <driver/gpio.h>
#include <led_strip.h>
#include <esp_timer.h>
#include <atomic>
#include <mutex>

#include "led.h"

namespace walle
{
    class RgbLed : public Led
    {
    public:
        RgbLed(gpio_num_t pin);
        virtual ~RgbLed();

    public:
        void onStateChanged() override;
        void show(uint8_t r, uint8_t g, uint8_t b);
        void blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms);
        void breathe();
        void turnOff();

    private:
        gpio_num_t _pin;
        led_strip_handle_t _led_strip = nullptr;
        esp_timer_handle_t _strip_timer = nullptr;
        std::mutex _mutex;
    };
}

#endif