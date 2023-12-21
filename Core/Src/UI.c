//
// Created by RisingEntropy on 2023/7/10.
//
#include "UI.h"
char buffer[10];
void UI_Init(struct UI *ui){
    ui->output_state = UI_NO_OUTPUT;
    ui->select = 0;
    ui->voltage = 45;
    ui->locked = UI_UNLOCKED;
    ui->power = 0.0f;
}
inline void write_to_buffer(uint8_t voltage){
    if(voltage>=100){
        buffer[0] = '1';
        buffer[1] = (voltage/10)%10+'0';
        buffer[2] = '.';
        buffer[3] = voltage%10+'0';
        buffer[4] = 'V';
    }else{
        buffer[0] = voltage/10+'0';
        buffer[1] = '.';
        buffer[2] = voltage%10+'0';
        buffer[3] = 'V';
    }
}
void UI_render_UI(struct UI *ui, u8g2_t *u8g2){
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_ncenB24_te); /*字库选择*/
    write_to_buffer(ui->voltage);
    u8g2_DrawStr(u8g2,36-(ui->voltage/10>10)*16,24,buffer);
    u8g2_DrawStr(u8g2,36,50,"5.7W"); //暂时没用到
    u8g2_DrawLine(u8g2,0,52,127,52);
    u8g2_SetFont(u8g2,u8g2_font_ncenB08_tf);
    if(ui->output_state==UI_DO_OUTPUT){
        u8g2_DrawStr(u8g2,0,63,"ON");
    }else{
        u8g2_DrawStr(u8g2,0,63,"OFF");
    }
    u8g2_DrawLine(u8g2,64,52,64,127);
    if(ui->locked==UI_LOCKED){
        u8g2_DrawStr(u8g2,64,63,"LOCKED");
    }else{
        u8g2_DrawStr(u8g2,64,63,"UNLOCKED");
    }
    u8g2_SendBuffer(u8g2);
}
void UI_update_voltage(struct UI *ui, uint8_t voltage){
    if(voltage<1)voltage = 1;
    if(voltage>150)voltage = 150;
    ui->voltage = voltage;
}
void UI_move_select(struct UI *ui){
    ui->select++;
    ui->select%=3;
}
void UI_set_select(struct UI *ui, uint8_t sel){
    ui->select = sel%3;
}
void UI_turn_on(struct UI *ui){
    ui->power = UI_DO_OUTPUT;
}
void UI_turn_off(struct UI *ui){
    ui->power = UI_NO_OUTPUT;
}
void UI_switch_power(struct UI *ui){
    ui->output_state ^= 1;
}
void UI_switch_lock(struct UI *ui){
    ui->locked ^= 1;
}