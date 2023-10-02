#include "buttons.h"

#define BUTTON_PIN_0    0
#define BUTTON_PIN_1    1
#define BUTTON_PIN_2    2
#define BUTTON_PIN_3    3

void Buttons_Init()
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3); // input
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0; //  pull-up
  SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0 | SYSCFG_EXTICR1_EXTI1 | SYSCFG_EXTICR1_EXTI2 | SYSCFG_EXTICR1_EXTI3);
  SYSCFG->EXTICR[0] |=  (SYSCFG_EXTICR1_EXTI0_PA | SYSCFG_EXTICR1_EXTI1_PA | SYSCFG_EXTICR1_EXTI2_PA | SYSCFG_EXTICR1_EXTI3_PA);
  EXTI->IMR  |=  (1 << BUTTON_PIN_0) | (1 << BUTTON_PIN_1) | (1 << BUTTON_PIN_2) | (1 << BUTTON_PIN_3);
  EXTI->FTSR |=  (1 << BUTTON_PIN_0) | (1 << BUTTON_PIN_1) | (1 << BUTTON_PIN_2) | (1 << BUTTON_PIN_3);;
  NVIC_SetPriority(EXTI0_IRQn, 0);
  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_SetPriority(EXTI1_IRQn, 0);
  NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_SetPriority(EXTI2_IRQn, 0);
  NVIC_EnableIRQ(EXTI2_IRQn);
  NVIC_SetPriority(EXTI3_IRQn, 0);
  NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI0_IRQHandler(void) {
  if (EXTI->PR & (1 << BUTTON_PIN_0)) {
    EXTI->PR |= (1 << BUTTON_PIN_0);
    EXTI->IMR  &=  ~(1 << BUTTON_PIN_0);
    TIM2->ARR = 20;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
}

void EXTI1_IRQHandler(void) {
  if (EXTI->PR & (1 << BUTTON_PIN_1)) {
    EXTI->PR |= (1 << BUTTON_PIN_1);
    EXTI->IMR  &=  ~(1 << BUTTON_PIN_1);
    TIM2->ARR = 20;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
}

void EXTI2_IRQHandler(void) {
  if (EXTI->PR & (1 << BUTTON_PIN_2)) {
    EXTI->PR |= (1 << BUTTON_PIN_2);
    EXTI->IMR  &=  ~(1 << BUTTON_PIN_2);
    TIM2->ARR = 20;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
}

void EXTI3_IRQHandler(void) {
  if (EXTI->PR & (1 << BUTTON_PIN_3)) {
    EXTI->PR |= (1 << BUTTON_PIN_3);
    EXTI->IMR  &=  ~(1 << BUTTON_PIN_3);
    TIM2->ARR = 20;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
  }
}