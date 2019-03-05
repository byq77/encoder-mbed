/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#include <mbed.h>
#include "Encoder.h"
#include "target/target_board.h"

#define MAX_TIMER_VALUE_HALF 32768
#define ENCODER_INIT(a, b, c, d, e)                         \
    {                                                       \
        _timer.Instance = (a);                              \
        _timer.Init.Period = (b);                           \
        _timer.Init.CounterMode = TIM_COUNTERMODE_UP;       \
        _timer.Init.Prescaler = 0;                          \
        _timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; \
        _encoder.EncoderMode = (c);                         \
        _encoder.IC1Filter = 0x0F;                          \
        _encoder.IC1Polarity = (d);                         \
        _encoder.IC1Prescaler = TIM_ICPSC_DIV4;             \
        _encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;   \
        _encoder.IC2Filter = 0x0F;                          \
        _encoder.IC2Polarity = (e);                         \
        _encoder.IC2Prescaler = TIM_ICPSC_DIV4;             \
        _encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;   \
        _polarity = 1;                                      \
        _initialized = false;                               \
    }

extern uint32_t encoder_gpio_pull;

Encoder::Encoder(TIM_TypeDef * TIMx, bool polarity)
: _TIM(TIMx)
{
    _polarity = polarity;
    int d = polarity ? TIM_INPUTCHANNELPOLARITY_RISING : TIM_INPUTCHANNELPOLARITY_FALLING;
    ENCODER_INIT(TIMx, 0xffff, TIM_ENCODERMODE_TI12, d, TIM_INPUTCHANNELPOLARITY_RISING);  
#if defined(TARGET_PANTHER)
    encoder_gpio_pull = GPIO_NOPULL;
#else
    encoder_gpio_pull = GPIO_PULLDOWN;
#endif
}

Encoder::Encoder(TIM_TypeDef *TIMx, const TIM_HandleTypeDef *timer, const TIM_Encoder_InitTypeDef *encoder, uint32_t pull)
    : _TIM(TIMx), _timer(*timer), _encoder(*encoder)
{
    _polarity = (_encoder.IC1Polarity == TIM_INPUTCHANNELPOLARITY_RISING);
    encoder_gpio_pull = pull;
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
    
    _encoder_high_bits = NVIC_irq_init(_TIM);

    core_util_critical_section_exit();
    _initialized = true;
}

int32_t Encoder::getCount() const
{
    int32_t count, hbits;
    core_util_critical_section_enter();
    count = _TIM->CNT;
    if ((_TIM->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE))
    {
        _TIM->SR = ~(TIM_IT_UPDATE);
        if (_TIM->CNT < MAX_TIMER_VALUE_HALF)
            *_encoder_high_bits += 1;
        else
            *_encoder_high_bits -= 1;
        count = _TIM->CNT;
    }
    hbits = *_encoder_high_bits;
    core_util_critical_section_exit();
    return  (hbits << 16) | count;
}

bool Encoder::getDir()
{
    return __HAL_TIM_IS_TIM_COUNTING_DOWN(&_timer);
}

void Encoder::print_debug_info()
{
#if DEBUG_LOGS_ENCODER
    if(_initialized)
        printf("cnt: %d dir: %s\r\n", getCount(), getDir() ? "-" : "+");
    else
        printf("Library is not initialized!\r\n");
#endif
}

void Encoder::resetCount()
{
    if(!_initialized)
        return;
    core_util_critical_section_enter();
        *_encoder_high_bits = _TIM->CNT = 0;
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

void Encoder::togglePolarity()
{
    
    _polarity = !_polarity;
    _encoder.IC1Polarity = (_polarity ? TIM_INPUTCHANNELPOLARITY_RISING : TIM_INPUTCHANNELPOLARITY_FALLING);
    if(_initialized)
    {
        //TODO implement!
    }
}