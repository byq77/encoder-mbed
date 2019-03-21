# encoder-mbed
Hardware encoder library for stm32f4.

## Import
In your mbed application root folder run:

```bash
$ mbed add https://github.com/byq77/encoder-mbed.git
```

## Supported targets

* **TARGET_CORE2**
    * ENCODER_1: `TIM2` (`PA_0` and `PA_1`) 
    * ENCODER_2: `TIM8` (`PC_6` and `PC_7`)
    * ENCODER_3: `TIM3` (`PB_4` and `PA_7`)
    * ENCODER_4: `TIM4` (`PB_6` and `PB_7`)
* **TARGET_PANTHER**
    * ENCODER_1: `TIM4` (`PB_6` and `PB_7`)
    * ENCODER_2: `TIM1` (`PE_9` and `PE_11`)
    * ENCODER_3: `TIM3` (`PC_6` and `PC_7`)
    * ENCODER_4: `TIM2` (`PA_15` and `PB_3`)
* **TARGET_NUCLEO_F401RE**
    * `TIM1` (`PA_8` and `PA_9`)
    * `TIM2` (`PA_0` and `PA_1`)
    * `TIM3` (`PB_4` and `PB_5`)
    * `TIM4` (`PB_6` and `PB_7`)
* **TARGET_RCP**
    * ENCODER1:`TIM2` (`PA_0` and `PA_1`)
    * ENCODER2:`TIM4` (`PD_12` and `PD_13`)

You can easily add support for other STM32 targets or change assigned pins by editing following files `target/target_board.h` and `EncoderMspInitF4.cpp`.

## Example code

`main.cpp`
```cpp
#include <mbed.h>
#include <Thread.h>
#include <mbed_events.h>
#include <Encoder.h>

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