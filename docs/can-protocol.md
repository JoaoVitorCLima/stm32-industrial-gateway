# Protocolo CAN

Formato little-endian para os campos numéricos.

## `0x180` — sensor sample

| Byte | Campo | Tipo | Escala |
|---:|---|---|---|
| 0–1 | temperatura | `int16_t` | centésimos de °C |
| 2–3 | alimentação | `uint16_t` | mV |
| 4–5 | contador | `uint16_t` | incremento por amostra |
| 6 | versão do payload | `uint8_t` | atualmente `1` |
| 7 | reservado | `uint8_t` | `0` |

## `0x181` — status

| Byte | Campo | Tipo |
|---:|---|---|
| 0–3 | bitmap de erros | `uint32_t` |
| 4–7 | uptime | `uint32_t`, ms |

## `0x200` — comando

| Byte | Campo |
|---:|---|
| 0 | opcode |
| 1–4 | argumento little-endian |
| 5–7 | reservado |

Qualquer alteração de payload deve gerar uma nova versão e atualizar este documento.
