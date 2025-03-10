#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>
#include "esp_log.h"


#define SENSOR_TYPE DHT_TYPE_AM2301     /* 1 === DHT22 === AM2301 */
#define DATA_GPIO 25
#define FILTER_SIZE 5

static const char *TAG = "DHT_SENSOR";

void dht_read_task(void *pvParameters);

void app_main() {
  if (
    xTaskCreate(
      dht_read_task,   /* Ponteiro para a função que a tarefa executará */
      "dht_read_task", /* Nome identificador da tarefa */
      configMINIMAL_STACK_SIZE * 3,   /* Tamanho da stack (memória alocada para variaveis locais e chamadas de função) */
      NULL,       /* Ponteiro para dados que serão passados para a tarefa */
      5,          /* Prioridade da tareda (0 = mais baixa, 24 = mais alta no ESP-IDF) */
      NULL        /* Handle (identificador) para gerenciar a tarefa posteriormente */
    ) != pdPASS
  ) {
    ESP_LOGE(TAG, "Falha ao criar tarefa!");
    return;
  }
}

void dht_read_task(void *pvParameters) {
  float temp_buffer[FILTER_SIZE] = {0};
  uint8_t idx = 0;
  gpio_set_pull_mode(DATA_GPIO, GPIO_PULLUP_ONLY); // Pull-up interno

  while (1) {
      float temperature, humidity;
      esp_err_t err = dht_read_float_data(SENSOR_TYPE, DATA_GPIO, &humidity, &temperature);

      if (err == ESP_OK) {
          // Filtro de média móvel
          temp_buffer[idx] = temperature;
          idx = (idx + 1) % FILTER_SIZE;
          float avg_temp = 0;
          for (int i = 0; i < FILTER_SIZE; i++) avg_temp += temp_buffer[i];
          avg_temp /= FILTER_SIZE;

          ESP_LOGI(TAG, "Umidade: %.1f%%, Temp: %.1f°C (Média: %.1f°C)", humidity, temperature, avg_temp);
      } else {
          ESP_LOGE(TAG, "Erro: %s (0x%X)", esp_err_to_name(err), err);
      }

      vTaskDelay(pdMS_TO_TICKS(2000));
  }
}