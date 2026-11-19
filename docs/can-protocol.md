# CAN Protocol

All numeric fields use little-endian byte order.

## `0x180` — Sensor sample

| Byte | Field | Type | Scale |
|---:|---|---|---|
| 0–1 | temperature | `int16_t` | centi-degrees Celsius |
| 2–3 | supply voltage | `uint16_t` | mV |
| 4–5 | sample counter | `uint16_t` | incremented per sample |
| 6 | payload version | `uint8_t` | currently `1` |
| 7 | reserved | `uint8_t` | `0` |

## `0x181` — System status

| Byte | Field | Type |
|---:|---|---|
| 0–3 | fault bitmap | `uint32_t` |
| 4–7 | uptime | `uint32_t`, ms |

## `0x200` — Command

| Byte | Field |
|---:|---|
| 0 | opcode |
| 1–4 | little-endian argument |
| 5–7 | reserved |

Any payload change must increment the payload version and update this document.
