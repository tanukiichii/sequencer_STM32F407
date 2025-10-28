#include "tm1638.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define STB_GPIO_Port GPIOC
#define STB_Pin GPIO_PIN_14
#define CLK_GPIO_Port GPIOC
#define CLK_Pin GPIO_PIN_13
#define DIO_GPIO_Port GPIOC
#define DIO_Pin GPIO_PIN_15

#define STB_HIGH() HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_SET)
#define STB_LOW()  HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET)
#define CLK_HIGH() HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET)
#define CLK_LOW()  HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET)
#define DIO_HIGH() HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_SET)
#define DIO_LOW()  HAL_GPIO_WritePin(DIO_GPIO_Port, DIO_Pin, GPIO_PIN_RESET)

static void tm_delay(void)
{
    volatile int i;
    for (i = 0; i < 40; ++i) __asm volatile ("nop");
}

void DIO_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DIO_GPIO_Port, &GPIO_InitStruct);
}

void DIO_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DIO_GPIO_Port, &GPIO_InitStruct);
}

void TM1638_SendByte(uint8_t data)
{
    DIO_SetOutput();
    for (uint8_t i = 0; i < 8; ++i) {
        CLK_LOW();
        tm_delay();
        if (data & 0x01) DIO_HIGH();
        else DIO_LOW();
        tm_delay();
        CLK_HIGH();
        tm_delay();
        data >>= 1;
    }
    CLK_HIGH();
    tm_delay();
}


uint8_t TM1638_ReadByte(void)
{
    uint8_t value = 0;
    DIO_SetInput();
    for (uint8_t i = 0; i < 8; ++i) {
        CLK_LOW();
        tm_delay();
        CLK_HIGH();
        tm_delay();
        if (HAL_GPIO_ReadPin(DIO_GPIO_Port, DIO_Pin)) {
            value |= (1 << i);
        }
        tm_delay();
    }

    CLK_HIGH();
    tm_delay();
    DIO_SetOutput();
    return value;
}


void TM1638_Command(uint8_t cmd)
{
    STB_LOW();
    TM1638_SendByte(cmd);
    STB_HIGH();
}

void TM1638_Init(void)
{
    STB_HIGH();
    CLK_HIGH();

    TM1638_Command(0x40);   // ?????????????
    STB_LOW();
    TM1638_SendByte(0xC0);  // ?????? ? ?????? 0

    // ????????? ?????????? ??? ?????
    for (int i = 0; i < 16; i++) {
        TM1638_SendByte(0xFF); // ??? ???????? ????????
    }
    STB_HIGH();

    TM1638_Command(0x8F);   // ???????? ???????, ??????? ????
}
uint16_t TM1638_ReadKeys(void)
{
    uint8_t keyData[4] = {0};
    uint16_t result = 0;

    STB_LOW();
    TM1638_SendByte(0x42);  // ??????? ?????? ??????
    DIO_SetInput();

    for (uint8_t i = 0; i < 4; i++) {
        keyData[i] = TM1638_ReadByte();
    }

    DIO_SetOutput();
    STB_HIGH();

    // ?????????? ????????? ?? ?????
    result |= (keyData[0] & 0x04) ? (1 << 0) : 0;  // S1
    result |= (keyData[0] & 0x40) ? (1 << 1) : 0;  // S2  
    result |= (keyData[1] & 0x04) ? (1 << 2) : 0;  // S3
    result |= (keyData[1] & 0x40) ? (1 << 3) : 0;  // S4
    result |= (keyData[2] & 0x04) ? (1 << 4) : 0;  // S5
    result |= (keyData[2] & 0x40) ? (1 << 5) : 0;  // S6
    result |= (keyData[3] & 0x04) ? (1 << 6) : 0;  // S7
    result |= (keyData[3] & 0x40) ? (1 << 7) : 0;  // S8  

    result |= (keyData[0] & 0x02) ? (1 << 8) : 0;  // S9
    result |= (keyData[0] & 0x20) ? (1 << 9) : 0;  // S10
    result |= (keyData[1] & 0x02) ? (1 << 10) : 0; // S11
    result |= (keyData[1] & 0x20) ? (1 << 11) : 0; // S12
    result |= (keyData[2] & 0x02) ? (1 << 12) : 0; // S13
    result |= (keyData[2] & 0x20) ? (1 << 13) : 0; // S14
    result |= (keyData[3] & 0x02) ? (1 << 14) : 0; // S15
    result |= (keyData[3] & 0x20) ? (1 << 15) : 0; // S16

    return result;
}

uint8_t TM1638_TestResponse(void)
{
    uint8_t val = 0;

    STB_LOW();
    TM1638_SendByte(0x42); // ??????? ??????
    DIO_SetInput();
    for (int i = 0; i < 8; i++)
    {
        CLK_LOW();
        HAL_Delay(1);
        CLK_HIGH();
        HAL_Delay(1);
        if (HAL_GPIO_ReadPin(DIO_GPIO_Port, DIO_Pin))
            val |= (1 << i);
    }
    DIO_SetOutput();
    STB_HIGH();

    return val;
}