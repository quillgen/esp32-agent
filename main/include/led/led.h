#ifndef __LED_H_
#define __LED_H_

namespace walle
{
    class Led
    {
    public:
        virtual ~Led() = default;

    public:
        virtual void onStateChanged() = 0;
    };

    class NoLed : public Led
    {
    public:
        virtual void onStateChanged() override {}
    };
}

#endif