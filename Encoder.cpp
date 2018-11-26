/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#include <mbed.h>
#include "Encoder.h"
#include "./target/target_board.h"

Encoder::Encoder(TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode)
:_TIM(TIMx)
{
    encoderSetup(&_encoder, &_timer, _TIM, maxcount, encmode);
    _initialized=false;
}

void Encoder::encoderSetup(TIM_Encoder_InitTypeDef * encoder, TIM_HandleTypeDef * timer, 
                            TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode, uint32_t polarity)
{
    timer->Instance = TIMx;
    timer->Init.Period = maxcount;
    timer->Init.CounterMode = TIM_COUNTERMODE_UP;
    timer->Init.Prescaler = 0;
    timer->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    encoder->EncoderMode = encmode;

    encoder->IC1Filter = 0x0F;
    encoder->IC1Polarity = polarity;
    encoder->IC1Prescaler = TIM_ICPSC_DIV4;
    encoder->IC1Selection = TIM_ICSELECTION_DIRECTTI;

    encoder->IC2Filter = 0x0F;
    encoder->IC2Polarity = polarity;
    encoder->IC2Prescaler = TIM_ICPSC_DIV4;
    encoder->IC2Selection = TIM_ICSELECTION_DIRECTTI;
}

void Encoder::init()
{
    if(_initialized)
        return;
    if (HAL_TIM_Encoder_Init(&_timer, &_encoder) != HAL_OK)
    {
        error("Couldn't Init Encoder\r\n");
    }

    if (HAL_TIM_Encoder_Start(&_timer, TIM_CHANNEL_ALL) != HAL_OK)
    {
        error("Couldn't Start Encoder\r\n");
    }

    // Update (aka over- and underflow) interrupt enabled
    __HAL_TIM_ENABLE_IT(&_timer, TIM_IT_UPDATE);
    // The initialisation process generates an update interrupt, so we'll have to clear the update flag before anything else
    __HAL_TIM_CLEAR_FLAG(&_timer, TIM_IT_UPDATE);
    
    core_util_critical_section_enter();
    
    _encoder_count = NVIC_irq_init(_TIM);

    core_util_critical_section_exit();
    _initialized = true;
}

int32_t Encoder::getCount()
{
    if(_initialized)
        return (int32_t)_TIM->CNT + *_encoder_count;
    else
        return 0;
}

bool Encoder::getDir()
{
    if(_initialized)
        return __HAL_TIM_IS_TIM_COUNTING_DOWN(&_timer);
    else
        return 0;
}

void Encoder::print_debug_info()
{
    if(_initialized)
        LOG("cnt: %d dir: %s\r\n", getCount(), getDir() ? "-" : "+");
    else
        LOG("Library is not initialized!\r\n");
}

void Encoder::resetCount()
{
    if(!_initialized)
        return;
    core_util_critical_section_enter();
        *_encoder_count = _TIM->CNT = 0;
        __HAL_TIM_CLEAR_FLAG(&_timer, TIM_IT_UPDATE);
    core_util_critical_section_exit();
}