#include "app_tasks.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "app_config.h"
#include "app_types.h"
#include "hal_port.h"

static QueueHandle_t sample_queue;
static volatile uint32_t system_errors;
static volatile uint32_t can_frames_sent;

static void sensor_task(void *argument) {
    (void)argument;
    sensor_sample_t sample = {0};

    for (;;) {
        if (hal_sensor_read(&sample)) {
            (void)xQueueOverwrite(sample_queue, &sample);
            system_errors &= ~SYSTEM_SENSOR_FAULT;
        } else {
            system_errors |= SYSTEM_SENSOR_FAULT;
        }
        vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

static void can_task(void *argument) {
    (void)argument;
    sensor_sample_t sample = {0};
    uint8_t payload[8];

    for (;;) {
        if (xQueueReceive(sample_queue, &sample, pdMS_TO_TICKS(CAN_PERIOD_MS)) == pdPASS) {
            payload[0] = (uint8_t)(sample.temperature_centi_c >> 8);
            payload[1] = (uint8_t)sample.temperature_centi_c;
            payload[2] = (uint8_t)(sample.supply_mv >> 8);
            payload[3] = (uint8_t)sample.supply_mv;
            payload[4] = (uint8_t)(sample.sample_counter >> 8);
            payload[5] = (uint8_t)sample.sample_counter;
            payload[6] = 0;
            payload[7] = 0;

            if (hal_can_send(CAN_ID_SENSOR, payload, sizeof(payload))) {
                ++can_frames_sent;
                system_errors &= ~SYSTEM_CAN_FAULT;
            } else {
                system_errors |= SYSTEM_CAN_FAULT;
            }
        }
    }
}

static void health_task(void *argument) {
    (void)argument;
    char status[96];

    for (;;) {
        (void)snprintf(status, sizeof(status),
                       "status: errors=0x%02lx can_tx=%lu uptime=%lums\r\n",
                       (unsigned long)system_errors,
                       (unsigned long)can_frames_sent,
                       (unsigned long)hal_uptime_ms());
        hal_uart_write(status);
        hal_watchdog_refresh();
        vTaskDelay(pdMS_TO_TICKS(HEALTH_PERIOD_MS));
    }
}

void app_tasks_start(void) {
    sample_queue = xQueueCreate(1, sizeof(sensor_sample_t));
    configASSERT(sample_queue != NULL);

    configASSERT(xTaskCreate(sensor_task, "sensor", 256, NULL, 3, NULL) == pdPASS);
    configASSERT(xTaskCreate(can_task, "can", 256, NULL, 3, NULL) == pdPASS);
    configASSERT(xTaskCreate(health_task, "health", 256, NULL, 1, NULL) == pdPASS);
}
