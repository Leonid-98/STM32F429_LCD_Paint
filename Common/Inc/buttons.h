#include "main.h"

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#define BUTTONS_JOY_PORT GPIOC
#define BUTTONS_JOY_PIN GPIO_PIN_11
#define BUTTONS_BLUE_PORT GPIOG
#define BUTTONS_BLUE_PIN GPIO_PIN_2
#define BUTTONS_YELLOW_PORT GPIOG
#define BUTTONS_YELLOW_PIN GPIO_PIN_3
#define BUTTONS_USER_PORT GPIOA
#define BUTTONS_USER_PIN GPIO_PIN_0

typedef enum {
	Button_Up, Button_Rising, Button_Falling, Button_Down,
} button_state_e;

typedef enum {
	Active = 0, Not_Active,
} pin_state_e;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	pin_state_e curr_pin_state;
	pin_state_e prev_pin_state;
	button_state_e state;
} button_st;

void buttons_updateGivenButton(button_st *button);
void buttons_updateAll();
button_state_e buttons_joyGetState();
button_state_e buttons_blueGetState();
button_state_e buttons_yellowGetState();
button_state_e buttons_userGetState();

#endif /* INC_BUTTONS_H_ */
