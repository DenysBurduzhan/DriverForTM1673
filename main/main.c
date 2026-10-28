#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"


#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_23
#define PIN_NUM_CLK  GPIO_NUM_18
#define PIN_NUM_CS   GPIO_NUM_5
#define SPI_HOST   SPI3_HOST

void spi_init(void);
void spi_write(uint8_t data, size_t len);

esp_err_t err;
spi_device_handle_t spi;

void app_main(void)
{

}

void spi_init(void){
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .max_transfer_sz = 4096
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
    };
    err = spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_DISABLED);
    if (err != ESP_OK) {
        ESP_LOGE("SPI", "Failed to initialize SPI bus");
        return;
    }
    err = spi_bus_add_device(SPI_HOST, &devcfg, &spi);
    if (err != ESP_OK) {
        ESP_LOGE("SPI", "Failed to add SPI device");
        return;
    }
}

void spi_write(uint8_t addr, uint8_t data){
    spi_transaction_t t = {
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = {addr, data},
        .length = 16
 };
 gpio_set_level(PIN_NUM_CS, 0);
 printf("Writing '0x%x' data at 0x%x\n", data, addr);
 err = spi_device_transmit(spi, &t);
 if (err != ESP_OK) {
     ESP_LOGE("SPI", "Failed to transmit SPI data");
 }
 gpio_set_level(PIN_NUM_CS, 1);
 vTaskDelay(100 / portTICK_PERIOD_MS);
 printf("Writing operation completed\n\n");
}



