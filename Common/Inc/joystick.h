#include "spi.h"
#include "main.h"
#include "stdlib.h"

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#define CS_START() HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin, 0)
#define CS_END() HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin, 1)

#define JOY_SPI_SELECT_X 0b00011000
#define JOY_SPI_SELECT_Y 0b00100000

#define JOY_X_MIN_RAW 420
#define JOY_X_CENTER_RAW 1970
#define JOY_X_MAX_RAW 3500
#define JOY_Y_MIN_RAW 440
#define JOY_Y_CENTER_RAW 2112
#define JOY_Y_MAX_RAW 3575

#define JOY_MIN -100
#define JOY_CENTER 0
#define JOY_MAX 100
#define JOY_CENTER_THRESH 15

#define JOY_UPDATE_MAX_DELAY_MS 16
#define JOY_UPDATE_MIN_DELAY_MS 4

typedef struct
{
	int16_t x;
	int16_t y;
} joy_pos_st;

void joy_readXY(joy_pos_st *joy_pos);
uint8_t joy_getTaskDelayMs(int8_t pos);

#endif /* INC_JOYSTICK_H_ */
