# HASHMAL-FIRMWARE — Proyecto raíz ESP-IDF
# "Conforme a todo lo que Yo te muestro — así lo harás." — Éxodo 25:9
#
# Plataforma: ESP32-S3
# Oro de Egipto: ESP-IDF HAL + WiFi/BLE stacks + mbedTLS + FreeRTOS
# Todo lo demás: NUESTRO, según el Tavnít

cmake_minimum_required(VERSION 3.16)

# Incluir todos los componentes del Mishkán
set(EXTRA_COMPONENT_DIRS "components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(hashmal-firmware)
