//
// Created by RisingEntropy on 2023/7/10.
//

#ifndef L051MAX77857_MAX77857_H
#define L051MAX77857_MAX77857_H
#include "EntropyI2C.h"
#define SWITCH_FREQ_1_2MHz 0
#define SWITCH_FREQ_1_5MHz 1
#define SWITCH_FREQ_1_8MHz 2
#define SWITCH_FREQ_2_1MHz 3
#define SWITCH_CURRENT_LIMIT_7_0A 0
#define SWITCH_CURRENT_LIMIT_6_2A 1
#define SWITCH_CURRENT_LIMIT_5_6A 2
#define SWITCH_CURRENT_LIMIT_4_6A 3
#define SWITCH_CURRENT_LIMIT_3_8A 4
#define SWITCH_CURRENT_LIMIT_2_8A 5
#define SWITCH_CURRENT_LIMIT_1_8A 6
#define SWITCH_CURRENT_LIMIT_0_99A 7
struct MAX77857_Handler{
    struct I2C_Port *i2c_port;
    GPIO_TypeDef *EN_GPIO_port;
    uint16_t EN_GPIO_pin;
    uint8_t REG_INT_SRC;
    uint8_t REG_INT_MASK;
    uint8_t REG_CONT1;
    uint8_t REG_CONT2;
    uint8_t REG_CONT3;
    uint8_t REG_I2C_CONFG;
};
void MAX77857_init(struct MAX77857_Handler *handler,struct I2C_Port *i2C_port, GPIO_TypeDef *EN_GPIO_port, uint16_t EN_GPIO_pin);
void MAX77857_enable(struct MAX77857_Handler *handler);
void MAX77857_disable(struct MAX77857_Handler *handler);
uint8_t MAX77857_read_status(struct MAX77857_Handler *handler);
uint8_t MAX77857_write_status(struct MAX77857_Handler *handler);
uint8_t MAX77857_set_compensation_RC(struct MAX77857_Handler *handler, uint8_t level);
uint8_t MAX77857_set_switching_freq(struct MAX77857_Handler *handler, uint8_t level);
uint8_t MAX77857_set_switching_current_limit(struct MAX77857_Handler *handler, uint8_t level);
uint8_t MAX77857_set_output_voltage(struct MAX77857_Handler*handler, float voltage);


#endif //L051MAX77857_MAX77857_H
