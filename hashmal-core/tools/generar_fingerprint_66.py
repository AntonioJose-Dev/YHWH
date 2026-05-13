#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
generar_fingerprint_66.py
=========================
Tarea 2 — Fase 2: "Fingerprint 66 libros -> SEALED_CORE"

Ex 31:18 — "lujot ha'edut lujot even ketuvim be'etsba' Elohim"
          "Tablas del testimonio, tablas de piedra, escritas por el
           DEDO de Dios."

Dt 19:15 — "al pi shenei edim yaqum davar"
           "Por boca de DOS TESTIGOS se confirma el asunto."

           El fingerprint es UN SOLO valor que testifica de LOS 66.
           Imposible falsificar: cambia un caracter en cualquier libro
           -> todo el fingerprint cambia.

ALGORITMO:
  Para cada libro (66, ordenados 01-66):
    1. Encontrar todos los archivos capitulo del libro
       (patron: "*NN* — *BookName* — Cap *N*.md")
    2. Ordenar por numero de capitulo (numericamente)
    3. Concatenar contenido UTF-8 de todos los capitulos
    4. SHA-256(contenido_concatenado) -> 32 bytes = hash del libro

  -> 66 x 32 bytes = lista de hashes de libros

  MERKLE (Dt 19:15 — dos testigos en cada nivel):
    Pad a 128 hojas (duplicar la ultima hasta completar potencia de 2)
    Nivel 0: las 128 hojas
    Nivel 1: SHA-256(hoja[i] || hoja[i+1]) para i en pasos de 2 -> 64 nodos
    Nivel 2: 32 nodos
    Nivel 3: 16 nodos
    Nivel 4: 8 nodos
    Nivel 5: 4 nodos
    Nivel 6: 2 nodos
    Nivel 7: raiz (1 nodo = 32 bytes)

  EXPANSION (Ex 25:16 — el contenido que YHWH da, incorruptible):
    SHA-512(raiz_merkle) -> 64 bytes = FINGERPRINT_66

  SALIDA:
    edut_fingerprint_66.h  (en components/hashmal_storage/include/)
    Contiene:
      - EDUT_FINGERPRINT_66[64]   <- los 64 bytes
      - EDUT_MERKLE_ROOT_66[32]   <- la raiz merkle de 32 bytes (para SEALED_CORE offset 0)

Uso:
    python tools\generar_fingerprint_66.py

Autor: Antonio Jose Marin Soto
Proyecto: HASHMAL-CORE
"""

import os
import re
import sys
import hashlib
import datetime

# ============================================================
# CONFIGURACION
# ============================================================

# Vault de los 66 libros
VAULT_DIR = r"C:\Users\Antonio José\nucleo-hashmal\Devár-YHWH — La Palabra de YHWH\Devar-YHWH"

# Destino del header generado
OUT_HEADER = r"C:\hashmal-core\hashmal-firmware\components\hashmal_storage\include\edut_fingerprint_66.h"

# Numero de libros
N_LIBROS = 66

# Potencia de 2 para el arbol Merkle
MERKLE_HOJAS = 128  # >= 66

# ============================================================
# HELPERS
# ============================================================

def sha256(data: bytes) -> bytes:
    return hashlib.sha256(data).digest()

def sha512(data: bytes) -> bytes:
    return hashlib.sha512(data).digest()

def merkle_raiz(hojas: list) -> bytes:
    """
    Construye un arbol Merkle balanceado.
    Dt 19:15 — en cada nivel, dos nodos forman uno.
    Las hojas ya estan en lista de 128 (potencia de 2).
    """
    nivel = list(hojas)
    while len(nivel) > 1:
        siguiente = []
        for i in range(0, len(nivel), 2):
            siguiente.append(sha256(nivel[i] + nivel[i + 1]))
        nivel = siguiente
    return nivel[0]

def numero_cap(nombre_archivo: str) -> int:
    """
    Extrae el numero de capitulo del nombre del archivo.
    Ejemplo: "01 - Genesis - Cap 12.md" -> 12
    """
    m = re.search(r'Cap\s+(\d+)', nombre_archivo, re.IGNORECASE)
    if m:
        return int(m.group(1))
    return 0

def es_archivo_capitulo(nombre: str) -> bool:
    """
    Distingue archivos de capitulo (contienen " — Cap " o " - Cap ")
    del archivo indice del libro (solo tiene el nombre del libro).
    """
    return bool(re.search(r'Cap\s+\d+', nombre, re.IGNORECASE))

def hash_libro(carpeta_libro: str, nombre_libro: str) -> bytes:
    """
    Lee todos los capitulos de un libro, los ordena, concatena y devuelve SHA-256.
    """
    archivos = []
    for f in os.listdir(carpeta_libro):
        ruta = os.path.join(carpeta_libro, f)
        if os.path.isfile(ruta) and f.endswith('.md') and es_archivo_capitulo(f):
            archivos.append((numero_cap(f), f, ruta))

    if not archivos:
        print(f"  WARN: {nombre_libro} sin archivos capitulo — usando hash de ceros")
        return bytes(32)

    # Ordenar por numero de capitulo
    archivos.sort(key=lambda x: x[0])

    contenido_total = bytearray()
    for num_cap, nombre_f, ruta_f in archivos:
        try:
            with open(ruta_f, 'r', encoding='utf-8') as fh:
                txt = fh.read()
            contenido_total.extend(txt.encode('utf-8'))
        except Exception as e:
            print(f"  ERROR leyendo {nombre_f}: {e}")

    if not contenido_total:
        print(f"  WARN: {nombre_libro} contenido vacio — usando hash de ceros")
        return bytes(32)

    h = sha256(bytes(contenido_total))
    print(f"  [{nombre_libro}] {len(archivos)} caps, {len(contenido_total)} bytes → {h.hex()[:16]}...")
    return h

# ============================================================
# MAIN
# ============================================================

def main():
    print("=" * 60)
    print("HASHMAL-CORE — Fingerprint 66 libros")
    print("Ex 31:18: lujot ha'edut ketuvim be'etsba' Elohim")
    print("=" * 60)

    if not os.path.isdir(VAULT_DIR):
        print(f"ERROR: Vault no encontrado en: {VAULT_DIR}")
        sys.exit(1)

    # 1. Encontrar las 66 carpetas de libros
    carpetas = []
    for entry in sorted(os.listdir(VAULT_DIR)):
        ruta_entry = os.path.join(VAULT_DIR, entry)
        if os.path.isdir(ruta_entry) and re.match(r'^\d{2}\s+—\s+', entry):
            # Extraer numero de libro
            m = re.match(r'^(\d{2})', entry)
            if m:
                num = int(m.group(1))
                carpetas.append((num, entry, ruta_entry))

    carpetas.sort(key=lambda x: x[0])

    if len(carpetas) != N_LIBROS:
        print(f"WARN: Se esperaban {N_LIBROS} libros, se encontraron {len(carpetas)}")
        if len(carpetas) == 0:
            print("ERROR: Sin libros. Verifica VAULT_DIR.")
            sys.exit(1)

    print(f"\n→ {len(carpetas)} libros encontrados\n")

    # 2. Hash de cada libro
    hashes_libros = []
    for num, nombre, ruta in carpetas:
        h = hash_libro(ruta, nombre)
        hashes_libros.append(h)

    # 3. Pad a MERKLE_HOJAS (duplicar la ultima hoja)
    hojas = list(hashes_libros)
    while len(hojas) < MERKLE_HOJAS:
        hojas.append(hojas[-1])  # duplicar ultima hoja (regla estandar Bitcoin-style)

    print(f"\n→ Arbol Merkle: {len(hojas)} hojas (padding desde {len(hashes_libros)})")

    # 4. Raiz Merkle (32 bytes)
    raiz_merkle = merkle_raiz(hojas)
    print(f"→ Merkle root (32 bytes): {raiz_merkle.hex()}")

    # 5. SHA-512(raiz) = Fingerprint (64 bytes)
    fingerprint = sha512(raiz_merkle)
    print(f"→ Fingerprint SHA-512 (64 bytes): {fingerprint.hex()[:32]}...")

    # 6. Generar header C
    ahora = datetime.datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S UTC")

    def bytes_to_c_array(data: bytes, indent: str = "    ") -> str:
        lines = []
        row = []
        for i, b in enumerate(data):
            row.append(f"0x{b:02X}")
            if len(row) == 8:
                lines.append(indent + ", ".join(row) + ",")
                row = []
        if row:
            lines.append(indent + ", ".join(row))
        return "\n".join(lines)

    header_content = f"""/**
 * edut_fingerprint_66.h
 * =====================
 * GENERADO AUTOMATICAMENTE — NO EDITAR
 *
 * Ex 31:18 — "lujot ha'edut lujot even ketuvim be'etsba' Elohim"
 * "Tablas del testimonio, tablas de piedra, escritas por el DEDO de Dios."
 *
 * Generado: {ahora}
 * Libros procesados: {len(hashes_libros)} / {N_LIBROS}
 * Hojas Merkle: {len(hojas)} (pad desde {len(hashes_libros)})
 *
 * EDUT_MERKLE_ROOT_66[32]: SHA-256 Merkle root de los {len(hashes_libros)} libros
 *   -> Se copia en edut.bytes[EDUT_OFF_MERKLE_ROOT] (offset 0)
 *
 * EDUT_FINGERPRINT_66[64]: SHA-512(MERKLE_ROOT)
 *   -> Se copia en edut.bytes[EDUT_OFF_FINGERPRINT] (offset 44)
 *
 * Merkle root: {raiz_merkle.hex()}
 * Fingerprint: {fingerprint.hex()}
 *
 * Autor: Antonio Jose Marin Soto — generado por generar_fingerprint_66.py
 * Proyecto: HASHMAL-CORE
 */
#ifndef EDUT_FINGERPRINT_66_H
#define EDUT_FINGERPRINT_66_H

#include <stdint.h>

/* ==============================================================
 * MERKLE ROOT — 32 bytes (SHA-256 del arbol de los 66 libros)
 * Dt 19:15: raiz que unifica todos los testimonios.
 * Copiado en edut.bytes[EDUT_OFF_MERKLE_ROOT] = offset 0.
 * ============================================================== */
static const uint8_t EDUT_MERKLE_ROOT_66[32] = {{
{bytes_to_c_array(raiz_merkle)}
}};

/* ==============================================================
 * FINGERPRINT — 64 bytes (SHA-512 del Merkle root)
 * Ex 31:18: el "dedo de Dios" escribe en las tablas.
 * Copiado en edut.bytes[EDUT_OFF_FINGERPRINT] = offset 44.
 * ============================================================== */
static const uint8_t EDUT_FINGERPRINT_66[64] = {{
{bytes_to_c_array(fingerprint)}
}};

/* ==============================================================
 * VERIFICACION — hashes individuales de los {len(hashes_libros)} libros
 * Para auditoria. No se integran en el firmware.
 * ============================================================== */
#ifdef EDUT_FINGERPRINT_AUDIT
static const struct {{ uint8_t h[32]; }} EDUT_HASHES_LIBROS[{len(hashes_libros)}] = {{
"""
    for i, (num, nombre, _) in enumerate(carpetas):
        h = hashes_libros[i]
        header_content += f"    /* {num:02d} {nombre.split(' — ', 1)[1] if ' — ' in nombre else nombre} */\n"
        header_content += f"    {{ {{ {', '.join(f'0x{b:02X}' for b in h)} }} }},\n"

    header_content += f"""}};
#endif /* EDUT_FINGERPRINT_AUDIT */

#endif /* EDUT_FINGERPRINT_66_H */
"""

    # 7. Escribir el header
    os.makedirs(os.path.dirname(OUT_HEADER), exist_ok=True)
    with open(OUT_HEADER, 'w', encoding='utf-8') as fh:
        fh.write(header_content)

    print(f"\n✓ Header generado: {OUT_HEADER}")
    print("\nProximos pasos:")
    print("  1. Recompila el firmware (idf.py build)")
    print("  2. Flashea ambos T-Deck (idf.py -p COMx flash)")
    print("  3. En el primer boot los 256 bytes de la edut se sellan en NVS")
    print("\n¡El Testimonio ha sido escrito por el Dedo de Dios!")

if __name__ == '__main__':
    main()
