/*
	*
  ******************************************************************************
  * @file    myiic.c
  * @author  Sercan ERAT
  * @version V1.0.0
  ******************************************************************************
	*
*/

/*

SCL -> PB8
SDA -> PB9

*/

#include "myiic.h"
#include "delay.h"

void IIC_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			
	GPIO_Init(GPIOB, &GPIO_InitStruct);				
	  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); // SDA
	
	//Configure I2C1 
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;		
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_Init(I2C1, &I2C_InitStruct);				
	
	//Enable I2C1
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction){
	
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, address, direction);
	   
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

void I2C_write(I2C_TypeDef* I2Cx, uint8_t data) {
	
	I2C_SendData(I2Cx, data);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t I2C_read_ack(I2C_TypeDef* I2Cx){
	
	uint8_t data;
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	data = I2C_ReceiveData(I2Cx);
	return data;
}

uint8_t I2C_read_nack(I2C_TypeDef* I2Cx){
	
	uint8_t data; 
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	data = I2C_ReceiveData(I2Cx);
	return data;
}

void I2C_stop(I2C_TypeDef* I2Cx){
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C_readByte(uint8_t slave_address, uint8_t readAddr, uint8_t *data){
	
	I2C_start(I2C1, slave_address, I2C_Direction_Transmitter);
	I2C_write(I2C1, readAddr); 
	I2C_stop(I2C1);
	I2C_start(I2C1, slave_address, I2C_Direction_Receiver); 
	*data = I2C_read_nack(I2C1); 
}

void I2C_writeByte(uint8_t slave_address, uint8_t writeAddr, uint8_t data){
	
	I2C_start(I2C1, slave_address, I2C_Direction_Transmitter); 
	I2C_write(I2C1, writeAddr);
  I2C_write(I2C1, data);
  I2C_stop(I2C1);
}

void I2C_readBytes(uint8_t slave_address, uint8_t readAddr, uint8_t length, uint8_t *data){
	
	I2C_start(I2C1, slave_address, I2C_Direction_Transmitter); 	
	I2C_write(I2C1, readAddr); 																	
	I2C_stop(I2C1); 																						
	I2C_start(I2C1, slave_address, I2C_Direction_Receiver); 		
	while(length){
		if(length==1)
			*data = I2C_read_nack(I2C1);
		else
			*data = I2C_read_ack(I2C1);																
		
		data++;
		length--;
	}
}

void I2C_writeBytes(uint8_t slave_address, uint8_t writeAddr, uint8_t length, uint8_t *data){
	
	int i=0;
	I2C_start(I2C1, slave_address, I2C_Direction_Transmitter);
	I2C_write(I2C1, writeAddr);
	for(i=0; i<length; i++){	
		I2C_write(I2C1, data[i]);
  }
  I2C_stop(I2C1);
}

void I2C_readBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t *data){
	
  uint8_t tmp;
  I2C_readByte(slave_address, regAddr, &tmp);  
  *data = tmp & (1 << bitNum);
}

void I2C_readBits(uint8_t slave_address, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data){
	
  // 01101001 read byte
  // 76543210 bit numbers
  //    xxx   args: bitStart=4, length=3
  //    010   masked
  //   -> 010 shifted
  uint8_t mask,tmp;
  I2C_readByte(slave_address, regAddr, &tmp); 
  mask = ((1 << length) - 1) << (bitStart - length + 1);
  tmp &= mask;
  tmp >>= (bitStart - length + 1);
	*data = tmp;
}

void I2C_writeBit(uint8_t slave_address, uint8_t regAddr, uint8_t bitNum, uint8_t data){
  
	uint8_t tmp;
  I2C_readByte(slave_address, regAddr, &tmp);  
  tmp = (data != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
  I2C_writeByte(slave_address,regAddr,tmp); 
}

void I2C_writeBits(uint8_t slave_address, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data){
	
  //      010 value to write
  // 76543210 bit numbers
  //    xxx   args: bitStart=4, length=3
  // 00011100 mask byte
  // 10101111 original value (sample)
  // 10100011 original & ~mask
  // 10101011 masked | value
  uint8_t tmp,mask;
  I2C_readByte(slave_address, regAddr, &tmp);
  mask = ((1 << length) - 1) << (bitStart - length + 1);
  data <<= (bitStart - length + 1); 
  data &= mask; 
  tmp &= ~(mask); 
  tmp |= data; 
  I2C_writeByte(slave_address, regAddr, tmp);	
}

void I2C_writeWord(uint8_t slave_address, uint8_t writeAddr, uint16_t data){
	
	I2C_start(I2C1, slave_address, I2C_Direction_Transmitter); 
	I2C_write(I2C1, writeAddr);         
  I2C_write(I2C1, (data >> 8));      // send MSB
	I2C_write(I2C1, (data << 8));			 // send LSB
  I2C_stop(I2C1);
}
