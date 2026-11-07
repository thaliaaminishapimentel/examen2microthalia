# Resultados de pruebas P01–P10 — FRDM-MCXA156

**Proyecto:** monitor analógico con alarma, firmware en C.  
**Equipo:** FRDM-MCXA156, MCU-Link VCOM (COM3), UART 115200 8N1, potenciómetro **5 kΩ usado en las pruebas**.  
**Placa / compilación:** compilación, depuración y ejecución física comunicadas por la estudiante desde VS Code + MCUXpresso.  
**IMPORTANTE:** el enunciado requiere potenciómetro **10 kΩ** para la entrega. Repetir o justificar el montaje final con 10 kΩ.

## Criterio de estados

- **PASS (funcional):** comportamiento observado y sustentado por lecturas reales compartidas durante la práctica.
- **PARCIAL:** existe evidencia de parte de la prueba, pero faltan condiciones, mediciones o pruebas exigidas literalmente en el enunciado.
- **PENDIENTE:** no se aportaron datos suficientes para certificar el caso.
- No se han inventado mediciones ni imágenes. Para entrega final, agregar las capturas o video correspondientes a los casos probados por monitor serial.

## Matriz de pruebas (evaluación conservadora)

| ID | Estado | Lo observado realmente | Lo que falta para cerrar el caso | Evidencia |
|---|---|---|---|---|
| P01 | PARCIAL | El firmware ejecuta y reporta `RUN`, `THR=70`, `ALARM=0` con el potenciómetro bajo; compilación y ejecución física confirmadas. | Reiniciar físicamente y consultar `STATUS` después de 200 ms; capturar estado inicial y valor cerca de 0. | Lecturas UART compartidas en conversación; captura final pendiente |
| P02 | PARCIAL | Telemetría cada 1000 ms demostrada por `t` y la prueba P10 (30 intervalos consecutivos). LED rojo observado parpadeando. | Medir **eventos ADC cada 100 ms** y cambio de LED cada **500 ms** con analizador/osciloscopio o traza, tolerancia ±5 %. | `P10_log_20261106_210046.txt` |
| P03 | PARCIAL | ADC de 16 bits produjo `ADC=0 / PCT=0`, valores próximos a 50 %, y `ADC=65535 / PCT=100` al girar potenciómetro. | Medir tensiones de 0 %, 50 %, 100 % de VREF con multímetro y comprobar error ≤3 puntos porcentuales. | Lecturas UART compartidas; voltajes/capturas pendientes |
| P04 | PARCIAL | Umbral 70 %: a PCT≈73 se obtuvo `ALARM=1` y a PCT≈35 `ALARM=0`. Con THR=50: a 50 % `ALARM=1`, a 47 % retuvo `ALARM=1`, a 42 % quedó `ALARM=0`. | Ejecutar y registrar secuencia literal **60, 72, 67 y 62 %** con THR=70 y ALARM 0,1,1,0. | Lecturas UART compartidas |
| P05 | PARCIAL | SW2 alternó RUN→PAUSE y PAUSE→RUN; pulsación sostenida 2 s y liberación, sin repetición, confirmadas por la estudiante y la telemetría. | Documentar cinco pulsaciones y, si es posible, inyectar o medir rebotes menores de 30 ms. | Lecturas UART y observación física; video/captura pendientes |
| P06 | PARCIAL | `STOP` respondió `OK PAUSE`; `ALARM=0` y ADC/telemetría siguieron. `START` respondió `OK RUN`; SW2 también reanudó RUN y se encendió la alarma cuando PCT=52 % y THR=50 %. | Repetir secuencia específica del enunciado en PCT≈80 % con THR=70 y registrar LEDs. | Lecturas UART compartidas |
| P07 | PARCIAL | `THR 50`, `THR 80` y `THR 95` aceptados según las pruebas; cambiar a 95 apagó la alarma con PCT=52 %. `THR 10` se ensayó, pero no se conservó aquí la respuesta textual completa. | Adjuntar evidencia explícita de `THR 10` y reunir respuesta/estado tras cada umbral. | Lecturas UART compartidas; P10 respalda THR 50 y 95 |
| P08 | PARCIAL | `THR 96` y `THR 50x` respondieron `ERR` sin cambiar THR; el desconocido `MODE=RUN` también devolvió `ERR`. | Ensayar `THR 9`, `THR abc`, `THR 50 extra` y otro comando desconocido con evidencia. | Lecturas UART compartidas |
| P09 | PASS (funcional) | Fragmentación `STA` + `TUS\n` generó estado; `STATUS\r\n` generó estado; línea de **33 'A'** respondió `ERR`, seguida de `STATUS` válido. | Guardar capturas o traza independiente de estas pruebas para la entrega. | Lecturas UART compartidas |
| P10 | **PASS de prueba automática; PARCIAL de rúbrica completa** | **150/150** respuestas identificadas; 30/30 intervalos de `t` de 1000 ms; 0 líneas inesperadas; latencia máxima **16 ms observada desde Python/Windows**. `STOP/START` y THR alternaron correctamente con ADC estable a PCT≈42 %. | Para cumplir literalmente toda P10: variar potenciómetro **durante** los 30 s; y usar marcas de tiempo/instrumentación para validar latencias y períodos físicos (la cifra de Python es aproximada). | [Log TXT](P10_log_20261106_210046.txt), [CSV](P10_respuestas_20261106_210046.csv) |

## P10 — mediciones derivadas de los archivos originales recibidos

- Registro: `P10_log_20261106_210046.txt`.
- Datos por comando: `P10_respuestas_20261106_210046.csv` (**150 registros**, sin filas ausentes).
- Duración nominal: **30 s**; cadencia de envío: **5 comandos/s**, un comando aproximadamente cada 200 ms.
- Distribución de comandos: **90 STATUS**, **15 STOP**, **15 START**, **15 THR 50**, **15 THR 95**.
- Respuestas: **150 identificadas / 150 enviadas**, **0 sin identificar**.
- Latencia desde aplicación Python (aproximada): **máxima 16 ms**, **media 3.55 ms**; ninguna registrada por encima de 100 ms.
- Telemetría: 31 líneas periódicas observadas / **30 intervalos de 1000 ms exactos según `t` del MCU**. El último registro se recibió tras el envío del comando 150.
- Errores de línea: **0 inesperadas**.
- El ADC se mantuvo alrededor de **42 %** durante esta prueba automatizada; **no** se documentó un barrido del potenciómetro durante esa ventana.
- Las medidas desde Windows incluyen planificación del sistema, adaptador USB y resolución temporal del monitor. **No equivalen a una certificación por osciloscopio de la latencia MCU.**

## Observaciones físicas de la práctica

- LED rojo: actividad (parpadeo observado).
- LED blanco: alarma; encendió con `ALARM=1` y se apagó con `ALARM=0`/STOP, según la estudiante.
- La estabilidad del ADC mejoró al corregir las conexiones del cursor del potenciómetro.
- Un `THR 95` fue rechazado una vez con `ERR`, mientras `THR 50` permaneció; una repetición posterior sí generó `OK THR=95`. No se atribuye una causa sin pruebas.
- El comando `MODE=RUN` no está definido y se rechazó con `ERR` correctamente.

## Trazabilidad de la versión

- Las evidencias P10 fueron recogidas **6 de noviembre de 2026** en el equipo de Windows, en COM3 a 115200 baudios.
- Al entregar, documentar versión de MCUXpresso/SDK, hash de firmware **compilado y cargado** y versión de repositorio. El hash de este documento no demuestra por sí solo que el firmware en la placa sea exactamente ese commit.
- Adjuntar video/capturas y mediciones pendientes antes de declarar que toda la rúbrica P01–P10 está aprobada.
