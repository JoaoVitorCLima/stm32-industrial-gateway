#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>

typedef struct {
    int16_t temperature_centi_c;
    uint16_t supply_mv;
    uint16_t sample_counter;
} sensor_sample_t;

typedef enum {
    SYSTEM_OK = 0,
    SYSTEM_SENSOR_FAULT = 1,
    SYSTEM_CAN_FAULT = 2,
    SYSTEM_STORAGE_FAULT = 4
} system_error_t;

#endif
