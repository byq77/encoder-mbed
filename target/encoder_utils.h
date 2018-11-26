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

#if DEBUG_LOGS_ENCODER
    #define LOG(f_ , ...) printf((f_), ##__VA_ARGS__)
#else
    #define LOG(f_ , ...) do {} while (0)
#endif

#endif /*__ENCODER_UTILS_H__ */