#include "timer.h"

void TIM5_Init()
{
  // Enable TIM1 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    // Configure TIM1 settings
    TIM5->PSC = 42000; // Prescaler to have a 1 kHz timer frequency
    TIM5->ARR = 999;  // Auto-reload value, so the timer will overflow every 1 second
    TIM5->CR1 |= TIM_CR1_URS; // Only generate update event (reload) for counter overflow/underflow
    TIM5->DIER |= TIM_DIER_UIE; // Enable update interrupt

    // Enable TIM1 interrupt in NVIC
    NVIC_SetPriority(TIM5_IRQn, 0); // Set priority (0 by default)
    NVIC_EnableIRQ(TIM5_IRQn);
    
    // Enable TIM1 counter
    TIM5->CR1 |= TIM_CR1_CEN;
}

void TIM2_Init()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure TIM1 settings
    TIM2->PSC = 42000; // Prescaler to have a 1 kHz timer frequency
    TIM2->ARR = 20;  // Auto-reload value, so the timer will overflow every 1 second
    TIM2->CR1 |= TIM_CR1_URS; // Only generate update event (reload) for counter overflow/underflow
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt

    // Enable TIM1 interrupt in NVIC
    NVIC_SetPriority(TIM2_IRQn, 0); // Set priority (0 by default)
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM5_IRQHandler(void) {
    // Clear the interrupt flag
    TIM5->SR &= ~TIM_SR_UIF;
    number_fade ^= 1;
    GPIOC->ODR ^= GPIO_ODR_ODR_13;
    interface_update = 1;
    // Your code here (will be executed when the timer overflows)
}

void TIM2_IRQHandler(void) {
    // Clear the interrupt flag
    static uint8_t button_counter = 0;
    static uint8_t button_counter_n = 0;
    TIM2->SR &= ~TIM_SR_UIF;
    
    if ((GPIOA->IDR & GPIO_IDR_IDR_0) == 0)
    {
      TIM2->ARR = 5;
      button_counter++;
      if(button_counter == 5)
      {
        button_counter = 0;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        EXTI->IMR  |=  (1 << 0);
        if(selected_symb < 2)
        {
          selected_symb++;
        }
        else
        {
          selected_symb = 0;
        }
        number_fade = 0;
        interface_update = 1;
      }
    }
    else
    {
      button_counter_n++;
      
      if(button_counter_n == 5)
      {
        button_counter_n = 0;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        EXTI->IMR  |=  (1 << 0);
        button_counter = 0;
      }
    }
    // Your code here (will be executed when the timer overflows)
}