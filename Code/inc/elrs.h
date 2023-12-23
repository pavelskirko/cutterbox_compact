#ifndef ELRS
#define ELRS

#include "stm32f4xx.h"
#include "main.h"

void uart_init();
char UART1_ReceiveChar(void);
void UART1_SendChar(char ch);

#endif