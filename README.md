# STM32 Industrial Gateway

![Status](https://img.shields.io/badge/status-in%20development-orange)
![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-blue)
![Platform](https://img.shields.io/badge/platform-STM32-03234B)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-00A98F)

Modular embedded gateway for industrial data acquisition, diagnostics, and CAN bus communication.

> **Project status:** The application architecture and host-side tests are implemented. The hardware abstraction layer must be connected to the selected STM32 board before claiming hardware validation.

## Overview

The gateway reads sensor data over I2C, publishes measurements through CAN, and provides a UART diagnostic CLI. The firmware is organized with FreeRTOS tasks, explicit fault flags, bounded queues, and a watchdog supervised by a dedicated health task.

This project is designed to demonstrate production-oriented embedded development practices: separation of concerns, documented interfaces, testable application logic, and hardware design planning.

## Key features

- STM32 firmware application in C;
- FreeRTOS tasks, queues, priorities, and watchdog supervision;
- I2C sensor acquisition;
- CAN measurement and status frames;
- UART diagnostic command-line interface;
- Fault bitmap and runtime status reporting;
- Hardware abstraction layer isolated from application logic;
- Documented CAN payloads and scaling rules;
- Host-side protocol tests;
- GitHub Actions CI with compiler warnings and static analysis;
- Custom PCB design plan with manufacturing checklist.

## System architecture

```text
 I2C sensor ──> SensorTask ──> SampleQueue ──> CanTask ──> CAN network
                                      │
                                      └──────> Status / diagnostics

 UART ────────> CommandTask

 HealthTask ──> Fault monitor + watchdog + uptime reporting
```

The application depends only on the interfaces in `firmware/hal/hal_port.h`. STM32Cube HAL handles, pin assignments, and peripheral-specific code belong in the HAL implementation, keeping the product logic portable and testable.

## UART diagnostic CLI

```text
help
status
sensor read
can stats
config show
config save
reset
```

The CLI is intended for service and commissioning. It must not block real-time acquisition or CAN transmission.

## CAN protocol

| CAN ID | Direction | Payload |
|---|---|---|
| `0x180` | Gateway → network | Temperature, supply voltage, sample counter |
| `0x181` | Gateway → network | Fault bitmap and uptime |
| `0x200` | Network → gateway | Command and argument |

All multi-byte values are little-endian. Temperature is encoded in centi-degrees Celsius and supply voltage in millivolts. The complete payload definition is available in [`docs/can-protocol.md`](docs/can-protocol.md).

## Repository structure

```text
firmware/inc/       Public application interfaces and configuration
firmware/src/       FreeRTOS tasks and product logic
firmware/hal/       STM32 HAL integration boundary
tests/              Host-side protocol tests
docs/               Architecture, protocol, validation, and hardware notes
pcb/                KiCad design entry point and manufacturing documentation
.github/workflows/  Continuous integration pipeline
```

## Getting started

1. Create an STM32CubeIDE project for the target MCU.
2. Enable FreeRTOS, CAN, I2C, UART, SWD, and an independent watchdog.
3. Add the files under `firmware/inc` and `firmware/src` to the project.
4. Implement the functions declared in `firmware/hal/hal_port.h` using STM32Cube HAL.
5. Configure the pins, clock tree, CAN bitrate, I2C speed, and UART baud rate.
6. Build, flash, and validate the board using [`docs/validation-plan.md`](docs/validation-plan.md).

The repository intentionally does not claim a specific STM32 part number yet. Selecting the MCU is part of the hardware phase and should be recorded in the schematic, BOM, and release notes.

## Quality and engineering practices

- Hardware dependencies isolated behind interfaces;
- Explicit fault handling instead of silent failures;
- Watchdog refreshed only by the health-monitoring task;
- Versioned and documented CAN payloads;
- Host tests for protocol constants and serialization;
- Automated CI for every push and pull request;
- PCB design review checklist covering power, EMC, CAN termination, ERC, and DRC.

## Roadmap

- [x] Define firmware architecture
- [x] Document CAN protocol
- [x] Implement UART diagnostic task
- [x] Add host-side protocol test
- [x] Add CI and static-analysis workflow
- [ ] Select target STM32 and sensor components
- [ ] Implement STM32 HAL integration
- [ ] Complete KiCad schematic and PCB layout
- [ ] Manufacture and assemble the first board
- [ ] Validate two-node CAN communication on the bench
- [ ] Publish measurements, photos, and a v1.0 release

## Documentation

- [Software architecture](docs/architecture.md)
- [CAN protocol](docs/can-protocol.md)
- [Hardware design guidelines](docs/hardware-design.md)
- [Validation plan](docs/validation-plan.md)

## License

This project is licensed under the MIT License. See [`LICENSE`](LICENSE).

## Author

**Joao Vitor Lima** — embedded systems, firmware, electronics, and PCB design.
