# STM32F429_LCD_Paint

## Demo
[<img src="/.images/preview.png" width="25%">](https://youtube.com/shorts/aMHg6pMG_s8?feature=share)
</br>


## Generic overview

This is FreeRTOS based paint application inspired by Windows 98 Paint, created within University of Tartu Embedded Systems course (LOTI.05.085) <br/>
Mainboard is STM32F429I-DISC1 modified with extension PCB, that contains: <br/>
1. Two additional GPIO buttons
2. Joystick potentiometer with ADC + SPI
3. 3x mikroBUS socket
4. FTDI UART – USB

<details><summary>Photo of board</summary>
Credits for this extension goes to University of Tartu
<br/>
<img src="/.images/devboard.png" width=35% height="auto"/>
</br>
</details>

<br/>

## App overview
Cursor can be moved with joystick. Pressing joystick button toggles draw/move mode. In move mode, you can move cursor without drawing. <br/>
Blue and yellow button allows user to navigate between tabs:
1. Change cursor size
2. Change color
3. Select eraser
4. Clear drawing area

If change cursor/color active, yellow and blue buttons will change those parameters.

**Gui example**

Tab selector          |  Color selector | Size selector
:-------------------------:|:-------------------------:|:-------------------------:
<img src="/.images/tab_selector.gif" width="auto" height="auto"/></br>  |  <img src="/.images/color_selector.gif" width="auto" height="auto"/></br> | <img src="/.images/size_selector.gif" width="auto" height="auto"/></br> 



<br/>

## Code flow

There is 3 tasks in FreeRTOS:
1. Update X position (Update cursor position on the screen according to the joy XY position)
2. Update Y position
3. Read XY joystick position (Read ADC values, process them)

Fact, that X and Y cursor update are independent makes cursor movement smoother. <br/>
Even smoother cursor movement gives next feature: cursor speed depending on joystick position from center. The far joystick is - the faster is cursor. 'Speed' is defined by rate frequency of task, that can de dynamically changed by changing taskDelay:
```
void taskUpdateXPos()
{
    uint8_t taskDelay;
    for (;;)
    {
        ... update cursor pos on screen ...

        taskDelay = joy_getTaskDelayMs(g_cursor_pos.x);
        vTaskDelay(pdMS_TO_TICKS(taskDelay));
    }
}
```

<br/>

## Project structure

Big part of the code was generated by CUBE MX. Given structure represents part of the code, that were either created or modified in this project:

```
Common/
├─ Inc/
│  ├─ buttons.h
│  ├─ screen_bitmaps.h
│  ├─ screen.h
│  ├─ joystick.h
├─ Src/
│  ├─ butotns.c
│  ├─ joystick.c
│  ├─ screen.c
Core/
├─ Src/
│  ├─ freertos.c
│  ├─ main.c
```
