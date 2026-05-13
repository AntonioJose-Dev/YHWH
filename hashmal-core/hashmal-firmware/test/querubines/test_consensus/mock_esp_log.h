/*
 * mock_esp_log.h
 * Mock of ESP-IDF's esp_log.h so the Querubines tests compile without
 * ESP-IDF installed. Logs go through printf instead of the real logger.
 */
#ifndef MOCK_ESP_LOG_H
#define MOCK_ESP_LOG_H

#include <stdio.h>
#include <stdint.h>

/* Neutralize the real esp_log.h if it ever gets on the include path.
 * These are the common include-guard spellings used across ESP-IDF
 * versions; defining them up front makes any later real include a no-op. */
#ifndef __ESP_LOG_H__
#define __ESP_LOG_H__
#endif
#ifndef _ESP_LOG_H_
#define _ESP_LOG_H_
#endif
#ifndef ESP_LOG_H_
#define ESP_LOG_H_
#endif
#ifndef INC_ESP_LOG_H
#define INC_ESP_LOG_H
#endif

/* Log levels matching the real enum values in ESP-IDF. */
typedef enum {
    ESP_LOG_NONE    = 0,
    ESP_LOG_ERROR   = 1,
    ESP_LOG_WARN    = 2,
    ESP_LOG_INFO    = 3,
    ESP_LOG_DEBUG   = 4,
    ESP_LOG_VERBOSE = 5
} esp_log_level_t;

/* Variadic log macros. Accept (tag, fmt, ...) just like the real ones.
 * ##__VA_ARGS__ swallows a leading comma when no arguments are passed. */
#define ESP_LOGE(tag, fmt, ...)  do { printf("E (%s) " fmt "\n", (tag), ##__VA_ARGS__); } while (0)
#define ESP_LOGW(tag, fmt, ...)  do { printf("W (%s) " fmt "\n", (tag), ##__VA_ARGS__); } while (0)
#define ESP_LOGI(tag, fmt, ...)  do { printf("I (%s) " fmt "\n", (tag), ##__VA_ARGS__); } while (0)
#define ESP_LOGD(tag, fmt, ...)  do { printf("D (%s) " fmt "\n", (tag), ##__VA_ARGS__); } while (0)
#define ESP_LOGV(tag, fmt, ...)  do { printf("V (%s) " fmt "\n", (tag), ##__VA_ARGS__); } while (0)

/* No-op level control (the real header exposes these; some code calls them). */
static inline void esp_log_level_set(const char *tag, esp_log_level_t level) {
    (void)tag; (void)level;
}

#endif /* MOCK_ESP_LOG_H */
