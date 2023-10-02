#include "timer.h"

uint8_t button_actions = 0;

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
      button_actions |= (1<<0);
      EXTI->IMR  |=  (1 << 0);
    }
  }
  else if ((GPIOA->IDR & GPIO_IDR_IDR_1) == 0)
  {
    TIM2->ARR = 5;
    button_counter++;
    if(button_counter == 5)
    {
      button_counter = 0;
      TIM2->CR1 &= ~TIM_CR1_CEN;
      button_actions |= (1<<1);
      EXTI->IMR  |=  (1 << 1);
    }
  }
  else if ((GPIOA->IDR & GPIO_IDR_IDR_2) == 0)
  {
    TIM2->ARR = 5;
    button_counter++;
    if(button_counter == 5)
    {
      button_counter = 0;
      TIM2->CR1 &= ~TIM_CR1_CEN;
      button_actions |= (1<<2);
      EXTI->IMR  |=  (1 << 2);
    }
  }
  else if ((GPIOA->IDR & GPIO_IDR_IDR_3) == 0)
  {
    
    TIM2->ARR = 5;
    button_counter++;
    if(button_counter == 5)
    {
      button_counter = 0;
      TIM2->CR1 &= ~TIM_CR1_CEN;
      button_actions |= (1<<3);
      EXTI->IMR  |=  (1 << 3);
    }
  }
  else
  {
    button_counter_n++;
    if(button_counter_n == 5)
    {
      button_counter_n = 0;
      TIM2->CR1 &= ~TIM_CR1_CEN;
      EXTI->IMR  |=  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) ;
      button_counter = 0;
    }
  }
}

void ButtonAction0()
{
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
  number_lightup = 0;
}

void ButtonAction1()
{
  if (selected_symb == 2)
  {
    time_digits[selected_symb] ^= 1;
    if(time_digits[selected_symb])
    {
      units[0] = 's';
    }
    else
    {
      units[0] = 'm';
    }
  }
  else if (time_digits[selected_symb] < 9)
  {
    time_digits[selected_symb]++;
  }
  else
  {
    time_digits[selected_symb] = 0;
  }
  interface_update = 1;
  number_lightup = 1;
}

void ButtonAction2()
{
  if (selected_symb == 2)
  {
    time_digits[selected_symb] ^= 1;
    if(time_digits[selected_symb])
    {
      units[0] = 's';
    }
    else
    {
      units[0] = 'm';
    }
  }
  else if (time_digits[selected_symb] > 0)
  {
    time_digits[selected_symb]--;
  }
  else
  {
    time_digits[selected_symb] = 9;
  }
  interface_update = 1;
  number_lightup = 1;
}

void ButtonAction3()
{
  uint32_t timer = 0;
  switch(time_digits[2])
  {
  case 0:
    timer = (time_digits[0] * 10 + time_digits[1]) * 60;
    break;
  case 1:
    timer = (time_digits[0] * 10 + time_digits[1]);
    break;
  }
}

void ButtonProcessing()
{
  if (button_actions & (1<<0))
  {
    ButtonAction0();
  }
  if (button_actions & (1<<1))
  {
    ButtonAction1();
  }
  if (button_actions & (1<<2))
  {
    ButtonAction2();
  }
  if (button_actions & (1<<3))
  {
    ButtonAction3();
  }
  button_actions = 0;
}
