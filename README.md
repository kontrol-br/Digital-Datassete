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

Os pinos ficam centralizados em `src/hardware_pins.h` para facilitar alteração do hardware.

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
