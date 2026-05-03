#pragma once



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



void get_keypad_input(char *input) {
	for (int i = 0; i < 3; i++) {
		gpio_set_direction(cols[i], GPIO_MODE_OUTPUT);
		gpio_set_level(cols[i], 0);

		for (int j = 0; j < 4; j++) {
			gpio_set_direction(rows[j], GPIO_MODE_INPUT);
			gpio_set_pull_mode(rows[j], GPIO_PULLUP_ONLY);

			if (gpio_get_level(rows[j]) == 0) {
				*input = keymap[j][i];
				printf("connection\n");
				return;
			}
		}

		gpio_set_level(cols[i], 1); // prevent ghosting
    }

	*input = '\0';
}
