# System Architecture

## Boundaries

`firmware/src` contains product logic. `firmware/hal` is the only layer that should know about STM32 HAL/LL, generated handles, and pin details. This separation makes the application easier to test and migrate between STM32 families.

## Tasks and priorities

| Task | Period | Priority | Responsibility |
|---|---:|---:|---|
| SensorTask | 100 ms | 3 | I2C acquisition and sample validation |
| CanTask | event/250 ms | 3 | CAN serialization and transmission |
| CommandTask | event | 2 | UART CLI and diagnostic commands |
| HealthTask | 1000 ms | 1 | Watchdog, uptime, and system status |

Queue capacity is intentionally bounded. In a real system, backpressure and sample loss must be observable rather than silently ignored.

## Fault handling

- Sensor failures set `SYSTEM_SENSOR_FAULT`;
- CAN transmission failures set `SYSTEM_CAN_FAULT`;
- HealthTask publishes status and refreshes the watchdog;
- A watchdog reset should be recorded during system startup;
- Maintenance commands must not block real-time tasks.
