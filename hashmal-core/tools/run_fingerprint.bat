@echo off
chcp 65001 > nul
echo.
echo ================================================================
echo HASHMAL-CORE - Tarea 2: Fingerprint 66 libros
echo Ex 31:18 - lujot ha'edut ketuvim be'etsba' Elohim
echo ================================================================
echo.

:: Cambiar al directorio raiz del proyecto
cd /d "%~dp0.."

:: Ejecutar el script Python
python tools\generar_fingerprint_66.py

echo.
if %ERRORLEVEL% EQU 0 (
    echo [OK] Header generado correctamente
    echo      Ahora puedes compilar: idf.py build
) else (
    echo [ERROR] El script fallo con codigo %ERRORLEVEL%
)

echo.
pause
