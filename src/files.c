/**
 * @file files.c
 * @author Derek Tan
 * @brief Implements file I/O helper functions.
 * @date 2023-12-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "utils/files.h"

char *file_load(const char *file_path)
{
    FILE *fs = fopen(file_path, CASK_NATIVE_FREAD);
    char *content_buffer = NULL;

    if (!fs)
        return content_buffer;

    fseek(fs, 0, SEEK_END);
    ssize_t content_size = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    if (content_size < 0)
        goto release_fs_point;

    content_buffer = calloc((content_size + 1), sizeof(char));

    if (content_buffer != NULL)
    {
        fread(content_buffer, sizeof(char), content_size, fs);
        
        // An incompletely read buffer is definitely invalid, so I discard it!
        if (ferror(fs))
        {
            free(content_buffer);
            content_buffer = NULL;
        }
    }

    /// @note Cleanup execution point: closes file stream.
release_fs_point:
    fclose(fs);
    fs = NULL;
    return content_buffer;
}

bool file_putblob(const char *file_path, const uint8_t *source, uint32_t size)
{
    FILE *fs = fopen(file_path, CASK_NATIVE_FWRITE_BIN);
    bool status;

    if (!fs)
        return false;
    
    fwrite(source, sizeof(uint8_t), size, fs);

    status = !ferror(fs);

    fclose(fs);

    return status;
}
