#include "freertos/idf_additions.h"
#include <stdio.h>
#include "oled_setup.h"
#include "keypad.h"

#define TOP_LINE_Y 6
#define LINE_HEIGHT 8

void app_main(void) {
	u8g2_init();

	while (1) {
		test();
	}

	while (1) {
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 0, TOP_LINE_Y, "dsmith@agsb $ daddytimmins");
		u8g2_DrawStr(&u8g2, 0, TOP_LINE_Y+LINE_HEIGHT, "dsmith@agsb $ daddytimmins");
		u8g2_SendBuffer(&u8g2);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
