# Validation Plan

## Firmware

- Confirm that the acquisition task runs every 100 ms.
- Disconnect the sensor and verify `SYSTEM_SENSOR_FAULT`.
- Interrupt the CAN bus and verify `SYSTEM_CAN_FAULT`.
- Confirm that the watchdog resets the system when the application is stalled.
- Verify that the queue preserves the most recent sensor sample.

## Communication

- Verify `0x180` frames with a CAN analyzer.
- Validate field endianness, scaling, and limits.
- Send `0x200` commands and verify the UART response.

## Hardware

- Measure the 3.3 V rail under load.
- Verify 120 ohm termination at the CAN network endpoints.
- Test reverse-polarity and ESD protection.
- Record assembly photos and a two-node CAN test video.
