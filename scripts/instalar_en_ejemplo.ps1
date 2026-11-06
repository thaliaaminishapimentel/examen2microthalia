# Alternative for Windows if CMake-project import does not work with a local SDK.
# 1. Import NXP's driver_examples/lpadc/polling as a Freestanding app.
# 2. Run this script with -Proyecto pointing to the IMPORTED example folder.
param(
  [Parameter(Mandatory=$true)] [string]$Proyecto
)
$ErrorActionPreference = 'Stop'
$raiz = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
if (-not (Test-Path $Proyecto)) { throw "No existe: $Proyecto" }
$dir = (Resolve-Path $Proyecto).Path
$main = Get-ChildItem -Path $dir -Recurse -File -Filter lpadc_polling.c |
    Where-Object { $_.FullName -notmatch '[\\/](build|Debug|Release)[\\/]' } |
    Select-Object -First 1
if (-not $main) { throw 'No encuentro lpadc_polling.c; importa primero ese ejemplo oficial de NXP.' }
$target = $main.DirectoryName
Copy-Item (Join-Path $raiz 'src/lpadc_polling.c') $main.FullName -Force
Copy-Item (Join-Path $raiz 'src/alarm_core.h') (Join-Path $target 'alarm_core.h') -Force
$pins = Get-ChildItem -Path $dir -Recurse -File -Filter pin_mux.c |
    Where-Object { $_.FullName -notmatch '[\\/](build|Debug|Release)[\\/]' } | Select-Object -First 1
if (-not $pins) { throw 'No encuentro pin_mux.c en el proyecto importado.' }
Copy-Item (Join-Path $raiz 'src/pin_mux.c') $pins.FullName -Force
$cfgs = @(Get-ChildItem -Path $dir -Recurse -File -Filter prj.conf |
    Where-Object { $_.FullName -notmatch '[\\/](build|Debug|Release)[\\/]' })
if ($cfgs.Count -eq 0) { throw 'No encuentro prj.conf en el proyecto importado.' }
# Keep the imported project's hardware_init.c, which configures ADC1/console.
# Only add required drivers. Existing project will compile its usual sources.
$add = @(
 'CONFIG_MCUX_COMPONENT_driver.ctimer=y',
 'CONFIG_MCUX_COMPONENT_driver.gpio=y',
 'CONFIG_MCUX_COMPONENT_driver.port=y',
 'CONFIG_MCUX_COMPONENT_driver.lpuart=y',
 'CONFIG_MCUX_COMPONENT_driver.reset=y'
)
foreach ($f in $cfgs) {
 $content = Get-Content $f.FullName -Raw
 foreach ($line in $add) {
   if ($content -notmatch [regex]::Escape($line)) { Add-Content -Path $f.FullName -Value $line }
 }
}
Write-Host 'Codigo instalado. Vuelve a compilar con MCUXpresso for VS Code.' -ForegroundColor Green
Write-Host ('Archivo principal: ' + $main.FullName)
