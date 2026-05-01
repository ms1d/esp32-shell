#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"



// SH1106 SPI pin numbers
#define CLK   12
#define MOSI  11
#define CS    10
#define DC    9
#define RES   8



void app_main(void) {
	while (1) {
		printf("alive!\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
