# STM32F429_LCD_Paint

## Demo



## Generic overview

This is FreeRTOS based paint application inspired by Windows 98 Paint, created within University of Tartu Embedded Systems course (LOTI.05.085) <br/>
Mainboard is STM32F429I-DISC1 modified with extension PCB, that contains: <br/>
1. Two additional GPIO buttons
2. Joystick potentiometer with ADC + SPI
3. 3x mikroBUS socket
4. FTDI UART – USB

<details><summary>Photo of board</summary>
Credits for this extension goes to University of Tartu

<img src="/.images/devboard.png" width=35% height="auto"/></br>
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

Color selector active           |  Color selector just scrolled
:-------------------------:|:-------------------------:
<img src="/.images/gui_btn_active.png" width="auto" height="auto"/></br>  |  <img src="/.images/gui_btn_selected.png" width="auto" height="auto"/></br>



<br/>

## Code flow

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
