#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

#define I2C I2C1

void IIC_Init(void);					

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

void I2C_readByte(uint8_t slave_address, uint8_t readAddr, uint8_t *data);
void I2C_writeByte(uint8_t slave_address, uint8_t writeAddr, uint8_t data);

void I2C_readBytes(uint8_t slave_address, uint8_t readAddr, uint8_t length, uint8_t *data);
void I2C_writeBytes(uint8_t slave_address, uint8_t writeAddr, uint8_t length, uint8_t *data);

void I2C_readBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
void I2C_readBits(uint8_t slave_address, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);

void I2C_writeBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t data);
void I2C_writeBits(uint8_t slave_address, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);

void I2C_writeWord(uint8_t slave_address, uint8_t writeAddr, uint16_t data);

#endif
