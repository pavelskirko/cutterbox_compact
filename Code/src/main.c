#include "main.h"

void delay(uint32_t ms) {
  // Assuming a system clock of 16MHz
  uint32_t ticks = ms * 16000; // Each millisecond has 16,000 ticks
  while (ticks--) {
    __NOP(); // No operation, just to waste time
  }
}

void SysInit(void) {
  // Reset the RCC configuration to the default reset state
  RCC->CR |= RCC_CR_HSION;          // Turn on HSI oscillator
  RCC->CFGR = 0x00000000;           // Reset CFGR register
  RCC->CR &= ~RCC_CR_HSEON;         // Disable HSE oscillator
  RCC->CR &= ~RCC_CR_PLLON;         // Disable PLL
  RCC->CR &= ~RCC_CR_PLLI2SON;     // Disable PLLI2S
  RCC->PLLCFGR = 0x24003010;       // Reset PLLCFGR register
  RCC->CR &= ~RCC_CR_CSSON;         // Disable CSS
  RCC->CIR = 0x00000000;            // Reset CIR register
  
  // Configure the system clock
  RCC->CR |= RCC_CR_HSEON;          // Enable HSE oscillator
  while (!(RCC->CR & RCC_CR_HSERDY)) {} // Wait for HSE to stabilize
  
  // Configure PLL to multiply HSE by 168 to get 168MHz system clock
  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE | RCC_PLLCFGR_PLLM_3); //25 / 8 = 3.125 
  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLN_0| RCC_PLLCFGR_PLLN_1| RCC_PLLCFGR_PLLN_3| RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6); // 3.125 * 84 = 262
  //    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLP_0); // 262 / 4 =  131
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLQ);
  RCC->CR |= RCC_CR_PLLON;          // Enable PLL
  while (!(RCC->CR & RCC_CR_PLLRDY)) {} // Wait for PLL to stabilize
  
  // Set the system clock source to PLL
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {} // Wait for PLL as system clock
  
  // Configure the AHB, APB1, and APB2 prescalers (if needed)
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; // APB1 prescaler = 4 131 / 4 = 32.8
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; // APB2 prescaler = 2
}

int main (void)
{
  // Enable clock for GPIO Port D (where LED is connected)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  
  // Configure the GPIO Pin (D12) as an output
  GPIOC->MODER |= GPIO_MODER_MODER13_0; // General purpose output mode
  GPIOC->OTYPER &= ~GPIO_OTYPER_OT_13;  // Push-pull output
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13; // High speed
  GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13; // No pull-up, no pull-down
  SysInit();
  I2C_init();
  draw_a_number(0);
  draw_a_number(1);
  draw_a_number(2);
  draw_a_number(3);
  draw_a_number(4);
  draw_a_number(5);
  draw_a_number(6);
  SSD1306_UpdateScreen();
  while (1)
  {
    GPIOC->ODR ^= GPIO_ODR_ODR_13;
    delay(1000);
  }
}

