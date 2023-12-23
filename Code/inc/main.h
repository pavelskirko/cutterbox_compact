#ifndef MAIN
#define MAIN

#include "stm32f4xx.h"
#include "screen.h"
#include "timer.h"
#include "core_cm4.h"
#include "buttons.h"

void FinishOperation();
void UART();
char UART1_ReceiveChar();
void UART1_SendChar(char ch);

extern uint32_t count;
extern uint32_t count_update;
extern volatile uint32_t interface_update;
extern uint8_t countdown;
extern uint8_t countdown_update;

#endif