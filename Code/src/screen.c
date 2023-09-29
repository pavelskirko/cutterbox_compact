#include "screen.h"

const uint16_t Numbers16x26 [] = {
  0x07F0,0x0FF8,0x1F7C,0x3E3E,0x3C1E,0x7C1F,0x7C1F,0x780F,0x780F,0x780F,0x780F,0x780F,0x780F,0x780F,0x7C1F,0x7C1F,0x3C1E,0x3E3E,0x1F7C,0x0FF8,0x07F0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [0]
  0x00F0,0x07F0,0x3FF0,0x3FF0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x01F0,0x3FFF,0x3FFF,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [1]
  0x0FE0,0x3FF8,0x3C7C,0x003C,0x003E,0x003E,0x003E,0x003C,0x003C,0x007C,0x00F8,0x01F0,0x03E0,0x07C0,0x0780,0x0F00,0x1E00,0x3E00,0x3C00,0x3FFE,0x3FFE,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [2]
  0x0FF0,0x1FF8,0x1C7C,0x003E,0x003E,0x003E,0x003C,0x003C,0x00F8,0x0FF0,0x0FF8,0x007C,0x003E,0x001E,0x001E,0x001E,0x001E,0x003E,0x1C7C,0x1FF8,0x1FE0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [3]
  0x0078,0x00F8,0x00F8,0x01F8,0x03F8,0x07F8,0x07F8,0x0F78,0x1E78,0x1E78,0x3C78,0x7878,0x7878,0xFFFF,0xFFFF,0x0078,0x0078,0x0078,0x0078,0x0078,0x0078,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [4]
  0x1FFC,0x1FFC,0x1FFC,0x1E00,0x1E00,0x1E00,0x1E00,0x1E00,0x1FE0,0x1FF8,0x00FC,0x007C,0x003E,0x003E,0x001E,0x003E,0x003E,0x003C,0x1C7C,0x1FF8,0x1FE0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [5]
  0x01FC,0x07FE,0x0F8E,0x1F00,0x1E00,0x3E00,0x3C00,0x3C00,0x3DF8,0x3FFC,0x7F3E,0x7E1F,0x3C0F,0x3C0F,0x3C0F,0x3C0F,0x3E0F,0x1E1F,0x1F3E,0x0FFC,0x03F0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [6]
  0x3FFF,0x3FFF,0x3FFF,0x000F,0x001E,0x001E,0x003C,0x0038,0x0078,0x00F0,0x00F0,0x01E0,0x01E0,0x03C0,0x03C0,0x0780,0x0F80,0x0F80,0x0F00,0x1F00,0x1F00,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [7]
  0x07F8,0x0FFC,0x1F3E,0x1E1E,0x3E1E,0x3E1E,0x1E1E,0x1F3C,0x0FF8,0x07F0,0x0FF8,0x1EFC,0x3E3E,0x3C1F,0x7C1F,0x7C0F,0x7C0F,0x3C1F,0x3F3E,0x1FFC,0x07F0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [8]
  0x07F0,0x0FF8,0x1E7C,0x3C3E,0x3C1E,0x7C1F,0x7C1F,0x7C1F,0x7C1F,0x3C1F,0x3E3F,0x1FFF,0x07EF,0x001F,0x001E,0x001E,0x003E,0x003C,0x38F8,0x3FF0,0x1FE0,0x0000,0x0000,0x0000,0x0000,0x0000, // Ascii = [9]
};

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

uint8_t slaveID = 0x3C;

static SSD1306_t SSD1306;

void I2C_init()
{
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

void ScreenInit()
{
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
  memset(SSD1306_Buffer, 0, sizeof(SSD1306_Buffer));
  SSD1306_UpdateScreen();
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, uint8_t color) 
{
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    /* Error */
    return;
  }
  
  /* Set color */
  if (color) 
  {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  } else {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}

void draw_a_number(uint8_t numb)
{
  uint32_t pixel;
  for (uint8_t i = 0; i < FONT_HEIGHT; i++) 
  {
    pixel = Numbers16x26[numb * FONT_HEIGHT + i];
    for (uint8_t j = 0; j < FONT_WIDTH; j++) 
    {
      if ((pixel << j) & 0x8000) 
      {
	SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), 1);
      } 
      else 
      {
        SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), 0);
      }
    }
  }
  SSD1306.CurrentX += FONT_WIDTH;
}

void SSD1306_UpdateScreen(void)
{
  uint8_t m;
  
  for (m = 0; m < 8; m++) 
  {
    ssd1306_write(0xB0 + m);
    ssd1306_write(0x00);
    ssd1306_write(0x10);
    
    while ((I2C1->SR2 & I2C_SR2_BUSY) != 0);
    I2C1->CR1 |= I2C_CR1_START;
    while ((I2C1->SR1 & I2C_SR1_SB) == 0);
    I2C1->DR = (slaveID << 1);
    while ((I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
    (void) I2C1->SR2;
    I2C1->DR = 0x40;
    while ((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)) == 0);
    for(uint32_t i = 0; i < SSD1306_WIDTH; i++)
    {
      I2C1->DR = SSD1306_Buffer[i+ m * SSD1306_WIDTH];
      while ((I2C1->SR1 & (I2C_SR1_BTF | I2C_SR1_AF)) == 0);
    }
    I2C1->CR1 |= I2C_CR1_STOP;
  }
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
