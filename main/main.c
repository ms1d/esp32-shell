#include "freertos/idf_additions.h"
#include "oled_setup.h"
#include "keypad.h"
#include "shell.h"



void app_main(void) {
	u8g2_init();
	keypad_init();


	while (1) {
		get_keypad_input(&input);
		handle_input();

		u8g2_ClearBuffer(&u8g2);
		for (int i = 0; i < BUFFER_SIZE; i+= LINE_WIDTH) {
			char line_buffer[LINE_WIDTH+1];

			for (int j = 0; j < LINE_WIDTH; j++) {
				line_buffer[j] = buffer[i+j];
			}

			u8g2_DrawStr(&u8g2, 0, TOP_LINE_Y + i / LINE_WIDTH * LINE_HEIGHT, line_buffer);
		}

		u8g2_SendBuffer(&u8g2);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
