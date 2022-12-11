#include <buttons.h>

// #######################
// FILE GLOBAL VARIABLES #
// #######################

button_st button_joy = {
	.port = BUTTONS_JOY_PORT,
	.pin = BUTTONS_JOY_PIN,
	.curr_pin_state = Not_Active,
	.prev_pin_state = Not_Active,
	.state = Button_Falling};

button_st button_blue = {
	.port = BUTTONS_BLUE_PORT,
	.pin = BUTTONS_BLUE_PIN,
	.curr_pin_state = Not_Active,
	.prev_pin_state = Not_Active,
	.state = Button_Falling};

button_st button_yellow = {
	.port = BUTTONS_YELLOW_PORT,
	.pin = BUTTONS_YELLOW_PIN,
	.curr_pin_state = Not_Active,
	.prev_pin_state = Not_Active,
	.state = Button_Falling};

button_st button_user = {
	.port = BUTTONS_USER_PORT,
	.pin = BUTTONS_USER_PIN,
	.curr_pin_state = Not_Active,
	.prev_pin_state = Not_Active,
	.state = Button_Falling};

// ###################
// PRIVATE FUNCTIONS #
// ###################

static void priv_updateGivenButton(button_st *button)
{
	button->curr_pin_state = HAL_GPIO_ReadPin(button->port, button->pin);

	if ((button->curr_pin_state == Not_Active) && (button->prev_pin_state == Not_Active))
	{
		button->state = Button_Up;
	}
	else if ((button->curr_pin_state == Active) && (button->prev_pin_state == Not_Active))
	{
		button->state = Button_Falling;
	}
	else if ((button->curr_pin_state == Active) && (button->prev_pin_state == Active))
	{
		button->state = Button_Down;
	}
	else if ((button->curr_pin_state == Not_Active) && (button->prev_pin_state == Active))
	{
		button->state = Button_Rising;
	}

	button->prev_pin_state = button->curr_pin_state;
}

// ##################
// PUBLIC FUNCTIONS #
// ##################

void buttons_updateAll()
{
	priv_updateGivenButton(&button_joy);
	priv_updateGivenButton(&button_blue);
	priv_updateGivenButton(&button_yellow);
	priv_updateGivenButton(&button_user);
}

button_state_e buttons_joyGetState()
{
	return button_joy.state;
}

button_state_e buttons_blueGetState()
{
	return button_blue.state;
}

button_state_e buttons_yellowGetState()
{
	return button_yellow.state;
}

button_state_e buttons_userGetState()
{
	return button_user.state;
}
