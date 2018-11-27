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
extern uint32_t encoder_gpio_pull;

Encoder::Encoder(TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode, uint32_t polarity, uint32_t pull)
{
    encoder_gpio_pull = pull;
    _TIM = TIMx;
    _timer.Instance = _TIM;
    _timer.Init.Period = maxcount;
    _timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    _timer.Init.Prescaler = 0;
    _timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    _encoder.EncoderMode = encmode;
    _encoder.IC1Filter = 0x0F;
    _encoder.IC1Polarity = polarity;
    _encoder.IC1Prescaler = TIM_ICPSC_DIV4;
    _encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    _encoder.IC2Filter = 0x0F;
    _encoder.IC2Polarity = polarity;
    _encoder.IC2Prescaler = TIM_ICPSC_DIV4;
    _encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    _initialized=false;
}

void Encoder::init()
{
    if(_initialized)
        return;
    if (HAL_TIM_Encoder_Init(&_timer, &_encoder) != HAL_OK)
    {
        error("Couldn't Init Encoder\r\n");
    }

    start();

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
    return (int32_t)_TIM->CNT + *_encoder_count;
}

bool Encoder::getDir()
{
    return __HAL_TIM_IS_TIM_COUNTING_DOWN(&_timer);
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

void Encoder::start(uint32_t channel)
{
    if (HAL_TIM_Encoder_Start(&_timer, channel) != HAL_OK)
    {
        error("Couldn't Start Encoder\r\n");
    }
}

void Encoder::stop(uint32_t channel)
{
    if (HAL_TIM_Encoder_Stop(&_timer, channel) != HAL_OK)
    {
        error("Couldn't Stop Encoder\r\n");
    }
}