# STM32 Industrial Gateway

Projeto de portfólio de um gateway para aquisição de dados e diagnóstico de máquinas.

## Objetivo

O dispositivo lê sensores por I2C, publica dados em uma rede CAN e oferece uma CLI de diagnóstico pela UART. O firmware usa FreeRTOS para separar aquisição, comunicação, armazenamento e supervisão.

## Habilidades demonstradas

- Firmware embarcado em C para STM32
- FreeRTOS: tasks, filas, mutexes e notificações
- CAN com protocolo documentado
- I2C para sensores
- UART com CLI de diagnóstico
- Watchdog e tratamento de falhas
- Organização modular de drivers e aplicação
- Desenvolvimento de esquemático e PCB

## Arquitetura

```text
Sensor I2C ──> SensorTask ──> DataQueue ──> CanTask ──> Rede CAN
                                      └──> StorageTask ──> Flash/EEPROM
UART/CLI ───────────────────────────────> CommandTask
                         HealthTask ────> Watchdog + diagnóstico
```

## Protocolo CAN

| ID | Direção | Dados |
|---|---|---|
| `0x180` | gateway → rede | temperatura `int16`, tensão `uint16`, contador `uint16` |
| `0x181` | gateway → rede | estado, erros e uptime |
| `0x200` | rede → gateway | comando e argumento |

Temperatura é enviada em centésimos de grau Celsius e tensão em milivolts.

## CLI UART

```text
help
status
sensor read
can stats
config show
config save
reset
```

## Estrutura

```text
firmware/                 Código independente da aplicação
firmware/inc/             Interfaces públicas
firmware/src/             Tasks e lógica de aplicação
firmware/hal/             Pontos de integração com STM32Cube/HAL
tests/                    Testes da lógica sem hardware
docs/                     Arquitetura, protocolo e plano de validação
pcb/                      Esquemático, layout, BOM e arquivos de fabricação
```

## Como usar

1. Crie um projeto STM32CubeIDE para o MCU escolhido.
2. Habilite FreeRTOS, CAN, I2C, UART e watchdog no `.ioc`.
3. Copie `firmware/inc` e `firmware/src` para o projeto.
4. Implemente as funções em `firmware/hal` usando os periféricos gerados.
5. Compile, grave a placa e valide seguindo `docs/validation-plan.md`.

O código deste repositório é um núcleo de aplicação portátil; nomes de handles e configuração de pinos dependem do STM32 escolhido.

## Roadmap

- [x] Arquitetura inicial do firmware
- [x] Protocolo CAN documentado
- [x] CLI UART e supervisão
- [ ] Driver de sensor real
- [ ] Esquemático e PCB final
- [ ] Testes em bancada com dois nós CAN
- [ ] Fotos, vídeo e release v1.0

## Autor

Projeto desenvolvido para demonstrar competências em sistemas embarcados, firmware e hardware eletrônico.
