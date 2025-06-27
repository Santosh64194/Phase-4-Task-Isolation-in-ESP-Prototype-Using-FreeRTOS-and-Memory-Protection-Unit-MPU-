/*
 * task1.c
 *
 *  Created on: Jun 17, 2025
 *      Author: uma_r
 */


#include "main.h"
#include "task1.h"
#include "FreeRTOS.h"
#include"FreeRTOSConfig.h"
#include "task.h"


extern I2C_HandleTypeDef hi2c1;

extern UART_HandleTypeDef huart2;


HAL_StatusTypeDef ret1;
uint8_t txbuffsuc[30]="Success!\n\r";
uint8_t txbufffail[30]="Fail!\n\r";

uint8_t txbuffsuc1[30]="Gyro config done!\n\r";
uint8_t txbuffsuc2[30]="Pwr config done!\n\r";
uint8_t txbuffread[30]="yaw read!\n\r";
uint8_t config=0x08;
uint8_t sleep=0x00;
uint8_t readbuffer[2];
uint16_t yaw_raw;

extern float yaw_dps[ SHARED_MEMORY_SIZE ] __attribute__( ( aligned( SHARED_MEMORY_SIZE ) ) );

float steerinput=50.0f;
uint16_t yaw[1];


void mpuint(void *params)
{

	ret1= HAL_I2C_IsDeviceReady(&hi2c1, 0b1101000<<1,1, 100);
	   if(ret1==HAL_OK)
	   {
		   printf("Success\r\n");
	   }
	   else
	   {
		   HAL_UART_Transmit(&huart2,txbufffail,30,10);
	   }

	   ret1= HAL_I2C_Mem_Write(&hi2c1, DEVADDRESS<<1, GYRO_CONFIG, 1, &config, 2, 100);

	   	   if(ret1==HAL_OK)
	   	   {
	   		   HAL_UART_Transmit(&huart2,txbuffsuc1,30,10);
	   	   }
	   	   else
	   	   {
	   		   HAL_UART_Transmit(&huart2,txbufffail,30,10);
	   	   }
	   	ret1= HAL_I2C_Mem_Write(&hi2c1, DEVADDRESS<<1, PWR_MANG1, 1, &sleep, 2, 100);
	   	   if(ret1==HAL_OK)
	   	   {
	   		   HAL_UART_Transmit(&huart2,txbuffsuc2,30,10);
	   		HAL_Delay(1000);
	   	   }
	   	   else
	   	   {
	   		   HAL_UART_Transmit(&huart2,txbufffail,30,10);
	   	   }

}

void mpuread(void *params)
{
	ret1=  HAL_I2C_Mem_Read(&hi2c1, DEVADDRESS<<1, GYROZ, 1, &readbuffer, 2, 100);
	if (ret1==HAL_OK)
	{
		yaw_raw =((uint16_t)readbuffer[0] << 8 )+ readbuffer[1];

		yaw_dps[0] = ((int16_t)yaw_raw) / 65.5f;

		printf("Yaw raw value: %d\r\n",yaw_raw);


		printf("Yaw value: %f\r\n",yaw_dps[0]);


	}

	else
	{
		HAL_UART_Transmit(&huart2,txbufffail,30,10);
	}

}

void controlunit (void)
{
	if(fabs(yaw_dps[0])<=steerinput)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
	}
}

void logger(void)
{
	float value2= yaw_dps[0];
//	yaw_dps[0]=0.0f;
}


