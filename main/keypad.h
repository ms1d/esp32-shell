#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "pins.h"



#define ONE_KEY '1'
#define TWO_KEY '2'
#define THREE_KEY '3'
#define FOUR_KEY '4'
#define FIVE_KEY '5'
#define SIX_KEY '6'
#define SEVEN_KEY '7'
#define EIGHT_KEY '8'
#define NINE_KEY '9'
#define ZERO_KEY '0'
#define BACKSPACE_KEY '*'
#define ENTER_KEY '#'
#define PGUP_KEY 'U'
#define LNUP_KEY 'u'
#define PGDN_KEY 'D'
#define LNDN_KEY 'd'

char keymap[4][4] = {
	{ ONE_KEY, TWO_KEY, THREE_KEY, PGUP_KEY },
	{ FOUR_KEY, FIVE_KEY, SIX_KEY, LNUP_KEY },
	{ SEVEN_KEY, EIGHT_KEY, NINE_KEY, LNDN_KEY },
	{ BACKSPACE_KEY, ZERO_KEY, ENTER_KEY, PGDN_KEY }
};



int rows[4] = { R1, R2, R3, R4 };
int cols[4] = { C1, C2, C3, C4 };



void keypad_init() {
	for (int i = 0; i < 4; i++) {
		gpio_set_direction(cols[i], GPIO_MODE_OUTPUT);
		gpio_set_level(cols[i], 1);
	}

	for (int i = 0; i < 4; i++) {
		gpio_set_direction(rows[i], GPIO_MODE_INPUT);
		gpio_set_pull_mode(rows[i], GPIO_PULLUP_ONLY);
	}
}



void get_keypad_input(char *input) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gpio_set_level(cols[j], 1);
		}

		gpio_set_level(cols[i], 0);

		for (int j = 0; j < 4; j++) {
			if (gpio_get_level(rows[j]) == 0) {
				*input = keymap[j][i];
				return;
			}
		}

		gpio_set_level(cols[i], 1); // prevent ghosting
    }

	*input = '\0';
}
