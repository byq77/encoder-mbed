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
===
```cpp
#include <mbed.h>
#include <Thread.h>
#include <ThisThread.h>
#include <Encoder.h>

using namespace rtos;

DigitalOut led(LED1);
Thread thread;

static void encoder_test()
{
    Encoder encoder2(TIM2);
    encoder2.init();
    while(1){
        encoder2.print_debug_info();
        ThisThread::sleep_for(100);
    }
}

int main()
{
    thread.start(callback(encoder_test));
    while(1)
    {
        led = !led;
        ThisThread::sleep_for(1000);
    }
}
```

`app_mbed.json`
===
```
{
    "config": {
       
    },
    "macros" : [
        
    ],
    "target_overrides" : {
        "NUCLEO_F401RE": {
            "events.shared-dispatch-from-application": 1,
            "platform.default-serial-baud-rate": 115200,
            "platform.stdio-baud-rate": 115200,
            "mbed-encoder.debug-logs": 1
        }
    }
}
```