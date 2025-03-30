#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "led/led.h"

namespace walle
{

    enum AppState
    {
        Unknown,
        Starting,
        ConfiguringWifi,
        Idle,
        Connecting,
        Listening,
        Speaking,
        Upgrading,
        Activating,
        Error,
    };

    class Application
    {
    public:
        static Application &GetInstance()
        {
            static Application instance;
            return instance;
        }

    public:
        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

    public:
        inline AppState getState()
        {
            return this->_state;
        }

        void start();

    private:
        Application();
        ~Application();

    private:
        void mainLoop();
        void setAppState(AppState state);

    private:
        volatile AppState _state = Unknown;
        Led *_led = nullptr;
    };
}

#endif