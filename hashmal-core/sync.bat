@echo off
chcp 65001 >nul
echo.
echo [1/3] Copiando TAVNIT...
robocopy "C:\Users\Antonio José\Mishkán-YHWH\Mishkán-YHWH\TAVNÍT" "C:\hashmal-core\vault\TAVNIT" /E /NFL /NDL /NJH /NJS

echo [2/3] Copiando CONSTRUCCION...
robocopy "C:\Users\Antonio José\Mishkán-YHWH\Mishkán-YHWH\CONSTRUCCIÓN" "C:\hashmal-core\vault\CONSTRUCCION" /E /NFL /NDL /NJH /NJS

echo [3/3] Copiando archivos raiz...
copy "C:\Users\Antonio José\Mishkán-YHWH\Mishkán-YHWH\ESTADO — Dónde estamos (para continuar).md" "C:\hashmal-core\vault\" >nul 2>&1

echo.
echo LISTO. Origen INTACTO. Todo en C:\hashmal-core\vault\
pause
