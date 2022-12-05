#include <screen.h>
#include <screen_bitmaps.h>

screen_pixel_pos_st cursor_pos = {.x = SCREEN_DRAW_AREA_CENTER_X, .y = SCREEN_DRAW_AREA_CENTER_Y};
uint8_t cursor_size = 15;

int8_t screen_selected_button_idx = 0;
uint8_t screen_is_button_selected = 1;

void screen_initBackground()
{
	BSP_LCD_DrawBitmap(0, 0, (uint8_t *)bmp_background);
	screen_updateButtons();
}

void screen_updateCursor()
{
	BSP_LCD_DrawRect(cursor_pos.x, cursor_pos.y, cursor_size, cursor_size);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DrawRect(cursor_pos.x + 1, cursor_pos.y + 1, cursor_size - 2, cursor_size - 2);
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}

void screen_updateButtons()
{
	uint16_t pos_x;
	for (uint8_t i = 0; i < SCREEN_NR_OF_BUTTONS; i++)
	{
		pos_x = i * SCREEN_BUTTON_DIST_X + SCREEN_BUTTON_START_X;
		if (i == screen_selected_button_idx)
		{
			if (screen_is_button_selected)
			{
				BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, (uint8_t *)bmp_buttonSelected);
			}
			else
			{
				BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, (uint8_t *)bmp_buttonPressed);
			}
		}
		else
		{
			BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, (uint8_t *)bmp_buttonNotPressed);
		}
	}
}

void screen_buttonRightPressed()
{
	if (screen_is_button_selected)
	{
		cursor_size++;
	}
	else
	{
		screen_selected_button_idx++;
		if (screen_selected_button_idx > 3)
		{
			screen_selected_button_idx = 0;
		}
	}
}

void screen_buttonLeftPressed()
{
	if (screen_is_button_selected)
	{
		cursor_size--;
	}
	else
	{
		screen_selected_button_idx--;
		if (screen_selected_button_idx < 0)
		{
			screen_selected_button_idx = 3;
		}
	}
}

void screen_buttonOkPressed()
{
	uint16_t pos_x = screen_selected_button_idx * SCREEN_BUTTON_DIST_X + SCREEN_BUTTON_START_X;
	BSP_LCD_DrawBitmap(pos_x, SCREEN_BUTTON_POS_Y, (uint8_t *)bmp_buttonNotPressed);
	HAL_Delay(100);
	screen_is_button_selected ^= 1;
}
