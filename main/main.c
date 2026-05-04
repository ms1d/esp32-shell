#include "freertos/idf_additions.h"
#include "oled_setup.h"
#include "keypad.h"
#include "shell.h"

#define CURSOR "_"
#define DISPLAY_CHAR_WIDTH 5
#define SPACE_WIDTH 1
#define CURSOR_VERT_OFFSET 2


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

			// Draw cursor
			if (curr_mode == SHELL && i < buffer_pos && i + LINE_WIDTH > buffer_pos) {
				u8g2_DrawStr(&u8g2,
						(buffer_pos - i % LINE_WIDTH) * DISPLAY_CHAR_WIDTH + SPACE_WIDTH,
						TOP_LINE_Y + i / LINE_WIDTH * LINE_HEIGHT + CURSOR_VERT_OFFSET, CURSOR);
			}
		}

		u8g2_SendBuffer(&u8g2);
	
		//DEBUG
	
		printf("\nBUFFER: %s\n", buffer);
		printf("BUFFER_POS: %d\n", buffer_pos);
		printf("BUFFER[BUFFER_POS]: %c\n", buffer[buffer_pos]);
		printf("MODE = %d\n", curr_mode);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
