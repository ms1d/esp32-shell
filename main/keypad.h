#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "pins.h"



char keymap[4][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};

int cols[3] = { C1, C2, C3 };
int rows[4] = { R1, R2, R3, R4 };



void keypad_init() {
	for (int i = 0; i < 3; i++) {
		gpio_set_direction(cols[i], GPIO_MODE_OUTPUT);
		gpio_set_level(cols[i], 1);
	}

	for (int i = 0; i < 4; i++) {
		gpio_set_direction(rows[i], GPIO_MODE_INPUT);
		gpio_set_pull_mode(rows[i], GPIO_PULLUP_ONLY);
	}
}



void get_keypad_input(char *input) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
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
