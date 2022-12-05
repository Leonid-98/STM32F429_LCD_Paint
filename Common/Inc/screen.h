#ifndef INC_LCD_GAME_H_
#define INC_LCD_GAME_H_

#include "main.h"
#include "stm32f429i_discovery_lcd.h"

#define SCREEN_WIDTH BSP_LCD_GetXSize()
#define SCREEN_HEIGHT BSP_LCD_GetYSize()

#define SCREEN_DRAW_AREA_START_X 9
#define SCREEN_DRAW_AREA_STOP_X 230
#define SCREEN_DRAW_AREA_START_Y 9
#define SCREEN_DRAW_AREA_STOP_Y 240
#define SCREEN_DRAW_AREA_CENTER_X 120
#define SCREEN_DRAW_AREA_CENTER_Y 125

#define SCREEN_NR_OF_BUTTONS 4
#define SCREEN_BUTTON_START_X 13
#define SCREEN_BUTTON_DIST_X 56
#define SCREEN_BUTTON_POS_Y 269

typedef struct
{
	uint8_t x;
	uint8_t y;
} screen_pixel_pos_st;

extern screen_pixel_pos_st cursor_pos;
extern uint8_t cursor_size;

void screen_initBackground();
void screen_updateCursor();
void screen_updateButtons();
void screen_buttonRightPressed();
void screen_buttonLeftPressed();
void screen_buttonOkPressed();

#endif /* INC_LCD_GAME_H_ */
