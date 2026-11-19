# Diretrizes de hardware

## Blocos da PCB

- MCU STM32 com SWD acessível;
- transceptor CAN compatível com 3,3 V;
- terminação CAN selecionável por jumper;
- regulador 5 V para 3,3 V;
- proteção ESD nos conectores externos;
- conector UART de serviço;
- sensor I2C e pontos de teste para alimentação, CANH e CANL.

## Checklist de revisão

- [ ] desacoplamento próximo a cada pino de alimentação;
- [ ] retorno de corrente e plano de terra revisados;
- [ ] pull-ups I2C dimensionados;
- [ ] terminação CAN presente somente nas extremidades;
- [ ] test points identificados no silkscreen;
- [ ] ERC e DRC sem erros não justificados;
- [ ] BOM com fabricante, part number e substituto aprovado.
