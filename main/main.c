// FUNCTION DEFINITION
void app_main(void);
void app_loop(void);
void restart_seconds(int seconds);

#include <stdio.h>
#include <esp_system.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
  printf("Equipamento ligado!\n");
  app_loop();
  restart_seconds(5);
}

void app_loop(void) {
  printf("Início do Loop!\n");
  int loop = 0;
  while(loop < 100) {
    printf("Loop %d\n", loop);
    fflush(stdout);
    loop++;
    vTaskDelay(pdMS_TO_TICKS(10));  // Pausa de 10ms
  }
  printf("Fim do Loop!\n");
}

void restart_seconds(int seconds) {
  for(int i = seconds; i > 0; i--) {
    printf("\rReiniciando o ESP32 em %d segundos...", i);
    fflush(stdout);
    vTaskDelay(pdMS_TO_TICKS(1000));  // Pausa de 2 segundos
  }
  esp_restart();
}