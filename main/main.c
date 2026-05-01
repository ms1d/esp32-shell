#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "u8g2.h"



#define CLK   18
#define MOSI  17
#define CS    16
#define DC    15
#define RES   14

static spi_device_handle_t spi;

static void spi_init(void) {
    spi_bus_config_t buscfg = {
        .mosi_io_num = MOSI,
        .miso_io_num = -1,
        .sclk_io_num = CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = -1,   // u8g2 handles CS
        .queue_size = 1
    };

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
}

uint8_t u8x8_gpio_and_delay_esp32(u8x8_t *u8x8,
                                  uint8_t msg,
                                  uint8_t arg_int,
                                  void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            gpio_set_direction(DC, GPIO_MODE_OUTPUT);
            gpio_set_direction(CS, GPIO_MODE_OUTPUT);
            gpio_set_direction(RES, GPIO_MODE_OUTPUT);
            break;

        case U8X8_MSG_DELAY_MILLI:
            vTaskDelay(pdMS_TO_TICKS(arg_int));
            break;

        case U8X8_MSG_GPIO_RESET:
            gpio_set_level(RES, arg_int);
            break;

        default:
            break;
    }
    return 1;
}

uint8_t u8x8_byte_esp32_hw_spi(u8x8_t *u8x8,
                               uint8_t msg,
                               uint8_t arg_int,
                               void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_BYTE_SEND: {
            spi_transaction_t t = {0};
            t.length = arg_int * 8;
            t.tx_buffer = arg_ptr;
            spi_device_transmit(spi, &t);
        }
        break;

        case U8X8_MSG_BYTE_SET_DC:
            gpio_set_level(DC, arg_int);
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            gpio_set_level(CS, 0);
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            gpio_set_level(CS, 1);
            break;

        case U8X8_MSG_BYTE_INIT:
            break;

        default:
            return 0;
    }
    return 1;
}

u8g2_t u8g2;

void u8g2_init() {

    spi_init();
    
	u8g2_Setup_sh1106_128x64_noname_f(
        &u8g2,
        U8G2_R0,
        u8x8_byte_esp32_hw_spi,
        u8x8_gpio_and_delay_esp32
    );

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
}

void app_main(void) {
	u8g2_init();

	while (1) {
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawStr(&u8g2, 0, 6, "dsmith@agsb $ daddytimmins");
		u8g2_SendBuffer(&u8g2);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
