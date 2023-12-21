//
// Created by Entropy on 2023/3/3.
//

#ifndef ENTROPYI2C_ENTROPYI2C_H
#define ENTROPYI2C_ENTROPYI2C_H

#include <stdbool.h>
#include "stm32l0xx_hal.h"
#define ENTROPY_STATE_INPUT 0x01u  //设置GPIO状态输入
#define ENTROPY_STATE_OUTPUT 0x02u  //设置GPIO状态输出
#define ENTROPY_OPERATION_SCK 0x03u  //操作SCK
#define ENTROPY_OPERATION_SDA 0x04u  //操作SDA
#define ENTROPY_RESULT_SUCCESS 0x5u
#define ENTROPY_RESULT_ERROR 0x6u
#define ENTROPY_READ 1
#define ENTROPY_WRITE 0
#define ENTROPY_MSB 0x07u
#define ENTROPY_LSB 0x08u
struct I2C_Port{
    GPIO_TypeDef *SCK_port, *SDA_port;
    uint16_t SCK_pin, SDA_pin;
    uint8_t direction;
    uint8_t address;
    uint8_t bit_format;
};
static void entropy_delay(uint32_t delay);
void entropy_set_OD(struct I2C_Port*);//设置输入or输出
struct I2C_Port entropy_get_i2c_port(GPIO_TypeDef *SCK_port, GPIO_TypeDef *SDA_port, uint16_t SCK_pin, uint16_t SDA_pin, uint8_t address);//获取一个I2C实例
void entropy_set_high(struct I2C_Port *port, uint8_t operation);//拉高
void entropy_set_low(struct I2C_Port *port, uint8_t operation);//拉低
uint8_t entropy_gpio_read(struct I2C_Port *port);
void entropy_start(struct I2C_Port *port);
void entropy_stop(struct I2C_Port *port);
//void entropy_set_MSB(struct I2C_Port *port);
//void entropy_set_LSB(struct I2C_Port *port);
uint8_t entropy_read_byte(struct I2C_Port *port);
void entropy_write_byte(struct I2C_Port *port, uint8_t byte);
uint8_t entropy_read(struct I2C_Port *port, uint8_t*arr, size_t size);
uint8_t entropy_write(struct I2C_Port *port, uint8_t*arr, size_t size);
bool entropy_wait(struct I2C_Port *port);
//uint8_t entropy_reverse_byte(uint8_t);
void entropy_ack(struct I2C_Port *port);
void entropy_nack(struct I2C_Port *port);
uint8_t entropy_write_register(struct I2C_Port *port, uint8_t reg, uint8_t *data, size_t size);
uint8_t entropy_read_register(struct I2C_Port *port, uint8_t reg, uint8_t *data, size_t size);
uint8_t entropy_check_device(struct I2C_Port *port);
#endif //ENTROPYI2C_ENTROPYI2C_H
