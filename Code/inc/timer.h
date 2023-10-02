#ifndef TIMER
#define TIMER

#include "stm32f4xx.h"
#include "main.h"

void TIM5_Init();
void TIM2_Init();
void ButtonAction0();
void ButtonAction1();
void ButtonAction2();
void ButtonAction3();
void ButtonProcessing();
void CountdownProcessing();

extern uint32_t timer;
extern uint32_t real_timer;

#endif