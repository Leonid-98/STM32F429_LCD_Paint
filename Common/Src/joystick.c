#include <joystick.h>

// ##################
// EXTERN VARIABLES #
// ##################

joy_pos_st joy_pos;

// ###################
// PRIVATE FUNCTIONS #
// ###################

/**
 * Converts raw data from interval [a, b] to interval [c, d]
 * where a, b is raw ADC values and c, d is scaled values.
 * Using formula f(t) = ((d - c) / (b - a)) * (t - a) + c
 *
 * NB! X raw value is inverted, i.e joystick left position is ADC MAX output
 * and right position is ADC MIN output
 */
static void priv_convertRawData(int16_t *x, int16_t *y)
{
	float a;
	float b;
	float c;
	float d;

	// Convert x value
	if (*x <= JOY_X_CENTER_RAW)
	{
		a = JOY_X_MAX_RAW;
		b = JOY_X_CENTER_RAW;
		c = -100;
		d = 0;
	}
	else
	{
		a = JOY_X_CENTER_RAW;
		b = JOY_X_MIN_RAW;
		c = 0;
		d = 100;
	}
	*x = ((d - c) / (b - a)) * ((float)(*x) - a) + c;

	// Convert y value
	if (*y <= JOY_X_CENTER_RAW)
	{
		a = JOY_Y_MIN_RAW;
		b = JOY_Y_CENTER_RAW;
		c = JOY_MIN;
		d = JOY_CENTER;
	}
	else
	{
		a = JOY_Y_CENTER_RAW;
		b = JOY_Y_MAX_RAW;
		c = JOY_CENTER;
		d = JOY_MAX;
	}
	*y = ((d - c) / (b - a)) * ((float)(*y) - a) + c;
}

// ##################
// PUBLIC FUNCTIONS #
// ##################

/**
 * Gets joystick XY position by reading ADC values. ADC connected to the SPI4
 */
void joy_readXY(joy_pos_st *joy_pos)
{
	static uint8_t joy_tx_buff[1];
	static uint8_t joy_rx_buff[2];

	// Read joy ADC values
	joy_tx_buff[0] = JOY_SPI_SELECT_X;
	CS_START();
	HAL_SPI_TransmitReceive(&hspi4, joy_tx_buff, joy_rx_buff, 2, HAL_MAX_DELAY);
	CS_END();
	int16_t x = (joy_rx_buff[0] << 8) | joy_rx_buff[1];

	HAL_Delay(10);

	joy_tx_buff[0] = JOY_SPI_SELECT_Y;
	CS_START();
	HAL_SPI_TransmitReceive(&hspi4, joy_tx_buff, joy_rx_buff, 2, HAL_MAX_DELAY);
	CS_END();
	int16_t y = (joy_rx_buff[0] << 8) | joy_rx_buff[1];
	// Convert values to the intervals between JOY_MIN and JOY_MAX

	priv_convertRawData(&x, &y);
	joy_pos->x = x;
	joy_pos->y = y;
}

/**
 * Calculates freeRTOS task delay for joystick positon update function
 */
uint8_t joy_getTaskDelayMs(int8_t pos)
{
	// TODO get rid of hardcoded values
	uint8_t taskDelay;
	pos = abs(pos);

	if (pos > 90)
	{
		taskDelay = 4;
	}
	else if (pos > 80)
	{
		taskDelay = 6;
	}
	else if (pos > 70)
	{
		taskDelay = 8;
	}
	else if (pos > 60)
	{

		taskDelay = 10;
	}
	else if (pos > 50)
	{

		taskDelay = 12;
	}
	else if (pos > 40)
	{
		taskDelay = 14;
	}
	else
	{
		taskDelay = 16;
	}

	return taskDelay;
}
