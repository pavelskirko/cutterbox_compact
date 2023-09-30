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

void TIM5_IRQHandler(void) {
    // Clear the interrupt flag
    TIM5->SR &= ~TIM_SR_UIF;
    if(count)
    {
      count--;
      count_update = 1;
    }
    GPIOC->ODR ^= GPIO_ODR_ODR_13;
    // Your code here (will be executed when the timer overflows)
}