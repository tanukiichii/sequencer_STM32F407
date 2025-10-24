// tm1638.h
#ifndef TM1638_H
#define TM1638_H

#include "main.h"

void TM1638_Init(void);
void TM1638_DisplayTest(void);
void DIO_SetInput(void);
void DIO_SetOutput(void);
uint8_t TM1638_ReadByte(void);
uint16_t TM1638_ReadKeys(void);
void TM1638_Command(uint8_t cmd);
uint8_t TM1638_TestResponse(void);
void TM1638_SendByte(uint8_t data);

#define STB_GPIO_Port GPIOA
#define STB_Pin GPIO_PIN_5
#define STB_HIGH() HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_SET)
#define STB_LOW()  HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET)

#endif