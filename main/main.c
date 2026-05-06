#include "freertos/idf_additions.h"
#include "sh1106_setup.h"
#include "keypad.h"
#include "shell.h"



void app_main(void) {
	u8g2_init();
	keypad_init();


	while (1) {
		get_keypad_input(&input);
		handle_input();
		init_data(); // function in my data.h - feel free to omit if you do not need it

		u8g2_ClearBuffer(&u8g2);
		draw_screen(buffer, BUFFER_SIZE);
		u8g2_SendBuffer(&u8g2);	
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
