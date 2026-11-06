# Resultado de pruebas P01-P10 (para completar sobre la placa real)

**Importante:** Los resultados de hardware y las mediciones NO se pueden certificar sin la placa, el multímetro y el monitor serial. No marcar PASS automáticamente.

| Prueba | Resultado esperado resumido | Observado / medición | PASS / FAIL / PENDIENTE | Evidencia (archivo) |
|---|---|---|---|---|
| P01 | Arranque RUN, THR=70, ALARM=0, lectura ~0 % | Pendiente de medir | PENDIENTE | `P01_terminal.png` |
| P02 | ADC 100 ms, actividad 500 ms, UART 1000 ms, ±5 % | Pendiente de medir | PENDIENTE | `P02_tiempos.csv` |
| P03 | 0, 50, 100 % con error <=3 puntos | Pendiente de medir | PENDIENTE | `P03_multimetro.jpg` |
| P04 | Secuencia de alarma 0, 1, 1, 0 (60, 72, 67, 62 %) | Pendiente de medir | PENDIENTE | `P04_serie.png` |
| P05 | 1 alternancia por pulsación, rebote <30 ms rechazado | Pendiente de medir | PENDIENTE | `P05_video.mp4` |
| P06 | STOP en PAUSE, ADC y UART siguen; START reinicia alarma | Pendiente de medir | PENDIENTE | `P06_serie.png` |
| P07 | THR 50, 80, 10, 95 OK y alarma coherente | Pendiente de medir | PENDIENTE | `P07_serie.png` |
| P08 | THR inválidos y desconocidos responden ERR sin modificar estado | Pendiente de medir | PENDIENTE | `P08_serie.png` |
| P09 | STATUS fragmentado/CRLF; línea 33 caracteres produce ERR | Pendiente de medir | PENDIENTE | `P09_serie.png` |
| P10 | 30 s, 5 STATUS/s, respuesta <=100 ms sin corromper líneas | Pendiente de medir | PENDIENTE | `P10_serie_tiempos.csv` |

## Validación sin placa (no sustituye la evaluación)

`tests/test_core.c` ejecuta pruebas locales de lógica, umbral, histéresis, parser y rebotes. Resultado registrado al crear este proyecto: **PASS en compilador C de escritorio**, NO en MCU.

## Versión final

Anotar el hash Git de la versión que se demuestre en la placa: `________________`.
