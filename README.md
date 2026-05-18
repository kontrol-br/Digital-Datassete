# Digital Datasette RP2040 Firmware

Firmware C++ modular para Raspberry Pi Pico/RP2040 com Arduino-Pico. A primeira versão entrega a base de um Digital Datasette multi-plataforma para ZX Spectrum/TK90X, ZX81/TK85 e MSX.

## Recursos iniciais

- Auto-teste de boot para serial, LCD ST7735 80x160, ADC, buzzer e cartão SD.
- Criação/verificação das pastas `/ZX_SPECTRUM`, `/ZX81` e `/MSX` no SD.
- Menu principal com seleção de plataforma, Tools e Settings.
- Submenus por plataforma com stubs de playback/gravação.
- Monitor de entrada analógica em GP26/ADC0 com média, Vpp, barra e status LOW/GOOD/HIGH/CLIP.
- Navegador de SD com entrada em pastas, retorno para pasta anterior e exclusão de arquivos com confirmação.
- Buzzer configurável no menu Settings e persistência simples em `/CONFIG.INI` quando o SD está disponível.

## GPIOs padrão

Os pinos ficam centralizados em `src/hardware_pins.h` para facilitar alteração do hardware. A tabela abaixo resume as ligações sugeridas para Raspberry Pi Pico/RP2040.

| Função | Sinal do módulo | GPIO / nome no firmware | Pino físico Pico | Observações |
| --- | --- | --- | --- | --- |
| Entrada digital condicionada | Saída do 74HC14 | GP14 / `Pins::DIGITAL_IN_PIN` | 19 | Entrada para captura de pulsos já em nível lógico 3,3 V. Não aplicar 5 V direto no GPIO. |
| Leitura analógica | Sinal analógico de áudio/nível | GP26 / ADC0 / `Pins::ADC_IN_PIN` | 31 | Faixa máxima do ADC do RP2040 é 0 a 3,3 V; use divisor/condicionamento se a fonte puder exceder 3,3 V. |
| Alimentação +5 V | VBUS | VBUS | 40 | Saída/entrada de 5 V do USB. Use GND comum com todos os módulos. |
| Display ST7735S SPI | CS | GP9 / `Pins::LCD_CS` | 12 | Display SPI colorido padrão do firmware. |
| Display ST7735S SPI | DC/A0 | GP8 / `Pins::LCD_DC` | 11 | Linha de comando/dados. |
| Display ST7735S SPI | RST/RES | GP12 / `Pins::LCD_RST` | 16 | Reset do display. |
| Display ST7735S SPI | SCK/SCL | GP10 / `Pins::LCD_SCK` | 14 | Clock SPI do display. |
| Display ST7735S SPI | SDA/MOSI | GP11 / `Pins::LCD_MOSI` | 15 | Dados SPI para o display. |
| Display ST7735S SPI | VCC/LED | 3V3 ou VBUS, conforme módulo | 36 ou 40 | Preferir 3,3 V quando o módulo suportar; alguns módulos aceitam 5 V por terem regulador. |
| Display ST7735S SPI | GND | GND | 3, 8, 13, 18, 23, 28, 33 ou 38 | Terra comum. |
| Display SSD1306 I2C (opcional) | SDA | GP20 / `Pins::OLED_SDA` | 26 | Alternativa ao ST7735S; requer implementação/biblioteca SSD1306 no firmware. |
| Display SSD1306 I2C (opcional) | SCL | GP21 / `Pins::OLED_SCL` | 27 | Barramento I2C sugerido sem conflito com o SPI do SD/display. |
| Display SSD1306 I2C (opcional) | VCC | 3V3 | 36 | Use 3,3 V para manter os pull-ups I2C em nível seguro ao RP2040. |
| Display SSD1306 I2C (opcional) | GND | GND | 3, 8, 13, 18, 23, 28, 33 ou 38 | Terra comum. |
| Encoder/potenciômetro de menu | Esquerda / A / CLK | GP2 / `Pins::ENCODER_LEFT` | 4 | Entrada com pull-up interno; acionar para GND. Também corresponde ao botão UP atual. |
| Encoder/potenciômetro de menu | Direita / B / DT | GP3 / `Pins::ENCODER_RIGHT` | 5 | Entrada com pull-up interno; acionar para GND. Também corresponde ao botão DOWN atual. |
| Encoder/potenciômetro de menu | Push / SW | GP6 / `Pins::ENCODER_PUSH` | 9 | Entrada com pull-up interno; acionar para GND. Também corresponde ao botão SELECT atual. |

> Observação: o firmware atual lê `GP2`, `GP3` e `GP6` como botões digitais ativos em nível baixo. Um encoder rotativo em quadratura pode ser ligado nesses sinais, mas a decodificação específica de quadratura ainda deve ser implementada se você quiser giro contínuo em vez de eventos simples esquerda/direita.

## Compilação

Este projeto usa PlatformIO com Arduino-Pico:

```bash
pio run -e pico
```

Para uma checagem sintática em ambiente host sem hardware/Arduino, use:

```bash
g++ -std=c++17 -DUNIT_TEST -Isrc src/*.cpp -o /tmp/digital-datasette-host
```

## Próximos passos planejados

- Implementar TAP/TZX/CAS/P playback real usando PWM, PIO ou temporização dedicada.
- Implementar captura de pulsos no GP14 via interrupção/PIO.
- Persistir mais preferências em `/CONFIG.INI`.
