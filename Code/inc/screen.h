#ifndef SCREEN
#define SCREEN

#include "stm32f4xx.h"
#include <string.h>
#include "main.h"

typedef struct {
  uint16_t CurrentX;
  uint16_t CurrentY;
  uint8_t Initialized;
  uint16_t SelectedX;
  uint16_t SelectedY;
} SSD1306_t;

#define FONT_HEIGHT     26
#define FONT_WIDTH      16
#define SMALL_FONT_HEIGHT     18
#define SMALL_FONT_WIDTH      11
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7

void ssd1306_write(uint32_t data);
void SSD1306_UpdateScreen(void);
void draw_a_digit(uint8_t numb);
void draw_a_number(uint32_t numb);
void SSD1306_DrawPixel(uint16_t x, uint16_t y, uint8_t color);
void ScreenInit();
void I2C_init();
void draw_countdown(uint32_t numb);
void draw_a_symbol(char symb);
void InterfaceDraw(uint8_t digits_one, uint8_t digit_two, char * time_unit);
void InterfaceUpdate();
void ClearArea(uint16_t x, uint16_t y, uint16_t height, uint16_t width);
void set_first_symbol();
void CountDownInterfaceUpdate();
void ScreenFill();

extern uint8_t number_fade;
extern uint8_t selected_symb;
extern uint8_t time_digits[3];
extern char units[];
extern uint8_t number_lightup;

#endif