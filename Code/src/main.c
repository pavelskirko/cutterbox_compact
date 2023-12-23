#include "main.h"

uint32_t count = 30;
uint32_t count_update = 0;
uint8_t countdown = 0;
uint8_t countdown_update = 0;
volatile uint32_t interface_update = 0;

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
  RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSE | RCC_PLLCFGR_PLLM_3); //25 / 8 = 3.125 
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_6; // 3.125 * 64 = 200
//  RCC->PLLCFGR |= (RCC_PLLCFGR_PLLP_0); // 200 / 2 =  100
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLQ);
  RCC->CR |= RCC_CR_PLLON;          // Enable PLL
  while (!(RCC->CR & RCC_CR_PLLRDY)) {} // Wait for PLL to stabilize
  
  // Set the system clock source to PLL
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {} // Wait for PLL as system clock
  
  // Configure the AHB, APB1, and APB2 prescalers (if needed)
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler = 1
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 prescaler = 2 100 / 2 = 50
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV16; // APB2 prescaler = 8 (21)
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
  ScreenInit();
  TIM5_Init();
  TIM2_Init();
  Buttons_Init();

   GPIOB->MODER &= ~GPIO_MODER_MODER1;
  GPIOB->MODER |= GPIO_MODER_MODER1_0;
  GPIOB->OTYPER |= GPIO_OTYPER_OT_1;
//  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_0;
  GPIOB->ODR |= GPIO_ODR_ODR_1;
  InterfaceDraw(0, 0, "m");
  set_first_symbol();
  
  while (1)
  {
    ButtonProcessing();
    if (interface_update & !countdown)
    {
      interface_update = 0;
      InterfaceUpdate();
    }
    else if (countdown & countdown_update)
    {
      countdown_update = 0;
      CountdownProcessing();
      CountDownInterfaceUpdate();
    }
  }
}

void FinishOperation()
{
  TIM5->CR1 &= ~TIM_CR1_CEN;
  TIM2->CR1 &= ~TIM_CR1_CEN;
  countdown = 0;
  interface_update= 0;
}

