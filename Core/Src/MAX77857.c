//
// Created by RisingEntropy on 2023/7/10.
//
#include "MAX77857.h"
void MAX77857_init(struct MAX77857_Handler *handler,struct I2C_Port *i2C_port, GPIO_TypeDef *EN_GPIO_port, uint16_t EN_GPIO_pin){
    handler->i2c_port = i2C_port;
    handler->EN_GPIO_port = EN_GPIO_port;
    handler->EN_GPIO_pin = EN_GPIO_pin;
    MAX77857_enable(handler);
    MAX77857_read_status(handler);
}
void MAX77857_enable(struct MAX77857_Handler *handler){
    HAL_GPIO_WritePin(handler->EN_GPIO_port,handler->EN_GPIO_pin, GPIO_PIN_SET);
}
void MAX77857_disable(struct MAX77857_Handler *handler){
    HAL_GPIO_WritePin(handler->EN_GPIO_port,handler->EN_GPIO_pin, GPIO_PIN_RESET);
}
uint8_t MAX77857_read_status(struct MAX77857_Handler *handler){
    uint8_t stat = 0;
    stat += entropy_read_register(handler->i2c_port,0x10,&handler->REG_INT_SRC,1);
    stat += entropy_read_register(handler->i2c_port,0x11,&handler->REG_INT_MASK,1);
    stat += entropy_read_register(handler->i2c_port,0x12,&handler->REG_CONT1,1);
    stat += entropy_read_register(handler->i2c_port,0x13,&handler->REG_CONT2,1);
    stat += entropy_read_register(handler->i2c_port,0x14,&handler->REG_CONT3,1);
    stat += entropy_read_register(handler->i2c_port,0x15,&handler->REG_I2C_CONFG,1);
    return stat==0x6u*6;
}
uint8_t MAX77857_write_status(struct MAX77857_Handler *handler){
    uint8_t stat = 0;
    stat += entropy_write_register(handler->i2c_port,0x10,&handler->REG_INT_SRC,1);
    stat += entropy_write_register(handler->i2c_port,0x11,&handler->REG_INT_MASK,1);
    stat += entropy_write_register(handler->i2c_port,0x12,&handler->REG_CONT1,1);
    stat += entropy_write_register(handler->i2c_port,0x13,&handler->REG_CONT2,1);
    stat += entropy_write_register(handler->i2c_port,0x14,&handler->REG_CONT3,1);
    stat += entropy_write_register(handler->i2c_port,0x15,&handler->REG_I2C_CONFG,1);
    return stat==0x6u*6;
}
uint8_t MAX77857_set_compensation_RC(struct MAX77857_Handler *handler, uint8_t level){
    if(level>7)level=7;
    uint8_t tmp = handler->REG_CONT1;
    tmp = tmp&0x1F; // [7:5]位置0
    tmp |= level<<5;
    handler->REG_CONT1 = tmp;
    return entropy_write_register(handler->i2c_port,0x12,&tmp,1)==ENTROPY_RESULT_SUCCESS;
}
uint8_t MAX77857_set_switching_freq(struct MAX77857_Handler *handler, uint8_t level){
    if(level>3)level=3;
    uint8_t tmp = handler->REG_CONT1;
    tmp = tmp&0xE7; // [4:3]位置0
    tmp |= level<<3;
    handler->REG_CONT1 = tmp;
    return entropy_write_register(handler->i2c_port,0x12,&tmp,1)==ENTROPY_RESULT_SUCCESS;
}
uint8_t MAX77857_set_switching_current_limit(struct MAX77857_Handler *handler, uint8_t level){
    if(level>7)level=7;
    uint8_t tmp = handler->REG_CONT1;
    tmp = tmp&0xF8; // [2:0]位置0
    tmp |= level;
    handler->REG_CONT1 = tmp;
    return entropy_write_register(handler->i2c_port,0x12,&tmp,1)==ENTROPY_RESULT_SUCCESS;
}
uint8_t MAX77857_set_output_voltage(struct MAX77857_Handler*handler, float voltage){
    if(voltage<4.5f)voltage = 4.5f;
    if(voltage>15.f)voltage = 15.f;
    uint8_t vol_data = 0x3C;
    vol_data += (uint8_t)((voltage-4.5f)/(10.5f)*(0xCC+1-0x3D));
    handler->REG_CONT2 = vol_data;
    return entropy_write_register(handler->i2c_port, 0x13, &vol_data, 1);
}
