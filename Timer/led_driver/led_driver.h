#define MAX_NUM_DISPLAY 8
#define ANODA 0
#define KATODA 1

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t total_displays;
uint8_t position;
uint8_t led_type;
volatile uint8_t number[MAX_NUM_DISPLAY];
volatile uint8_t *digit_ddr;
volatile uint8_t *digit_port;
volatile uint8_t *display_ddr;
volatile uint8_t *display_port;

void LEDInit(uint8_t total_displays_f, uint8_t led_type_f, volatile uint8_t *digit_ddr_wsk, volatile uint8_t *digit_port_wsk, volatile uint8_t *display_ddr_wsk, volatile uint8_t *display_port_wsk);

void LEDSetValue(char *val);

void LEDSetNumber(int nr);

// 1 - 9 -> 01 - 09
void LEDSetNumberWithZero(int nr);

// set position first char on screen. 0 - first, 1 - second, etc.
void LEDSetPosition(uint8_t pos);

void clearDisplay();

uint8_t convertChar(char s);

void setTimer0();

#include "led_driver.c"