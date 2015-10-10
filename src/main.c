/*
	*
  ******************************************************************************
  * @file    main.c
  * @author  Sercan ERAT
  * @version V1.0.0
  * @date    09-10-2015
  ******************************************************************************
	*
*/ 
	
	
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "main.h"
#include "delay.h"
#include <stdio.h>

#include "myiic.h"
#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "usart.h"

uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container
float M_PI=3.14f;

char str_main[50];

int main(void) {
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	
	delay_init();
	printf("Debug Init\n");
	
	IIC_Init();
	Init_USART1();
	
	MPU6050(0xD0);
	MPUinitialize();
	
	if( MPUtestConnection()== SUCCESS) {
		STM_EVAL_LEDOn(LED6);
	}
	
	MPUdmpInitialize();
  MPUsetDMPEnabled(true);
	
	mpuIntStatus = MPUgetIntStatus();
  packetSize = MPUdmpGetFIFOPacketSize();
	
	while (1) {
		
		mpuIntStatus = MPUgetIntStatus();
    fifoCount = MPUgetFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        MPUresetFIFO();
        printf("FIFO overflow!");
    } 
		else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = MPUgetFIFOCount();
        MPUgetFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
			
        MPUdmpGetQuaternion(&q, fifoBuffer);
        MPUdmpGetEuler(euler, &q);
			
				sprintf(str_main,"#:%.2f:%.2f:%.2f\n", euler[0]* 180/M_PI, euler[2]* 180/M_PI, euler[1]* 180/M_PI);
				USART_puts(USART1, str_main);
				//printf("Yaw: %.2f  Pitch: %.2f Roll: %.2f\n", euler[0]* 180/M_PI, euler[2]* 180/M_PI, euler[1]* 180/M_PI);
		}
		
		STM_EVAL_LEDToggle(LED3);
	}
}
