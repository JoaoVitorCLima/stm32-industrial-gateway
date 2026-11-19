#ifndef HAL_PORT_H
#define HAL_PORT_H

#include <stdbool.h>
#include <stdint.h>
#include "app_types.h"

bool hal_sensor_read(sensor_sample_t *sample);
bool hal_can_send(uint32_t id, const uint8_t *data, uint8_t length);
void hal_uart_write(const char *text);
bool hal_uart_read_line(char *buffer, uint32_t capacity, uint32_t timeout_ms);
void hal_watchdog_refresh(void);
uint32_t hal_uptime_ms(void);

#endif
