//
// Created by Entropy on 2023/3/3.
//
#include "EntropyI2C.h"

static void entropy_delay(uint32_t delay){
    volatile uint32_t dly = delay;
    while(dly--)__NOP();
}
void entropy_set_OD(struct I2C_Port *port) {//设置输入or输出
    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin = port->SCK_pin;
    HAL_GPIO_Init(port->SCK_port, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = port->SDA_pin;
    HAL_GPIO_Init(port->SDA_port, &GPIO_InitStruct);
}

uint8_t entropy_gpio_read(struct I2C_Port *port){
    if ((port->SDA_port->IDR & port->SDA_pin) != (uint32_t)GPIO_PIN_RESET){
        return 1;
    }else{
        return 0;
    }
}
struct I2C_Port entropy_get_i2c_port(GPIO_TypeDef *SCK_port, GPIO_TypeDef *SDA_port, uint16_t SCK_pin, uint16_t SDA_pin, uint8_t address){//获取一个I2C实例
    struct I2C_Port port = {0};
    port.SCK_port = SCK_port;
    port.SDA_port = SDA_port;
    port.SDA_pin = SDA_pin;
    port.SCK_pin = SCK_pin;
    port.address = address;
    port.bit_format = ENTROPY_MSB;
    entropy_set_OD(&port);
    return port;
}
void entropy_set_low(struct I2C_Port *port, uint8_t operation) {//拉高
    if(operation==ENTROPY_OPERATION_SCK){
        port->SCK_port->BSRR = (uint32_t)port->SCK_pin<< 16u;
        //->BSRR = (uint32_t)GPIO_Pin << 16u;
    }else{
        port->SDA_port->BSRR = (uint32_t)port->SDA_pin<< 16u;
    }
}
void entropy_set_high(struct I2C_Port *port, uint8_t operation) {//拉低
    if(operation==ENTROPY_OPERATION_SCK){
        port->SCK_port->BSRR = (uint32_t)port->SCK_pin;
        //->BSRR = (uint32_t)GPIO_Pin << 16u;
    }else{
        port->SDA_port->BSRR = (uint32_t)port->SDA_pin;
    }
}
void entropy_start(struct I2C_Port *port){
    entropy_set_high(port, ENTROPY_OPERATION_SCK);
    entropy_set_high(port, ENTROPY_OPERATION_SDA);
    entropy_delay(20);
    entropy_set_low(port, ENTROPY_OPERATION_SDA);
    entropy_delay(20);
    entropy_set_low(port, ENTROPY_OPERATION_SCK);
    entropy_delay(20);

}
void entropy_stop(struct I2C_Port *port){
    entropy_set_low(port, ENTROPY_OPERATION_SDA);
    entropy_set_high(port, ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    entropy_set_high(port, ENTROPY_OPERATION_SDA);
}

void entropy_ack(struct I2C_Port *port){
    entropy_set_low(port,ENTROPY_OPERATION_SDA);
    entropy_delay(20);
    entropy_set_high(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    entropy_set_low(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    entropy_set_high(port,ENTROPY_OPERATION_SDA);
}
void entropy_nack(struct I2C_Port *port){
    entropy_set_high(port,ENTROPY_OPERATION_SDA);
    entropy_delay(20);
    entropy_set_high(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    entropy_set_low(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
}
uint8_t entropy_read_byte(struct I2C_Port *port){
    uint8_t buff = 0;
    for(uint8_t i = 0;i<8;i++){
        buff <<= 1;
        entropy_set_high(port,ENTROPY_OPERATION_SCK);
        entropy_delay(20);
        if(entropy_gpio_read(port)){
            buff|=1;
        }
        entropy_set_low(port,ENTROPY_OPERATION_SCK);
        entropy_delay(20);
    }

    return buff;

}
void entropy_write_byte(struct I2C_Port *port, uint8_t byte){
    for(int i = 0;i<8;i++){
        if(byte&0x80){
            entropy_set_high(port, ENTROPY_OPERATION_SDA);
        }else{
            entropy_set_low(port,ENTROPY_OPERATION_SDA);
        }
        entropy_delay(20);
        entropy_set_high(port,ENTROPY_OPERATION_SCK);
        entropy_delay(20);
        entropy_set_low(port,ENTROPY_OPERATION_SCK);
        if(i==7)entropy_set_high(port,ENTROPY_OPERATION_SDA);
        byte<<=1;
        entropy_delay(20);
    }
}
uint8_t entropy_read(struct I2C_Port *port, uint8_t*arr, size_t size){
    entropy_start(port);
    entropy_write_byte(port, (port->address<<1u)|ENTROPY_READ);
    if(entropy_wait(port))
        return ENTROPY_RESULT_ERROR;
    for(size_t i = 0;i<size;i++){
        arr[i] = entropy_read_byte(port);
        if(i==size-1){
            entropy_nack(port);
        }else{
            entropy_ack(port);
        }
    }
    entropy_stop(port);
    return ENTROPY_RESULT_SUCCESS;
}
bool entropy_wait(struct I2C_Port *port){
    entropy_set_high(port,ENTROPY_OPERATION_SDA);
    entropy_delay(20);
    entropy_set_high(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    if(entropy_gpio_read(port)){
//        entropy_delay(20);
        entropy_set_low(port,ENTROPY_OPERATION_SCK);
        entropy_delay(20);
        return true;
    }
//    entropy_delay(20);
    entropy_set_low(port,ENTROPY_OPERATION_SCK);
    entropy_delay(20);
    return false;

}
uint8_t entropy_write(struct I2C_Port *port, uint8_t*arr, size_t size){
    entropy_start(port);
    entropy_write_byte(port, (port->address<<1u)|ENTROPY_WRITE);
    if(entropy_wait(port)){
        return ENTROPY_RESULT_ERROR;
    }
    for(size_t i = 0;i<size;i++){
        entropy_write_byte(port,arr[i]);
        if(entropy_wait(port)) {
            entropy_stop(port);
            return ENTROPY_RESULT_ERROR;
        }
    }
    entropy_stop(port);
    return ENTROPY_RESULT_SUCCESS;
}
uint8_t entropy_write_register(struct I2C_Port *port, uint8_t reg, uint8_t *data, size_t size){
    entropy_start(port);
    entropy_write_byte(port, (port->address<<1u)|ENTROPY_WRITE);
    if(entropy_wait(port)){
//        entropy_stop(port);
    }
    entropy_write_byte(port, reg);
    if(entropy_wait(port)){
//        entropy_stop(port);
//        return ENTROPY_RESULT_ERROR; //MP8862似乎ACK有点问题，要很慢才行，直接给他ban了
    }
    for(size_t i = 0;i<size;i++){
        entropy_write_byte(port,data[i]);
        if(entropy_wait(port)) {
            entropy_stop(port);
            return ENTROPY_RESULT_ERROR;
        }
    }
    entropy_stop(port);
    return ENTROPY_RESULT_SUCCESS;
}
uint8_t entropy_read_register(struct I2C_Port *port, uint8_t reg, uint8_t *data, size_t size){
    entropy_start(port);
    entropy_write_byte(port,(port->address<<1u)|ENTROPY_WRITE);
    if(entropy_wait(port)){
//        entropy_stop(port);
//        return ENTROPY_RESULT_ERROR; //MP8862似乎ACK有点问题，要很慢才行，直接给他ban了
    }

    entropy_write_byte(port,reg);
    if(entropy_wait(port)){
//        entropy_stop(port);
//        return ENTROPY_RESULT_ERROR; //MP8862似乎ACK有点问题，要很慢才行，直接给他ban了
    }
    entropy_stop(port);
//    entropy_delay(20);
    uint8_t stat = entropy_read(port,data,size);

    return stat;
}
/*
 * ****************
 * return 1 denotes the device is available
 */
uint8_t entropy_check_device(struct I2C_Port *port){
    uint8_t result;
    entropy_start(port);
    entropy_write_byte(port, (port->address<<1)|ENTROPY_WRITE);
    result = entropy_wait(port);
    entropy_stop(port);
    return !result;
}
//void entropy_set_MSB(struct I2C_Port *port){
//    port->bit_format = ENTROPY_MSB;
//}
//void entropy_set_LSB(struct I2C_Port *port){
//    port->bit_format = ENTROPY_LSB;
//}

//uint8_t entropy_reverse_byte(uint8_t v){
//    uint8_t res = 0;
//    for(uint8_t i = 0;i<8;i++){
//        res = ((res<<1)|(v&1));
//        v>>=1;
//    }
//    return res;
//}