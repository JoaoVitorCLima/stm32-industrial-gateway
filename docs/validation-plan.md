# Plano de validação

## Firmware

- Confirmar que a task de aquisição executa a cada 100 ms.
- Desconectar o sensor e verificar `SYSTEM_SENSOR_FAULT`.
- Interromper o barramento CAN e verificar `SYSTEM_CAN_FAULT`.
- Confirmar que o watchdog reinicia o sistema quando a aplicação trava.
- Validar que a fila não perde o valor mais recente do sensor.

## Comunicação

- Verificar frames `0x180` com um analisador CAN.
- Validar endianness, escala e limites dos campos.
- Enviar comandos `0x200` e conferir resposta pela UART.

## Hardware

- Medir alimentação de 3,3 V sob carga.
- Verificar terminação de 120 ohms nas extremidades da rede CAN.
- Testar proteção contra inversão de polaridade e ESD.
- Registrar fotos da PCB montada e do teste com dois nós.
