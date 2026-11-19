#include <assert.h>
#include <stdint.h>

#include "app_config.h"

static uint16_t read_u16_le(const uint8_t *data) {
    return (uint16_t)data[0] | ((uint16_t)data[1] << 8U);
}

int main(void) {
    const uint8_t encoded_temperature[] = {0x10U, 0x27U};

    assert(CAN_ID_SENSOR == 0x180U);
    assert(CAN_ID_STATUS == 0x181U);
    assert(CAN_ID_COMMAND == 0x200U);
    assert(read_u16_le(encoded_temperature) == 10000U);
    assert(SENSOR_PERIOD_MS == 100U);
    return 0;
}
