#include "freertos/idf_additions.h"
#include "oled_setup.h"
#include "keypad.h"

#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define BUFFER_SIZE 255
#define MIN_BUFFER_POS 14
#define PROMPT "dsmith@agsb $ "

void app_main(void) {
	u8g2_init();
	keypad_init();

	char input = '\0';
	char buffer[BUFFER_SIZE+1] = PROMPT;
	int buffer_pos = MIN_BUFFER_POS; // convention: points to the next free position

	while (1) {
		get_keypad_input(&input);

		if (input != '\0') {
			if (input == '*' && buffer_pos > MIN_BUFFER_POS) {
				buffer_pos--; buffer[buffer_pos] = '\0';
			}

			else if (input == '#') {
				while (buffer_pos > MIN_BUFFER_POS) {
					buffer_pos--;
					buffer[buffer_pos] = '\0';
				}
			}

			else if (input != '*') {
				buffer[buffer_pos] = input;
				buffer_pos++;
			}
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
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
