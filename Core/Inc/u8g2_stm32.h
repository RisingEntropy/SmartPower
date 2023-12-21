//
// Created by RisingEntropy on 2023/7/10.
//

#ifndef L051MAX77857_U8G2_STM32_H
#define L051MAX77857_U8G2_STM32_H
#include "main.h"
#include "u8g2.h"
#include "stm32l0xx_hal.h"
#define u8 uint8_t
#define MAX_LEN    128  //
#define OLED_ADDRESS  0x78 // oled
#define OLED_CMD   0x00  //
#define OLED_DATA  0x40  //

/* USER CODE BEGIN Prototypes */
uint8_t entropy_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);
void testDrawPixelToFillScreen(u8g2_t *u8g2);
#endif //L051MAX77857_U8G2_STM32_H
