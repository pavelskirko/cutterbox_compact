#include "elrs.h"

void uart_init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);
  GPIOA->AFR[1] |= (7 << 4) | (7 << 8);
  

  USART1->CR1 &= ~USART_CR1_UE; // Disable UART1
  USART1->BRR = 0xee; // Set the baud rate
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable transmitter and receiver
  USART1->CR1 |= USART_CR1_UE; // Enable UART1
}

char UART1_ReceiveChar(void) {
    // Wait until the receive buffer is not empty
    while (!(USART1->SR & USART_SR_RXNE)) {}

    // Read and return the received character
    return (char)(USART1->DR & 0xFF);
}

void UART1_SendChar(char ch) {
    // Wait until the transmit buffer is empty
    while (!(USART1->SR & USART_SR_TXE)) {}

    // Send the character
    USART1->DR = (ch & 0xFF);
}
