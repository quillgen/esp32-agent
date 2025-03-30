#include "application.h"
#include <esp_log.h>

#include "led/rgb_led.h"

using namespace walle;

#define TAG "Application"

Application::Application()
{
    this->_led = new RgbLed((gpio_num_t)CONFIG_BLINK_GPIO);
    setAppState(Unknown);
}

Application::~Application()
{
    delete this->_led;
}

void Application::start()
{
}

void Application::mainLoop()
{
}

void Application::setAppState(AppState state)
{
    if (this->_state == state)
    {
        return;
    }
    ESP_LOGI(TAG, "STATE: %s", state);
    this->_led->onStateChanged();
}