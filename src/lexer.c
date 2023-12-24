/**
 * @file lexer.c
 * @author Derek Tan
 * @brief Implements the Cask lexer.
 * @date 2023-12-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <string.h>
#include "frontend/lexer.h"

/* Lexer char predicate impl. */

bool predicate_is_spacing(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool predicate_is_numeric(char c)
{
    return c >= '0' && c <= '9';
}

bool predicate_is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool predicate_is_alphanum(char c)
{
    return predicate_is_alpha(c) || predicate_is_numeric(c);
}

bool predicate_is_symbolic(char c)
{
    return c == '=' || c == '!' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*' || c == '/';
}

/* Lexer impl. */

void lexer_init(Lexer *lexer, const char *source_cstr)
{
    lexer->source_view = source_cstr;
    lexer->source_index = 0U;
    lexer->source_length = strlen(source_cstr);
}

Token lexer_lex_single(Lexer *lexer, LexicalType lexical_type)
{
    uint32_t token_position = lexer->source_index;
    uint32_t token_length = 1U;

    lexer->source_index++;

    return (Token){.begin = token_position, .length = token_length, .type = lexical_type};
}

Token lexer_lex_until(Lexer *lexer, LexicalType lexical_type, char terminator)
{
    /// @note skip starting terminator to avoid infinite loop!
    lexer->source_index++;

    uint32_t token_position = lexer->source_index;
    uint32_t token_length = 0U;

    while (lexer->source_index < lexer->source_length)
    {
        if (lexer_peek(lexer) == terminator)
        {
            lexer->source_index++;
            break;
        }

        lexer->source_index++;
        token_length++;
    }

    return (Token){.begin = token_position, .length = token_length, .type = lexical_type};
}

Token lexer_lex_by(Lexer *lexer, LexicalType lexical_type, LexerPredicate predicate)
{
    uint32_t token_position = lexer->source_index;
    uint32_t token_length = 0U;

    while (lexer->source_index < lexer->source_length)
    {
        if (!predicate(lexer_peek(lexer)))
            break;

        lexer->source_index++;
        token_length++;
    }

    return (Token){.begin = token_position, .length = token_length, .type = lexical_type};
}

Token lexer_yield_token(Lexer *lexer)
{
    if (lexer->source_index >= lexer->source_length)
    {
        return (Token){.begin = lexer->source_index, .length = 1U, .type = CASK_EOF_TOKEN};
    }

    char temp = lexer_peek(lexer);

    switch (temp)
    {
    case '.':
        return lexer_lex_single(lexer, CASK_DOT_TOKEN);
    case ',':
        return lexer_lex_single(lexer, CASK_COMMA_TOKEN);
    case ':':
        return lexer_lex_single(lexer, CASK_COLON_TOKEN);
    case '(':
        return lexer_lex_single(lexer, CASK_LPAREN_TOKEN);
    case ')':
        return lexer_lex_single(lexer, CASK_RPAREN_TOKEN);
    case '[':
        return lexer_lex_single(lexer, CASK_LBRACK_TOKEN);
    case ']':
        return lexer_lex_single(lexer, CASK_RBRACK_TOKEN);
    case '{':
        return lexer_lex_single(lexer, CASK_LBRACE_TOKEN);
    case '}':
        return lexer_lex_single(lexer, CASK_RBRACE_TOKEN);
    default:
        if (predicate_is_spacing(temp))
            return lexer_lex_by(lexer, CASK_SPACING_TOKEN, predicate_is_spacing);
        if (temp == '#')
            return lexer_lex_until(lexer, CASK_COMMENT_TOKEN, temp);
        if (predicate_is_alpha(temp))
            return lexer_lex_by(lexer, CASK_RESERVED_TOKEN, predicate_is_alpha);
        if (predicate_is_numeric(temp))
            return lexer_lex_by(lexer, CASK_DIGITS_TOKEN, predicate_is_numeric(temp));
        if (predicate_is_symbolic(temp))
            return lexer_lex_by(lexer, CASK_OPERATOR_TOKEN, predicate_is_symbolic);
        break;
    }

    return (Token){.begin = lexer->source_index, .length = 1U, .type = CASK_UNKNOWN_TOKEN};
}

// void lexer_reset(Lexer *lexer, char *source_cstr);
