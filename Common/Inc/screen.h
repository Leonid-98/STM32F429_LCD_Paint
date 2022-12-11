#ifndef INC_LCD_GAME_H_
#define INC_LCD_GAME_H_

#include "main.h"
#include "stm32f429i_discovery_lcd.h"

#define SCREEN_WIDTH 						BSP_LCD_GetXSize()
#define SCREEN_HEIGHT 						BSP_LCD_GetYSize()

// Area, where user can draw
#define SCREEN_DRAW_AREA_START_X			9
#define SCREEN_DRAW_AREA_STOP_X 			230
#define SCREEN_DRAW_AREA_START_Y			9
#define SCREEN_DRAW_AREA_STOP_Y 			240
#define SCREEN_DRAW_AREA_CENTER_X 			120
#define SCREEN_DRAW_AREA_CENTER_Y 			125

// Main buttons, that show what function to do (i.e draw, select color, resize cursor)
#define SCREEN_NR_OF_BUTTONS 				4
#define SCREEN_NR_OF_BUTTON_STATES			3  // Pressed, not pressed, active
#define SCREEN_BUTTON_START_X 				13
#define SCREEN_BUTTON_DIST_X 				56
#define SCREEN_BUTTON_POS_Y 				269

// Square, where color itself is filled
#define SCREEN_CURSOR_COLOR_START_X 		11
#define SCREEN_CURSOR_COLOR_DIST_X			16
#define SCREEN_CURSOR_COLOR_SQUARE_WIDTH 	13
#define SCREEN_CURSOR_COLOR_POS_Y			253
#define SCREEN_CURSOR_MAX_SIZE				30
#define SCREEN_CURSOR_MIN_SIZE				2
#define SCREEN_CURSOR_BUFF_SIZE				9

// Selector above square, that shows what color is selected
#define SCREEN_COLOR_SELECTOR_START_X 		SCREEN_CURSOR_COLOR_START_X
#define SCREEN_COLOR_SELECTOR_DIST_X		SCREEN_CURSOR_COLOR_DIST_X
#define SCREEN_COLOR_SELECTOR_WIDTH 		SCREEN_CURSOR_COLOR_SQUARE_WIDTH
#define SCREEN_COLOR_SELECTOR_HEIGHT		2
#define SCREEN_COLOR_SELECTOR_POS_Y			248

// Colors, that filled in the squares
#define SCREEN_NR_OF_CURSOR_COLORS 			14
#define SCREEN_CURSOR_COLOR_0				LCD_COLOR_BLACK
#define SCREEN_CURSOR_COLOR_1				LCD_COLOR_GRAY
#define SCREEN_CURSOR_COLOR_2				LCD_COLOR_RED
#define SCREEN_CURSOR_COLOR_3				LCD_COLOR_ORANGE
#define SCREEN_CURSOR_COLOR_4				LCD_COLOR_YELLOW
#define SCREEN_CURSOR_COLOR_5				LCD_COLOR_GREEN
#define SCREEN_CURSOR_COLOR_6				LCD_COLOR_CYAN
#define SCREEN_CURSOR_COLOR_7				LCD_COLOR_BLUE
#define SCREEN_CURSOR_COLOR_8				LCD_COLOR_MAGENTA
#define SCREEN_CURSOR_COLOR_9				0xFF808080
#define SCREEN_CURSOR_COLOR_10				0xFF008080
#define SCREEN_CURSOR_COLOR_11				0xFF000080
#define SCREEN_CURSOR_COLOR_12				0xFF800080
#define SCREEN_CURSOR_COLOR_13				0xFFFF80FF

// Main colors
#define SCREEN_SELECTOR_COLOR				0xFF716F64
#define SCREEN_BACKGROUND_COLOR				0xFFDED9BC

typedef enum
{
	Button_Pensize = 0,
	Button_Palette = 1,
	Button_Erase = 2,
	Button_Clear = 3
} screen_selected_button_e;

typedef enum
{
	Button_Pressed = 0,
	Button_Not_Pressed = 1,
	Button_Active = 2
} screen_selected_button_state_e;

typedef enum
{
	Cursor_Left,
	Cursor_Right,
	Cursor_Up,
	Cursor_Down
} screen_cursor_direction_e;

typedef struct
{
	uint8_t x;
	uint8_t y;
} screen_pixel_pos_st;

void screen_initBackground();
void screen_buttonRightPressed();
void screen_buttonLeftPressed();
void screen_buttonOkPressed();
void screen_buttonJoyPressed();
void screen_cursorMoved(screen_cursor_direction_e direction);

extern screen_pixel_pos_st g_cursor_pos;
extern uint8_t g_cursor_size;

#endif /* INC_LCD_GAME_H_ */
