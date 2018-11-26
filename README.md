# encoder-mbed
Hardware encoder library for stm32f4.

## Import
In your mbed application root folder run:

```bash
$ cd lib # make sure you have lib dir, otherwise run 'mkdir lib'
$ mbed add https://github.com/byq77/encoder-mbed.git
```

## Supported targets

* TARGET_CORE2
* TARGET_NUCLEO_F401RE

You can easily add support for other STM32 targets.

## Example code

`main.cpp`
```cpp
#include <mbed.h>
#include <Thread.h>
#include <ThisThread.h>
#include <mbed_events.h>
#include <Encoder.h>

using namespace rtos;

DigitalOut led(LED1);
InterruptIn button(BUTTON1,PullDown);
Thread thread;

Encoder encoder(TIM2);
Mutex encoder_mutex;

static void encoder_test()
{
    encoder.init();
    while(1){
        encoder_mutex.lock();

        encoder.print_debug_info();

        encoder_mutex.unlock();
        ThisThread::sleep_for(100);
    }
}

static void buttonCallback()
{
    ThisThread::sleep_for(50);
    if(!button.read())
        return;
    encoder_mutex.lock();
        encoder.resetCount();
        printf("RESET!\r\n");
    encoder_mutex.unlock();
}


int main()
{
    EventQueue * q = mbed_event_queue();

    thread.start(encoder_test);
    button.rise(q->event(buttonCallback));
    while(1)
    {
        led = !led;
        ThisThread::sleep_for(1000);
    }
}
```

`app_mbed.json`
```
{
    "config": {
       
    },
    "macros" : [
        
    ],
    "target_overrides" : {
        "NUCLEO_F401RE": {
            "events.shared-dispatch-from-application": 0,
            "platform.default-serial-baud-rate": 115200,
            "platform.stdio-baud-rate": 115200,
            "mbed-encoder.debug-logs": 1
        }
    }
}
```