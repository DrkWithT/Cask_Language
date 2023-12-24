#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

typedef enum cask_lexical_type
{
    CASK_COMMENT_TOKEN,
    CASK_SPACING_TOKEN,
    CASK_KEYWORD_TOKEN,
    CASK_RESERVED_TOKEN,
    CASK_IDENTIFIER_TOKEN,
    CASK_OPERATOR_TOKEN,
    CASK_INTEGER_TOKEN,
    CASK_FLOAT_TOKEN,
    CASK_STRING_TOKEN,
    CASK_COLON_TOKEN,
    CASK_LPAREN_TOKEN,
    CASK_RPAREN_TOKEN,
    CASK_LBRACK_TOKEN,
    CASK_RBRACK_TOKEN,
    CASK_LBRACE_TOKEN,
    CASK_RBRACE_TOKEN,
    CASK_EOF_TOKEN,
} LexicalType;

typedef struct cask_token_t
{
    uint32_t begin;
    uint32_t length;
    LexicalType type;
} Token;

char *token_stringify(const Token *token, const char *source);

#define CASK_TOKEN_PTR_IS_EMPTY(token) (token->length == 0U)
#define CASK_TOKEN_IS_EOF(token) (token->type == CASK_EOF_TOKEN)

#endif