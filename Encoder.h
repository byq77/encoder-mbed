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
        Encoder(TIM_TypeDef *TIMx, uint32_t maxcount = 0xffff, uint32_t encmode = TIM_ENCODERMODE_TI12);
        int32_t getCount();
        void init();
        void print_debug_info();
        bool getDir();

        void resetCount();                   //TODO
        void setPolarity(uint32_t polarity); //TODO
        uint32_t getPolarity();              //TODO
        TIM_TypeDef *getTIMx();              //TODO
        void setPullUp();                    //TODO
        void setPullDown();                  //TODO
        void setPullReset();                 //TODO

    protected:
        TIM_Encoder_InitTypeDef _encoder;
        TIM_HandleTypeDef _timer;
        TIM_TypeDef *_TIM;

    protected:
        static void encoderSetup(TIM_Encoder_InitTypeDef *encoder, TIM_HandleTypeDef *timer,
                                 TIM_TypeDef *TIMx, uint32_t maxcount, uint32_t encmode, uint32_t polarity = TIM_INPUTCHANNELPOLARITY_RISING);
    };
} // namespace mbed
#endif