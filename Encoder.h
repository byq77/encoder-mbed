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

/**
 * @brief Quadrature encoder interface class.
 *
 * Hardware encoder interface for STM32F4 targets.
 * Support for following boards:
 * * CORE2
 * * PANTHER
 * * NUCLEOF401RE
 */
class Encoder
{
  public:

    /**
     * @brief Encoder constructor.
     * @param TIMx timer used by encoder   
     * @param polarity encoder polarity (counting direction)   
     */
    Encoder(TIM_TypeDef *TIMx, bool polarity = true);

    /**
     * @brief Encoder constructor.
     * @param TIMx timer used by encoder
     * @param timer pointer to timer configuration
     * @param encoder pointer to encoder configuration
     * @param pull GPIO_NOPULL / GPIO_PULLDOWN / GPIO_PULLUP
     */
    Encoder(TIM_TypeDef *TIMx, const TIM_HandleTypeDef * timer, const TIM_Encoder_InitTypeDef * encoder, uint32_t pull);

    /**
     * @brief Get current encoder count.
     * @return num of encoder ticks
     */
    int32_t getCount() const;
    
    /**
     * @brief Initialise Encoder instance.
     *
     * This function starts hardware encoder operation.
     */
    void init();

    /**
     * @brief Print short debugging information to Serial.
     */
    void print_debug_info();
    
    /**
     * @brief Get encoder counting direction.
     * @return true if encoder's counter is incremented, false otherwise
     */
    bool getDir();

    /**
     * @brief Reset current encoder count.
     */
    void resetCount();

    /**
     * @brief Restart encoder after stop.
     */
    void start(uint32_t channel = TIM_CHANNEL_ALL);
    
    /**
     * @brief Stop encoder.
     */
    void stop(uint32_t channel = TIM_CHANNEL_ALL);
    
    /**
     * @brief Toggle encoder polarity.
     */
    void togglePolarity();
    
    virtual ~Encoder(){};

  private:
    TIM_TypeDef *_TIM;
    TIM_HandleTypeDef _timer;
    TIM_Encoder_InitTypeDef _encoder;
    volatile int32_t *_encoder_high_bits;
    bool _initialized;
    bool _polarity;
};

#endif