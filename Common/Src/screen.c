#include <stdbool.h>
#include "screen.h"
#include "screen_bitmaps.h"

// ##################
// EXTERN VARIABLES #
// ##################
screen_pixel_pos_st g_cursor_pos = {.x = SCREEN_DRAW_AREA_CENTER_X, .y = SCREEN_DRAW_AREA_CENTER_Y};
uint8_t g_cursor_size = 8;

// #######################
// FILE GLOBAL VARIABLES #
// #######################
bool is_cursor_drawing = true;
screen_pixel_pos_st prev_cursor_pos = {.x = SCREEN_DRAW_AREA_CENTER_X, .y = SCREEN_DRAW_AREA_CENTER_Y};

int8_t selected_color_idx = 0;
uint32_t screen_colors[SCREEN_NR_OF_CURSOR_COLORS + 1] =
	{
		SCREEN_CURSOR_COLOR_0,
		SCREEN_CURSOR_COLOR_1,
		SCREEN_CURSOR_COLOR_2,
		SCREEN_CURSOR_COLOR_3,
		SCREEN_CURSOR_COLOR_4,
		SCREEN_CURSOR_COLOR_5,
		SCREEN_CURSOR_COLOR_6,
		SCREEN_CURSOR_COLOR_7,
		SCREEN_CURSOR_COLOR_8,
		SCREEN_CURSOR_COLOR_9,
		SCREEN_CURSOR_COLOR_10,
		SCREEN_CURSOR_COLOR_11,
		SCREEN_CURSOR_COLOR_12,
		SCREEN_CURSOR_COLOR_13,
		LCD_COLOR_WHITE};

screen_selected_button_e selected_button = Button_Pensize;
screen_selected_button_state_e selected_button_state = Button_Active;

const uint8_t *button_bitmaps[SCREEN_NR_OF_BUTTONS][SCREEN_NR_OF_BUTTON_STATES] =
	{
		{bmp_buttonPensizePressed, bmp_buttonPensizeNotPressed, bmp_buttonPensizeSelected},
		{bmp_buttonPalettePressed, bmp_buttonPaletteNotPressed, bmp_buttonPaletteSelected},
		{bmp_buttonEraserPressed, bmp_buttonEraserNotPressed, bmp_buttonEraserSelected},
		{bmp_buttonClearPressed, bmp_buttonClearNotPressed, bmp_buttonClearSelected}};

// ############################
// PRIVATE FUNCTIONS - CURSOR #
// ############################

static void priv_incCursorSize()
{
	if (g_cursor_size != SCREEN_CURSOR_MAX_SIZE)
	{
		g_cursor_size++;
	}
}

static void priv_decCursorSize()
{
	if (g_cursor_size != SCREEN_CURSOR_MIN_SIZE)
	{
		g_cursor_size--;
	}
}

static void priv_moveCursorAndDraw()
{
	BSP_LCD_DrawRect(g_cursor_pos.x, g_cursor_pos.y, g_cursor_size, g_cursor_size);
	if (screen_colors[selected_color_idx] == LCD_COLOR_WHITE)
	{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	}
	else
	{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	}
	BSP_LCD_DrawRect(g_cursor_pos.x + 1, g_cursor_pos.y + 1, g_cursor_size - 2, g_cursor_size - 2);
	BSP_LCD_SetTextColor(screen_colors[selected_color_idx]);
}

static void priv_moveCursorWithoutDraw()
{
	static uint32_t colors_around_pixel[SCREEN_CURSOR_BUFF_SIZE] = {LCD_COLOR_WHITE};

	// Fill previous cursor position with initial colors
	uint8_t i = 0;
	for (uint8_t row = prev_cursor_pos.x - 1; row <= prev_cursor_pos.x + 1; row++)
	{
		for (uint8_t col = prev_cursor_pos.y - 1; col <= prev_cursor_pos.y + 1; col++)
		{
			BSP_LCD_DrawPixel(row, col, colors_around_pixel[i]);
			i++;
		}
	}

	// Save into buffer current position's initial colors
	i = 0;
	for (uint8_t row = g_cursor_pos.x - 1; row <= g_cursor_pos.x + 1; row++)
	{
		for (uint8_t col = g_cursor_pos.y - 1; col <= g_cursor_pos.y + 1; col++)
		{
			colors_around_pixel[i] = BSP_LCD_ReadPixel(row, col);
			i++;
		}
	}

	// Draw cursor above initial colors (after they will be reset back)
	uint32_t cursor_color = BSP_LCD_ReadPixel(g_cursor_pos.x, g_cursor_pos.y) ^ 0x00FFFFFF;
	BSP_LCD_DrawPixel(g_cursor_pos.x, g_cursor_pos.y, cursor_color);
}

static void priv_updateCursor()
{
	if (is_cursor_drawing)
	{
		priv_moveCursorAndDraw();
	}
	else
	{
		priv_moveCursorWithoutDraw();
	}
}

// ####################################
// PRIVATE FUNCTIONS - COLOR SELECTOR #
// ####################################

static void priv_incSelectedColor()
{
	if (selected_color_idx != SCREEN_NR_OF_CURSOR_COLORS)
	{
		selected_color_idx++;
	}
}

static void priv_decSelectedColor()
{
	if (selected_color_idx != 0)
	{
		selected_color_idx--;
	}
}

static void priv_updateColorSelector()
{
	BSP_LCD_SetTextColor(SCREEN_BACKGROUND_COLOR);
	BSP_LCD_FillRect(0, SCREEN_COLOR_SELECTOR_POS_Y, SCREEN_WIDTH, SCREEN_COLOR_SELECTOR_HEIGHT);

	BSP_LCD_SetTextColor(SCREEN_SELECTOR_COLOR);
	uint16_t pos_x = selected_color_idx * SCREEN_COLOR_SELECTOR_DIST_X + SCREEN_COLOR_SELECTOR_START_X;
	BSP_LCD_FillRect(pos_x, SCREEN_COLOR_SELECTOR_POS_Y, SCREEN_COLOR_SELECTOR_WIDTH, SCREEN_COLOR_SELECTOR_HEIGHT);
}

// #############################
// PRIVATE FUNCTIONS - BUTTONS #
// #############################

static void priv_updateButtons()
{
	for (uint8_t i = 0; i < SCREEN_NR_OF_BUTTONS; i++)
	{
		uint16_t pos_x = i * SCREEN_BUTTON_DIST_X + SCREEN_BUTTON_START_X;
		if (i == selected_button)
		{
			BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, button_bitmaps[i][selected_button_state]);
		}
		else
		{
			BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, button_bitmaps[i][Button_Not_Pressed]);
		}
	}
}

static void priv_selectEraser(bool flag)
{
	static int8_t last_index;

	if (flag)
	{
		last_index = selected_color_idx;
		selected_color_idx = SCREEN_NR_OF_CURSOR_COLORS;
	}
	else
	{
		selected_color_idx = last_index;
	}
}

// ############################
// PRIVATE FUNCTIONS - SCREEN #
// ############################

static void priv_clearScreen()
{
	uint8_t x1 = SCREEN_DRAW_AREA_START_X;
	uint8_t y1 = SCREEN_DRAW_AREA_START_Y;
	uint8_t x2 = SCREEN_DRAW_AREA_STOP_X - SCREEN_DRAW_AREA_START_X;
	uint8_t y2 = SCREEN_DRAW_AREA_STOP_Y - SCREEN_DRAW_AREA_START_Y;

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(x1, y1, x2, y2);
	BSP_LCD_SetTextColor(screen_colors[selected_color_idx]);

	priv_updateCursor();
}

// ##################
// PUBLIC FUNCTIONS #
// ##################

void screen_initBackground()
{
	BSP_LCD_DrawBitmap(0, 0, (uint8_t *)bmp_background);

	uint16_t pos_x;
	for (uint8_t i = 0; i < SCREEN_NR_OF_CURSOR_COLORS; i++)
	{
		pos_x = i * SCREEN_CURSOR_COLOR_DIST_X + SCREEN_CURSOR_COLOR_START_X;
		BSP_LCD_SetTextColor(screen_colors[i]);
		BSP_LCD_FillRect(pos_x, SCREEN_CURSOR_COLOR_POS_Y, SCREEN_CURSOR_COLOR_SQUARE_WIDTH, SCREEN_CURSOR_COLOR_SQUARE_WIDTH);
	}

	priv_updateButtons();
	priv_updateColorSelector();
	priv_updateCursor();
	//	priv_clearScreen();
}

void screen_buttonRightPressed()
{
	if (selected_button_state == Button_Active)
	{
		switch (selected_button)
		{
		case (Button_Pensize):
			priv_incCursorSize();
			break;

		case (Button_Palette):
			priv_incSelectedColor();
			priv_updateColorSelector();
			break;

		default:
			break;
		}
	}
	else
	{
		if (selected_button != Button_Clear)
		{
			selected_button++;
		}
		priv_updateButtons();
	}
	priv_updateCursor();
}

void screen_buttonLeftPressed()
{
	if (selected_button_state == Button_Active)
	{
		switch (selected_button)
		{
		case (Button_Pensize):
			priv_decCursorSize();
			priv_updateCursor();
			break;

		case (Button_Palette):
			priv_decSelectedColor();
			priv_updateColorSelector();
			priv_updateCursor();
			break;

		default:
			break;
		}
	}
	else
	{
		if (selected_button != Button_Pensize)
		{
			selected_button--;
		}
		priv_updateButtons();
	}
	priv_updateCursor();
}

void screen_buttonOkPressed()
{
	if (selected_button_state == Button_Active)
	{
		selected_button_state = Button_Pressed;
	}
	else
	{
		selected_button_state = Button_Active;
	}

	if (selected_button == Button_Erase)
	{
		if (selected_button_state == Button_Active)
		{
			priv_selectEraser(true);
		}
		else
		{
			priv_selectEraser(false);
		}
	}
	else if (selected_button == Button_Clear)
	{
		priv_clearScreen();
		selected_button_state = Button_Pressed;
	}

	priv_updateButtons();
	priv_updateCursor();
}

void screen_buttonJoyPressed()
{
	is_cursor_drawing = !is_cursor_drawing;

	if (!is_cursor_drawing)
	{
		BSP_LCD_FillRect(g_cursor_pos.x, g_cursor_pos.y, g_cursor_size, g_cursor_size);
	}

	priv_updateCursor();
}

void screen_cursorMoved(screen_cursor_direction_e direction)
{
	prev_cursor_pos.x = g_cursor_pos.x;
	prev_cursor_pos.y = g_cursor_pos.y;

	if (direction == Cursor_Up)
	{
		g_cursor_pos.y++;
	}
	else if (direction == Cursor_Down)
	{
		g_cursor_pos.y--;
	}
	else if (direction == Cursor_Right)
	{
		g_cursor_pos.x++;
	}
	else if (direction == Cursor_Left)
	{
		g_cursor_pos.x--;
	}

	priv_updateCursor();
}
