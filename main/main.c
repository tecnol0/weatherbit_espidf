#include <stdint.h>
#include <string.h>

#include "weather_logic.h"

#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_slave.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22
#define I2C_ADDRESS 0x42
#define PACKET_SIZE 14

static const char *TAG = "WEATHERBIT";
static QueueHandle_t rx_queue;

// Representa un paquete enviado por la micro:bit
typedef struct {
    uint8_t bytes[PACKET_SIZE];
} weather_packet_t;

// Callback: se ejecuta cuando llegan datos I2C
static bool receive_callback(
    i2c_slave_dev_handle_t handle,
    const i2c_slave_rx_done_event_data_t *event,
    void *user_data)
{
    (void)handle;

    if (event->length != PACKET_SIZE) {
        return false;
    }

    weather_packet_t packet;
    memcpy(packet.bytes, event->buffer, PACKET_SIZE);

    BaseType_t task_woken = pdFALSE;

    xQueueSendFromISR(
        (QueueHandle_t)user_data,
        &packet,
        &task_woken
    );

    return task_woken == pdTRUE;
}

void app_main(void)
{
    rx_queue = xQueueCreate(5, sizeof(weather_packet_t));
    ESP_ERROR_CHECK(rx_queue ? ESP_OK : ESP_ERR_NO_MEM);

    i2c_slave_config_t config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .slave_addr = I2C_ADDRESS,
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .send_buf_depth = 64,
        .receive_buf_depth = 64,
        .flags.enable_internal_pullup = true,
    };

    i2c_slave_dev_handle_t slave_handle;

    ESP_ERROR_CHECK(
        i2c_new_slave_device(&config, &slave_handle)
    );

    i2c_slave_event_callbacks_t callbacks = {
        .on_receive = receive_callback,
    };

    ESP_ERROR_CHECK(
        i2c_slave_register_event_callbacks(
            slave_handle, &callbacks, rx_queue
        )
    );

    ESP_LOGI(TAG, "Receptor I2C iniciado");
    ESP_LOGI(TAG, "Direccion: 0x42");
    ESP_LOGI(TAG, "Esperando paquetes de 14 bytes...");

    weather_packet_t packet;

    while (1) {
        if (xQueueReceive(
                rx_queue,
                &packet,
                pdMS_TO_TICKS(7000)) == pdTRUE) {

            // --- INICIO DEL BLOQUE REEMPLAZADO Y LIMPIO ---
            weather_data_t lectura;

            bool paquete_valido = weather_decode_packet(
                packet.bytes,
                sizeof(packet.bytes),
                &lectura
            );

            if (paquete_valido) {
                ESP_LOGI(TAG, "===== ESTACION METEOROLOGICA =====");
                ESP_LOGI(TAG, "Temperatura: %.2f C", lectura.temperatura);
                ESP_LOGI(TAG, "Humedad: %.2f %%", lectura.humedad);
                ESP_LOGI(TAG, "Presion: %.1f hPa", lectura.presion);
            } else {
                ESP_LOGW(TAG, "Paquete meteorologico invalido");
            }
            // --- FIN DEL BLOQUE REEMPLAZADO ---

        } else {
            ESP_LOGW(TAG, "Esperando micro:bit...");
        }
    }
}