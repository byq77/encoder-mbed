/*
 * encoder-mbed library (STM32F4)
 * Copyright (C) 2018 Szymon Szantula 
 *
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */
#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__
#if DEBUG_LOGS_ENCODER
    #define LOG(f_ , ...) printf((f_), ##__VA_ARGS__)
#else
    #define LOG(f_ , ...) do {} while (0)
#endif
#endif /* __DEBUG_LOG_H__ */