#include "freertos/idf_additions.h"
#include <stdio.h>
#include "oled_setup.h"
#include "keypad.h"

#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define BUFFER_SIZE 255

void app_main(void) {
	u8g2_init();

	char input, buffer[BUFFER_SIZE+1] = "dsmith@agsb $ ";
	int buffer_pos = 14;

	while (1) {
		test(&input);
		if (input == 1) {
			buffer[buffer_pos] = 'A';
			buffer_pos++;
		}
	
		u8g2_ClearBuffer(&u8g2);
		for (int i = 0; i < BUFFER_SIZE; i+= LINE_WIDTH) {
			char line_buffer[LINE_WIDTH+1];

			for (int j = 0; j < LINE_WIDTH; j++) {
				line_buffer[j] = buffer[i+j];
			}

			u8g2_DrawStr(&u8g2, 0, TOP_LINE_Y + i / LINE_WIDTH * LINE_HEIGHT, line_buffer);
		}

		u8g2_SendBuffer(&u8g2);
		vTaskDelay(pdMS_TO_TICKS(33));
	}
}
