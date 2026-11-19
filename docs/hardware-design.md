# Hardware Design Guidelines

## PCB blocks

- STM32 MCU with accessible SWD header;
- 3.3 V-compatible CAN transceiver;
- selectable CAN termination using a jumper;
- 5 V to 3.3 V power regulator;
- ESD protection on external connectors;
- service UART connector;
- I2C sensor and test points for power, CANH, and CANL.

## Design review checklist

- [ ] Decoupling capacitors placed close to each power pin;
- [ ] Current return paths and ground plane reviewed;
- [ ] I2C pull-up values calculated;
- [ ] CAN termination fitted only at network endpoints;
- [ ] Test points identified on the silkscreen;
- [ ] ERC and DRC completed with no unjustified errors;
- [ ] BOM includes manufacturer, part number, and approved alternative.
