#include "main.h"
#include "stm32f4xx.h"

uint8_t bool = 0;
uint8_t index = 0;
uint32_t global_timer = 0; // in 1/10 sec
uint8_t slaveID = 0x3C;
//uint8_t id_data = 0;

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

void i2c_init()
{
  
  #define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
  #define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
  #define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
  #define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
  #define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
  #define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
  #define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

  #define SSD1306_NORMALDISPLAY       0xA6
  #define SSD1306_INVERTDISPLAY       0xA7
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIOB->AFR[0] |= 0x4 << 24;
  GPIOB->AFR[0] |= 0x4 << 28;
  GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
  GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
  GPIOB->OTYPER |= (GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);
  GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR7);
  GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0);
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  /*Reset I2C*/
    I2C1->CR1 |= I2C_CR1_SWRST;    /*!BUSY ON */
    I2C1->CR1 &= ~I2C_CR1_SWRST;   /*!BUSY OFF*/
  I2C1->CR1 &= ~I2C_CR1_PE;
  I2C1->CR2 &= ~I2C_CR2_FREQ;
  I2C1->CR2 |= 42;
  I2C1->CCR |= 210;
  I2C1->TRISE = 43;
  I2C1->CR1 |= I2C_CR1_PE;

}

void ssd1306_write(uint32_t data)
{
    
    // Wait until I2C1 is not busy
    while ((I2C1->SR2 & I2C_SR2_BUSY) != 0);

    // Generate start condition
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until start condition is generated
    while ((I2C1->SR1 & I2C_SR1_SB) == 0);

    // Send slave address with write bit
    I2C1->DR = (slaveID << 1);

    // Wait until address is sent
    while ((I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);

    // Clear the ADDR bit by reading SR2 register
    (void) I2C1->SR2;
   // Send data
    I2C1->DR = 0x0;

    // Wait until data is sent
    while ((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)) == 0);

    // Send data
    I2C1->DR = data;

    // Wait until data is sent
    while ((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)) == 0);

    // Generate stop condition
    I2C1->CR1 |= I2C_CR1_STOP;
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
    i2c_init();
    ssd1306_write(0xAE); //display off
	ssd1306_write(0x20); //Set Memory Addressing Mode   
	ssd1306_write(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_write(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_write(0xC8); //Set COM Output Scan Direction
	ssd1306_write(0x00); //---set low column address
	ssd1306_write(0x10); //---set high column address
	ssd1306_write(0x40); //--set start line address
	ssd1306_write(0x81); //--set contrast control register
	ssd1306_write(0xFF);
	ssd1306_write(0xA1); //--set segment re-map 0 to 127
	ssd1306_write(0xA6); //--set normal display
	ssd1306_write(0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_write(0x3F); //
	ssd1306_write(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_write(0xD3); //-set display offset
	ssd1306_write(0x00); //-not offset
	ssd1306_write(0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_write(0xF0); //--set divide ratio
	ssd1306_write(0xD9); //--set pre-charge period
	ssd1306_write(0x22); //
	ssd1306_write(0xDA); //--set com pins hardware configuration
	ssd1306_write(0x12);
	ssd1306_write(0xDB); //--set vcomh
	ssd1306_write(0x20); //0x20,0.77xVcc
	ssd1306_write(0x8D); //--set DC-DC enable
	ssd1306_write(0x14); //
	ssd1306_write(0xAF); //--turn on SSD1306 panel
        ssd1306_write(SSD1306_INVERTDISPLAY);
    while (1) {
        // Toggle the LED (D12)
        GPIOC->ODR ^= GPIO_ODR_ODR_13;
        delay(1000); // Delay for 1000ms (1 second)
    }
}

