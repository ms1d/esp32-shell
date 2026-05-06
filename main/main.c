#include "freertos/idf_additions.h"
#include "sh1106_setup.h"
#include "keypad.h"
#include "shell.h"



void app_main(void) {
	u8g2_init();
	keypad_init();
	init_data(); // function in my data.h - feel free to omit if you do not need it

	while (1) {
		get_keypad_input(&input);
		handle_input();
		draw_screen();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
