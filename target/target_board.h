/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#ifndef __TARGET_BOARD_H__
#define __TARGET_BOARD_H__
#include <mbed.h>
#include "encoder_utils.h"

#if defined(TARGET_CORE2)
// TODO 
#elif defined(TARGET_NUCLEO_F401RE)
TIM_X_CNT(TIM2);
TIM_X_CNT(TIM3);
TIM_X_CNT(TIM4);
TIM_X_CNT(TIM1);
TIM_X_IRQ_HANDLER(TIM2);        
TIM_X_IRQ_HANDLER(TIM3);        
TIM_X_IRQ_HANDLER(TIM4);        
TIM_X_IRQ_HANDLER(TIM1);

inline volatile int32_t * NVIC_irq_init(TIM_TypeDef * TIMx)
{
    switch((uint32_t)TIMx)
    {
        case TIM2_BASE:
        NVIC_SETUP(TIM2,TIM2_IRQn);
        return &TIM2_cnt;

        case TIM3_BASE:
        NVIC_SETUP(TIM3,TIM3_IRQn);
        return &TIM3_cnt;

        case TIM4_BASE:
        NVIC_SETUP(TIM4,TIM4_IRQn);
        return &TIM4_cnt;

        case TIM1_BASE:
        NVIC_SETUP(TIM1,TIM1_UP_TIM10_IRQn);
        return &TIM1_cnt;

        default:
        return NULL;
    }
}

#endif

#endif /* __TARGET_BOARD_H__ */