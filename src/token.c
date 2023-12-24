/**
 * @file token.c
 * @author Derek Tan
 * @brief Implements Token object for Cask.
 * @date 2023-12-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "frontend/token.h"

char *token_stringify(const Token *token, const char *source)
{
    if (!token)
        return NULL;

    if (CASK_TOKEN_PTR_IS_EMPTY(token) || CASK_TOKEN_IS_EOF(token))
        return NULL;

    uint32_t buffer_size = token->length + 1U;
    uint32_t buffer_pos = token->begin;
    char *lexeme_buffer = calloc(buffer_size, sizeof(char));

    if (lexeme_buffer != NULL)
    {
        strncpy(lexeme_buffer, source + buffer_pos, buffer_size - 1U);
    }

    return lexeme_buffer;
}
