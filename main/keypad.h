#pragma once



#include "driver/gpio.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "pins.h"



// For now - if A button is pressed, return 1, else -1
void test(char *output) {
	int pins[7] = { P0, P1, P2, P3, P4, P5, P6 };

	for (int i = 0; i < 7; i++) {
		gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
		gpio_set_level(pins[i], 0);

		for (int j = 0; j < 7; j++) {
			if (i == j) continue;
			gpio_set_direction(pins[j], GPIO_MODE_INPUT);
			gpio_set_pull_mode(pins[j], GPIO_PULLUP_ONLY);
		}

		vTaskDelay(pdMS_TO_TICKS(5));

		for (int j = 0; j < 7; j++) if (j != i) {
			if (gpio_get_level(pins[j]) == 0) { *output = 1; return; }
		}
    }

	*output = -1;
}
