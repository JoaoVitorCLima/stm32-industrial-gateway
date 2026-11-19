# Arquitetura do sistema

## Fronteiras

`firmware/src` contém a lógica de produto. `firmware/hal` é a única camada que deve conhecer HAL/LL, handles gerados pelo STM32Cube e detalhes de pinos. Essa separação facilita testes e migração entre famílias STM32.

## Tarefas e prioridades

| Tarefa | Período | Prioridade | Responsabilidade |
|---|---:|---:|---|
| SensorTask | 100 ms | 3 | Aquisição I2C e validação de amostra |
| CanTask | evento/250 ms | 3 | Serialização e transmissão CAN |
| CommandTask | evento | 2 | CLI UART e comandos de diagnóstico |
| HealthTask | 1000 ms | 1 | Watchdog, uptime e status |

As filas têm capacidade limitada de propósito. Em sistemas reais, backpressure e perda de amostras devem ser observáveis, nunca silenciosos.

## Tratamento de falhas

- Falha de sensor seta `SYSTEM_SENSOR_FAULT`;
- Falha de transmissão seta `SYSTEM_CAN_FAULT`;
- A HealthTask envia o status e atualiza o watchdog;
- Um reset por watchdog deve ser registrado na inicialização;
- Comandos de manutenção não devem bloquear tarefas de tempo real.
