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
    // Clear the EXTI status flag.
    EXTI->PR |= (1 << BUTTON_PIN_1);
    // Toggle the global 'led on?' variable.
    if(selected_symb < 3)
    {
      selected_symb++;
    }
    else
    {
      selected_symb = 0;
    }
  }
}