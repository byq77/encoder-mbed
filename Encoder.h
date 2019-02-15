/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <mbed.h>
namespace mbed
{
    class Encoder
    {
    public:
        Encoder(TIM_TypeDef * TIMx);
        Encoder(TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode, uint32_t polarity, uint32_t pull);
        int32_t getCount();
        void init();
        void print_debug_info();
        bool getDir();
        void resetCount();
        void start(uint32_t channel = TIM_CHANNEL_ALL);
        void stop(uint32_t channel = TIM_CHANNEL_ALL);
        void togglePolarity(){
            _polarity*=-1;
        }
        virtual  ~Encoder() {};  

    private:
        TIM_Encoder_InitTypeDef _encoder;
        TIM_HandleTypeDef _timer;
        TIM_TypeDef *_TIM;
        volatile int32_t * _encoder_count;
        bool _initialized;
        int32_t _polarity;
    };
} // namespace mbed
#endif