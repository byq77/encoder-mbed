/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#ifndef __ENCODER_UTILS_H__
#define __ENCODER_UTILS_H__

#define TIM_X_CNT(TIM) static volatile int32_t TIM##_cnt

#define TIM_X_IRQ_HANDLER(TIM)                                  \
    static void TIM##_irqHandler(void)                          \
    {                                                           \
        if ((TIM->SR & (TIM_FLAG_UPDATE)) == (TIM_FLAG_UPDATE)) \
        {                                                       \
            TIM->SR = ~(TIM_IT_UPDATE);                         \
            if (!(TIM->CR1 & (TIM_CR1_DIR)))                    \
                TIM##_cnt += 1;                                 \
            else                                                \
                TIM##_cnt -= 1;                                 \
        }                                                       \
    }

#define NVIC_SETUP(TIM,INTERRUPT)                                     \
    do                                                                \
    {                                                                 \
        NVIC_SetVector((INTERRUPT), (uint32_t) & (TIM##_irqHandler)); \
        HAL_NVIC_SetPriority(INTERRUPT, 6, 0);                        \
        HAL_NVIC_EnableIRQ(INTERRUPT);                                \
        TIM##_cnt = 0;                                                \
    } while (0)

#endif /*__ENCODER_UTILS_H__ */