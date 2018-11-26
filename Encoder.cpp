/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#include <mbed.h>
#include "Encoder.h"
#include "debug_log.h"

#define TIM_X_CNT(TIM) static volatile int32_t TIM##_cnt
#define TIM_X_IRQ_HANDLER(TIM)                                  \
    static void TIM##_irqHandler(void)                          \
    {                                                           \
        if ((TIM->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE)) \
        {                                                       \
            TIM->SR = ~(TIM_IT_UPDATE);                         \
            if (!(TIM->CR1 & (TIM_CR1_DIR)))                    \
                TIM##_cnt += 0xffff;                            \
            else                                                \
                TIM##_cnt -= 0xffff;                            \
        }                                                       \
    }

#define NVIC_INIT(TIM)                                                 \
    do                                                                 \
    {                                                                  \
        NVIC_SetVector((TIM##_IRQn), (uint32_t) & (TIM##_irqHandler)); \
        HAL_NVIC_SetPriority(TIM##_IRQn, 7, 0);                        \
        HAL_NVIC_EnableIRQ(TIM##_IRQn);                                \
        TIM##_cnt = 0;                                                 \
    } while (0)

#if defined(CORE2)
//TODO 
#elif defined(TARGET_NUCLEO_F401RE)
TIM_X_CNT(TIM2);
TIM_X_CNT(TIM3);
TIM_X_CNT(TIM4);
TIM_X_CNT(TIM5);
TIM_X_IRQ_HANDLER(TIM2);        
TIM_X_IRQ_HANDLER(TIM3);        
TIM_X_IRQ_HANDLER(TIM4);        
TIM_X_IRQ_HANDLER(TIM5);        
#else
    #error "Target is not supported!"
#endif

static inline void NVIC_irq_init(TIM_TypeDef * TIMx)
{
    switch((uint32_t)TIMx)
    {
#if defined(TARGET_NUCLEO_F401RE)
        case TIM2_BASE:
        NVIC_INIT(TIM2);
        break;

        case TIM3_BASE:
        NVIC_INIT(TIM3);
        break;

        case TIM4_BASE:
        NVIC_INIT(TIM4);
        break;

        case TIM5_BASE:
        NVIC_INIT(TIM5);
        break;

        default:
        break;
    }
#elif defined(TARGET_CORE2)
//TODO
#endif
}

Encoder::Encoder(TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode)
:_TIM(TIMx)
{
    encoderSetup(&_encoder, &_timer, _TIM, maxcount, encmode);
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
    static bool initialized = false;

    if(initialized)
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
    
    NVIC_irq_init(_TIM);

    core_util_critical_section_exit();
    initialized = true;
}

int32_t Encoder::getCount()
{
    
    switch ((uint32_t)_TIM)
    {
#if defined(TARGET_NUCLEO_F401RE)
    case TIM2_BASE:
        return (int32_t)_TIM->CNT + TIM2_cnt;
    case TIM3_BASE:
        return (int32_t)_TIM->CNT + TIM3_cnt;

    case TIM4_BASE:
        return (int32_t)_TIM->CNT + TIM4_cnt;

    case TIM5_BASE:
        return (int32_t)_TIM->CNT + TIM5_cnt;
#elif defined(TARGET_CORE2)
//TODO
#endif

    default:
        return 0;
    }
}

bool Encoder::getDir()
{
    return __HAL_TIM_IS_TIM_COUNTING_DOWN(&_timer);
}

void Encoder::print_debug_info()
{
    LOG("cnt: %d dir: %s\r\n", getCount(), getDir() ? "-" : "+");
}