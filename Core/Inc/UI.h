//
// Created by RisingEntropy on 2023/7/10.
//

#ifndef L051MAX77857_UI_H
#define L051MAX77857_UI_H
#include "stm32l0xx_hal.h"
#include "u8g2.h"
#define UI_DO_OUTPUT 1
#define UI_NO_OUTPUT 0
//选择框套在哪
#define UI_SELECTING_VOLTAGE_FRAC 0  //套在小数部分，如5.7的7上
#define UI_SELECTING_VOLTAGE_INT 1  //套在整数部分，如5.7的5上
#define UI_SELECTING_OUTPUT 2  //套在是否输出上面
#define UI_LOCKED 1
#define UI_UNLOCKED 0
struct UI{
    uint8_t output_state; // 是否输出
    uint8_t select;  //选择框选择在哪
    uint8_t voltage;  //输出电压，共150档0.0V-15.0V，但是小于4.5V会被修改为4.5V,大于15V会被修改为15V
    uint8_t locked;
    float power;    //输出功率（假）
};
void UI_Init(struct UI *ui);
void UI_render_UI(struct UI *ui, u8g2_t *u8g2);
void UI_update_voltage(struct UI *ui, uint8_t voltage);
void UI_move_select(struct UI *ui);
void UI_set_select(struct UI *ui, uint8_t sel);
void UI_turn_on(struct UI *ui);
void UI_turn_off(struct UI *ui);
void UI_switch_power(struct UI *ui);
void UI_switch_lock(struct UI *ui);
#endif //L051MAX77857_UI_H
