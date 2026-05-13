$a = [char]225
$I = [char]205
$O = [char]211
$N = [char]209

$base   = $env:USERPROFILE + "\Mishk" + $a + "n-YHWH\Mishk" + $a + "n-YHWH"
$dst    = "C:\hashmal-core\vault"

$tavnit = $base + "\TAVN" + $I + "T"
$const  = $base + "\CONSTRUCCI" + $O + "N"
$anadi  = $base + "\A" + $N + "ADIDURAS"
$estado = $base + "\ESTADO " + [char]8212 + " D" + [char]243 + "nde estamos (para continuar).md"

if (-not (Test-Path $dst)) { New-Item -ItemType Directory -Path $dst -Force | Out-Null }

Write-Host "[1/4] TAVNIT..." -ForegroundColor Cyan
robocopy $tavnit "$dst\TAVNIT" /E /NJH /NJS

Write-Host "[2/4] CONSTRUCCION..." -ForegroundColor Cyan
robocopy $const "$dst\CONSTRUCCION" /E /NJH /NJS

Write-Host "[3/4] ANADIDURAS..." -ForegroundColor Cyan
robocopy $anadi "$dst\ANADIDURAS" /E /NJH /NJS

Write-Host "[4/4] Archivo de estado..." -ForegroundColor Cyan
if (Test-Path $estado) {
    Copy-Item $estado "$dst\" -Force
    Write-Host "  OK"
} else {
    Write-Host "  No encontrado: $estado"
}

Write-Host ""
Write-Host "LISTO. Origen intacto. Todo en $dst" -ForegroundColor Green
