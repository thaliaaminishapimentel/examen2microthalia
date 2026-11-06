# Monitor analógico con alarma — FRDM-MCXA156

**Proyecto de examen de Microcontroladores (ITLA)** — GPIO + TIMER (CTIMER1) + ADC (LPADC1) + UART (LPUART0) en un solo firmware en **C**.

**Nombre:** [COMPLETAR]  
**Matrícula:** [COMPLETAR]  
**Placa:** NXP FRDM-MCXA156  
**MCU:** MCXA156VLL (Arm Cortex-M33)  
**IDE:** Visual Studio **Code** + MCUXpresso for VS Code (no Microsoft Visual Studio clásico)  
**SDK:** MCUXpresso SDK para FRDM-MCXA156 (descargar desde NXP)  
**Compilador:** Arm GNU Toolchain (instalar desde MCUXpresso Installer).

> **Estado:** núcleo de lógica verificado en PC; **compilación cruzada y pruebas físicas pendientes**. No contiene un `.elf` porque debe construirse para la versión del SDK instalada y tu placa.

## Materiales y conexiones reales

| Elemento | Pin de la placa | Conexión |
|---|---|---|
| Potenciómetro 10 kΩ extremo 1 | **J3-8, LDO_3V3** | 3.3 V (NO 5 V) |
| Potenciómetro extremo 2 | **J3-12 o J3-14, GND** | Tierra |
| Potenciómetro terminal central/cursor | **A0 = J4-2 = P1_10 = ADC1_A8** | Entrada 0 a 3.3 V |
| LED actividad (ánodo) | **D2 = J1-6 = P3_1** | Pin D2 -> resistor 330 Ω -> ánodo; cátodo -> GND |
| LED alarma (ánodo) | **D4 = J1-10 = P3_31** | Pin D4 -> resistor 330 Ω -> ánodo; cátodo -> GND |
| Pulsador (ya integrado) | **SW2 WAKE UP = P1_7** | GPIO entrada con pull-up, pulsación = nivel 0 |
| Puerto UART de monitor | **LPUART0 / MCU-Link VCOM** | USB-C en **J21, MCU-Link**; no precisa cable UART externo |

**Advertencia:** desconecta el USB antes de cablear. NO conectes el cursor del potenciómetro a 5 V. La tensión de referencia del ADC es VDDA/VREFH de la placa, **nominalmente 3.3 V** (se recomienda medirla). Comparte GND. Usa resistencias limitadoras en ambos LED.

## Parámetros de firmware

- ADC: módulo `ADC1`, canal `A8` (pin P1_10), **16 bits**, rango de código 0–65535, **VREF = VDDA ≈ 3.3 V**, resolución alta (`kLPADC_ConversionResolutionHigh`), referencia `kLPADC_ReferenceVoltageAlt3`, reloj de ADC `FRO12M / 1 = 12 MHz`.
- Porcentaje: `PCT = (100*ADC + 32767)/65535` (aritmética 32 bit, redondeo entero).
- CTIMER: módulo **CTIMER1**, entrada `FRO_HF` (la placa normalmente configura 96 MHz), divisor `1`, prescaler de CTIMER `0` (divisor efectivo 1), match0 = `CLOCK_GetCTimerClkFreq(1)/1000`. Ejemplo para 96 MHz: `96,000` cuentas => tick de 1 ms. El reloj se consulta en ejecución, no se asume para programar el match.
- Tareas planificadas en el bucle principal desde tick de hardware: ADC cada 100 ms; alternancia del LED de actividad cada 500 ms; telemetría cada 1000 ms; antirrebote de 30 ms sin esperas bloqueantes.
- UART: **115200, 8 bits, sin paridad, 1 stop, sin control de flujo**, CRLF para todas las respuestas. Interrupción con búferes RX/TX; las líneas se interpretan fuera del ISR.
- Estado inicial: RUN, THR=70, ALARM=0, tiempo=0; alarma evaluada desde la primera conversión válida.
- Histéresis en RUN: enciende con `PCT >= THR`; apaga con `PCT < THR-5`; mantiene entre ambos. STOP/PAUSE apaga la alarma sin detener las otras tareas. START desde PAUSE y THR en RUN reevalúan desde la última muestra.

## Cómo compilar y grabar (Windows, Visual Studio Code)

**Primera instalación (necesaria una sola vez en el ordenador):**

1. Instala el **[MCUXpresso Installer de NXP](https://www.nxp.com/document/guide/getting-started-with-frdm-mcxa156:GS-FRDM-MCXA156)** con **MCUXpresso SDK Developer**, **ARM GNU Toolchain** y **LinkServer**. Instala la extensión **MCUXpresso for VS Code** de NXP.
2. Desde MCUXpresso, incorpora un **SDK para FRDM-MCXA156** usando la opción de repositorio remoto/archivo de SDK. El SDK (drivers, inicio, linker y biblioteca) se mantiene externo a este GitHub, por licencia/tamaño.
3. Descomprime o clona este repositorio. En VS Code abre **File > Open Folder** y selecciona `frdm-mcxa156-monitor`.
4. En el panel **MCUXpresso**, usa **Import Existing CMake Project** / **Import project** y selecciona esta carpeta, el SDK y la placa **FRDM-MCXA156** (núcleo cm33, toolchain Arm GNU). CMake usa el `CMakeLists.txt` y `prj.conf` incluidos.
5. En **Projects**, selecciona el proyecto `analog_alarm` y pulsa **Build Project**. Conecta USB-C **al puerto J21 (MCU-Link)** y pulsa **Debug / Flash**. Dale **Continue** si el programa queda detenido en `main`.
6. Abre **Serial Monitor** en VS Code, selecciona `COMx - MCU-Link VCom Port` y fija **115200 8N1**, con terminador LF o CRLF.

Si tu versión del SDK no importa correctamente este CMake independiente, usa la **ruta alternativa NXP oficial**: importa el ejemplo `driver_examples/lpadc/polling` para FRDM-MCXA156 como **Freestanding Project**, luego ejecuta el script `scripts/instalar_en_ejemplo.ps1 -Proyecto "C:\RUTA\AL\PROYECTO_IMPORTADO"` en PowerShell. El script reemplaza el C principal, añade la lógica y adapta los pines y dependencias. Vuelve a compilar desde MCUXpresso. El ejemplo aporta `hardware_init.c`, `pin_mux.h`, `app.h` y archivos CMake ya probados para la versión de SDK seleccionada. El script modifica el proyecto importado: guarda una copia si necesitas conservarlo.

**No es suficiente hacer `gcc main.c` en PowerShell:** este firmware usa registros y periféricos de la placa, por lo que requiere el SDK, el compilador Arm y la carga mediante MCU-Link.

## Prueba inmediata en el monitor serial

Con el potenciómetro en mínimo, espera unos 200 ms desde el reinicio y escribe `STATUS` seguido de Enter. La salida será similar a:

```text
t=200;ADC=0;PCT=0;THR=70;MODE=RUN;ALARM=0
```

La telemetría se imprime sola cada segundo. Aumenta el potenciómetro: el porcentaje cambiará y el LED de alarma se encenderá al superar 70 %, y se apagará al bajar de 65 %.

Comandos (sensibles a mayúsculas, una línea por Enter):

```text
STATUS      -> t=...;ADC=...;PCT=...;THR=70;MODE=RUN;ALARM=0
THR 50      -> OK THR=50
STOP        -> OK PAUSE
START       -> OK RUN
THR 96      -> ERR
```

También se puede alternar RUN/PAUSE pulsando **SW2**. Mantener SW2 presionado durante 2 s no genera repeticiones. Una línea de más de 32 caracteres recibe ERR y el parser se recupera en el próximo LF.

## Pruebas / evidencias / entrega

1. **En PC, sin placa:** compilar y ejecutar `tests/test_core.c` con GCC/Clang; ejemplo `gcc -std=c11 -Wall -Wextra -Werror tests/test_core.c -o test_core.exe` y `./test_core.exe`. Solo verifica la lógica reutilizada por el firmware, no los periféricos.
2. **En la placa:** completar P01–P10 y adjuntar capturas, trazas UART con marcas de tiempo, voltajes del potenciómetro, mediciones de periodos y video; plantilla en [`evidencias/RESULTADOS_P01_P10.md`](evidencias/RESULTADOS_P01_P10.md). No declares PASS sin mediciones.
3. **En GitHub:** registra los cambios de configuración, periféricos, integración y pruebas en commits reales. Identifica la versión probada con su hash/etiqueta.

## Referencias técnicas oficiales

- [Manual del FRDM-MCXA156, UM12121 (pines J1/J3/J4, USB)](https://www.nxp.com/docs/en/user-manual/UM12121.pdf)
- [NXP Getting Started (instalación SDK, VS Code, Build, Flash y VCOM)](https://www.nxp.com/document/guide/getting-started-with-frdm-mcxa156:GS-FRDM-MCXA156)
- [SDK ejemplo LPADC polling para FRDM-MCXA156](https://github.com/nxp-mcuxpresso/mcuxsdk-examples/tree/main/_boards/frdmmcxa156/driver_examples/lpadc/polling)
- [SDK ejemplo CTIMER simple match](https://github.com/nxp-mcuxpresso/mcuxsdk-examples/tree/main/driver_examples/ctimer/simple_match_interrupt)

## Autoría y estado de verificación

Firmware elaborado específicamente para este reto académico. La prueba del núcleo C fue ejecutada localmente y pasó; no se ha realizado la compilación cruzada en un equipo con MCUXpresso SDK ni la ejecución real en FRDM-MCXA156. Es indispensable compilar, cargar y medir en el hardware antes de indicar que el examen está concluido.
