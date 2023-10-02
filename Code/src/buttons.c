#include "buttons.h"

#define BUTTON_PIN_1    0

void Buttons_Init()
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER &= ~GPIO_MODER_MODER0; // input
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0; //  pull-up
  SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0);
  SYSCFG->EXTICR[0] |=  (SYSCFG_EXTICR1_EXTI0_PA);
  EXTI->IMR  |=  (1 << BUTTON_PIN_1);
  EXTI->FTSR |=  (1 << BUTTON_PIN_1);
  NVIC_SetPriority(EXTI0_IRQn, 0);
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {
  if (EXTI->PR & (1 << BUTTON_PIN_1)) {
    EXTI->PR |= (1 << BUTTON_PIN_1);
    EXTI->IMR  &=  ~(1 << BUTTON_PIN_1);
    TIM2->ARR = 20;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
}