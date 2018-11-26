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
TIM_X_CNT(TIM5);
TIM_X_IRQ_HANDLER(TIM2);        
TIM_X_IRQ_HANDLER(TIM3);        
TIM_X_IRQ_HANDLER(TIM4);        
TIM_X_IRQ_HANDLER(TIM5);

inline volatile int32_t * NVIC_irq_init(TIM_TypeDef * TIMx)
{
    switch((uint32_t)TIMx)
    {
        case TIM2_BASE:
        NVIC_INIT(TIM2);
        return &TIM2_cnt;

        case TIM3_BASE:
        NVIC_INIT(TIM3);
        return &TIM3_cnt;

        case TIM4_BASE:
        NVIC_INIT(TIM4);
        return &TIM4_cnt;

        case TIM5_BASE:
        NVIC_INIT(TIM5);
        return &TIM5_cnt;

        default:
        return NULL;
    }
}

#endif

#endif /* __TARGET_BOARD_H__ */