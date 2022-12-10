#include "stdlib.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "stream_buffer.h"

#include <joystick.h>
#include <screen.h>
#include <buttons.h>

osMutexId vComMutexHandleMain;
osThreadId defaultTaskHandle;

joy_pos_st joy_pos;

// Auto generated code
void StartDefaultTask(void const *argument);
extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);
__weak void vApplicationIdleHook(void) {
}
__weak void vApplicationStackOverflowHook(xTaskHandle xTask,
                                          signed char *pcTaskName)
{
}
__weak void vApplicationMallocFailedHook(void)
{
}
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
// Auto generated code end

void taskGetJoystickPos()
{
    int size;
    char joy_buff[30];
    for (;;)
    {
        joy_readXY(&joy_pos);

        size = snprintf(joy_buff, 30, "LF=%d, UD=%d\n", joy_pos.x, joy_pos.y);

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void taskUpdateXPos()
{
    uint8_t taskDelay;
    for (;;)
    {
        osMutexWait(vComMutexHandleMain, portMAX_DELAY);

        // Update postition (move object coord)
        if ((joy_pos.x > JOY_CENTER + JOY_CENTER_THRESH) & (g_cursor_pos.x < SCREEN_DRAW_AREA_STOP_X - g_cursor_size))
        {
            g_cursor_pos.x++;
            screen_updateCursor();
        }
        else if ((joy_pos.x < JOY_CENTER - JOY_CENTER_THRESH) & (g_cursor_pos.x > SCREEN_DRAW_AREA_START_X))
        {
            g_cursor_pos.x--;
            screen_updateCursor();
        }

        // Calculate delay (aka speed of the cursor)
        taskDelay = joy_getTaskDelayMs(g_cursor_pos.x);

        osMutexRelease(vComMutexHandleMain);
        vTaskDelay(pdMS_TO_TICKS(taskDelay));
    }
}

void taskUpdateYPos()
{
    uint8_t taskDelay;
    for (;;)
    {
        osMutexWait(vComMutexHandleMain, portMAX_DELAY);

        // Update postition (move object coord)
        if ((joy_pos.y > JOY_CENTER + JOY_CENTER_THRESH) & (g_cursor_pos.y > SCREEN_DRAW_AREA_START_Y))
        {

            g_cursor_pos.y--;
            screen_updateCursor();
        }
        else if ((joy_pos.y < JOY_CENTER - JOY_CENTER_THRESH) & (g_cursor_pos.y < SCREEN_DRAW_AREA_STOP_Y - g_cursor_size))
        {

            g_cursor_pos.y++;
            screen_updateCursor();
        }

        // Calculate delay (aka speed of the cursor)
        taskDelay = joy_getTaskDelayMs(g_cursor_pos.y);

        osMutexRelease(vComMutexHandleMain);
        vTaskDelay(pdMS_TO_TICKS(taskDelay));
    }
}

void MX_FREERTOS_Init(void)
{
    screen_initBackground();
    screen_updateCursor();

    osMutexDef(vComMutex);
    vComMutexHandleMain = osMutexCreate(osMutex(vComMutex));

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    xTaskCreate(taskGetJoystickPos, "Read JOY ADC XY", 1000, NULL, 1, NULL);
    xTaskCreate(taskUpdateXPos, "Update cursor X pos", 1000, NULL, 1, NULL);
    xTaskCreate(taskUpdateYPos, "Update cursor Y pos", 100, NULL, 3, NULL);
}

void StartDefaultTask(void const *argument)
{
    button_state_e state;

    uint8_t layer = 0;
    for (;;)
    {
        osDelay(10);
        buttons_updateAll();

        state = buttons_joyGetState();
        if (state == Button_Falling)
        {
        	screen_buttonJoyPressed();
        	screen_updateCursor();
        }

        state = buttons_blueGetState();
        if (state == Button_Falling)
        {
            screen_buttonLeftPressed();
            screen_updateCursor();
        }

        state = buttons_yellowGetState();
        if (state == Button_Falling)
        {
            screen_buttonRightPressed();
            screen_updateCursor();
        }

        state = buttons_userGetState();
        if (state == Button_Falling)
        {
            screen_buttonOkPressed();
            screen_updateCursor();
        }
    }
}
