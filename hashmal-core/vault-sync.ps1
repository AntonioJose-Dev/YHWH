# vault-sync.ps1
# Copia el vault de Mishkan-YHWH a hashmal-core SIN borrar nada del origen.
# Nombres de CARPETAS sin acentos. Nombres de ARCHIVOS intactos.

$origen  = "C:\Users\Antonio Jose\Mishkan-YHWH\Mishkan-YHWH"
$destino = "C:\hashmal-core\vault"

function Remove-Diacritics {
    param([string]$str)
    $norm = $str.Normalize([System.Text.NormalizationForm]::FormD)
    $sb = [System.Text.StringBuilder]::new()
    foreach ($c in $norm.GetEnumerator()) {
        $cat = [System.Globalization.CharUnicodeInfo]::GetUnicodeCategory($c)
        if ($cat -ne [System.Globalization.UnicodeCategory]::NonSpacingMark) {
            $sb.Append($c) | Out-Null
        }
    }
    return $sb.ToString()
}

function Copy-VaultFolder {
    param([string]$srcFolder, [string]$dstFolder)
    if (-not (Test-Path $srcFolder)) {
        Write-Host "  [SKIP] No existe: $srcFolder"
        return 0
    }
    $archivos = Get-ChildItem -Path $srcFolder -Recurse -File
    $count = 0
    foreach ($f in $archivos) {
        $rel   = $f.FullName.Substring($srcFolder.Length).TrimStart('\')
        $partes = $rel -split '\\'
        $normalized = @()
        for ($i = 0; $i -lt ($partes.Count - 1); $i++) {
            $normalized += Remove-Diacritics $partes[$i]
        }
        $normalized += $partes[-1]
        $rutaDst = Join-Path $dstFolder ($normalized -join '\')
        $dirDst  = Split-Path -Parent $rutaDst
        if (-not (Test-Path $dirDst)) {
            New-Item -ItemType Directory -Path $dirDst -Force | Out-Null
        }
        Copy-Item -Path $f.FullName -Destination $rutaDst -Force
        Write-Host "  OK $rel"
        $count++
    }
    return $count
}

$total = 0

Write-Host ""
Write-Host "[1/4] TAVNIT..."
$src = [System.IO.Path]::Combine($origen.Replace("Antonio Jose", "Antonio Jos" + [char]0xe9), "TAVN" + [char]0xcd + "T")
$total += Copy-VaultFolder $src "$destino\TAVNIT"

Write-Host ""
Write-Host "[2/4] CONSTRUCCION..."
$src2 = [System.IO.Path]::Combine($origen.Replace("Antonio Jose", "Antonio Jos" + [char]0xe9), "CONSTRUCCI" + [char]0xd3 + "N")
$total += Copy-VaultFolder $src2 "$destino\CONSTRUCCION"

Write-Host ""
Write-Host "[3/4] ANADIDURAS (raiz)..."
$src3 = [System.IO.Path]::Combine($origen.Replace("Antonio Jose", "Antonio Jos" + [char]0xe9), [char]0xd1 + "ADIDURAS")
$total += Copy-VaultFolder $src3 "$destino\ANADIDURAS"

Write-Host ""
Write-Host "[4/4] Archivos raiz..."
$origenReal = $origen.Replace("Antonio Jose", "Antonio Jos" + [char]0xe9)
$raiz = Get-ChildItem -Path $origenReal -File | Where-Object { $_.Extension -in '.md','.txt' }
foreach ($f in $raiz) {
    $dst = Join-Path $destino $f.Name
    Copy-Item -Path $f.FullName -Destination $dst -Force
    Write-Host "  OK $($f.Name)"
    $total++
}

Write-Host ""
Write-Host "TOTAL: $total archivos copiados"
Write-Host "Destino: $destino"
Write-Host "Origen INTACTO."
