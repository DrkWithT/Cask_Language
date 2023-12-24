#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stdint.h>

#define CASK_NATIVE_FWRITE "w"
#define CASK_NATIVE_FREAD "r"
#define CASK_NATIVE_FWRITE_BIN "wb"
#define CASK_NATIVE_FREAD_BIN "rb"

char *file_load(const char *file_path);

bool file_putblob(const char *file_path, const uint8_t *source, uint32_t size);

#endif