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
static sensor_sample_t last_sample;

static void sensor_task(void *argument) {
    (void)argument;
    sensor_sample_t sample = {0};

    for (;;) {
        if (hal_sensor_read(&sample)) {
            last_sample = sample;
            (void)xQueueOverwrite(sample_queue, &sample);
            system_errors &= ~SYSTEM_SENSOR_FAULT;
        } else {
            system_errors |= SYSTEM_SENSOR_FAULT;
        }
        vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

static void command_task(void *argument) {
    (void)argument;
    char command[UART_LINE_SIZE];

    for (;;) {
        if (!hal_uart_read_line(command, sizeof(command), portMAX_DELAY)) {
            continue;
        }

        if (strcmp(command, "help") == 0) {
            hal_uart_write("commands: help status sensor read can stats config show reset\r\n");
        } else if (strcmp(command, "status") == 0) {
            char response[96];
            (void)snprintf(response, sizeof(response),
                           "errors=0x%08lx can_tx=%lu uptime=%lums\r\n",
                           (unsigned long)system_errors,
                           (unsigned long)can_frames_sent,
                           (unsigned long)hal_uptime_ms());
            hal_uart_write(response);
        } else if (strcmp(command, "sensor read") == 0) {
            char response[80];
            (void)snprintf(response, sizeof(response), "temperature=%d.%02dC supply=%umV\r\n",
                           last_sample.temperature_centi_c / 100,
                           last_sample.temperature_centi_c % 100,
                           last_sample.supply_mv);
            hal_uart_write(response);
        } else if (strcmp(command, "can stats") == 0) {
            char response[48];
            (void)snprintf(response, sizeof(response), "can_tx=%lu\r\n",
                           (unsigned long)can_frames_sent);
            hal_uart_write(response);
        } else {
            hal_uart_write("error: unknown command; use 'help'\r\n");
        }
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
    configASSERT(xTaskCreate(command_task, "command", 256, NULL, 2, NULL) == pdPASS);
    configASSERT(xTaskCreate(health_task, "health", 256, NULL, 1, NULL) == pdPASS);
}
