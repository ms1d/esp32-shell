#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include "u8g2.h"
#include "u8x8.h"



u8g2_t u8g2;



// SH1106 SPI pin numbers
#define OLED_RST 17
#define OLED_DC  16



uint8_t u8x8_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	switch (msg) {
		case U8X8_MSG_DELAY_MILLI:
			esp_rom_delay_us(arg_int * 1000);
			break;

		case U8X8_MSG_GPIO_RESET:
			gpio_set_level(OLED_RST, arg_int);
			break;

		case U8X8_MSG_GPIO_DC:
			gpio_set_level(OLED_DC, arg_int);
			break;

		default:
			break;
	}

    return 1;
}



void u8g2_init(void) {
	// Init GPIO pins
	gpio_set_direction(OLED_RST, GPIO_MODE_OUTPUT);
	gpio_set_direction(OLED_DC, GPIO_MODE_OUTPUT);

	u8g2_Setup_sh1106_128x64_noname_f(
		&u8g2,
		U8G2_R0,					// init rot to 0
		u8x8_byte_4wire_sw_spi,		// software SPI is good enough for this project
		u8x8_gpio_and_delay_cb		// callback to control hardware outside of spi interations
	);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);    // 0 = power save off = display on
}



void app_main(void) {
	u8g2_init();

	while (1) {
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 10, 20, "wassup");
		u8g2_SendBuffer(&u8g2);
	}
}
