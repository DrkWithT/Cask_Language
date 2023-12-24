#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdint.h>

#include "frontend/token.h"

/**
 * @brief Defines a character testing function for the Lexer to classify a character's token category.
 */
typedef bool (*LexerPredicate)(char c);

bool predicate_is_spacing(char c);

bool predicate_is_numeric(char c);

bool predicate_is_alpha(char c);

bool predicate_is_alphanum(char c);

bool predicate_is_symbol(char c);

/**
 * @brief Encapsulates Lexer state. The state tracks the relative index and source length.
 */
typedef struct lexer_t
{
    char *source_view;
    uint32_t source_index;
    uint32_t source_length;
} Lexer;

/**
 * @brief Initializes the Lexer with a handle to an allocated c-string of a loaded file. See `src/files.c` for file loader.
 * 
 * @param lexer 
 * @param source_cstr 
 */
void lexer_init(Lexer *lexer, char *source_cstr);

/**
 * @brief Helper function that scans a portion of text into a tagged token based on a predicate. Otherwise if the character is a special symbol letter, yield a special symbol token. See `LexerPredicate` for predicate details.
 * 
 * @param lexer 
 * @param predicate 
 * @param is_single If the token has one character only.
 * @return Token 
 */
Token lexer_lex_by(Lexer *lexer, LexerPredicate predicate, bool is_single);

/**
 * @brief Main lexer function. This will run the lexer on a course through a lexeme and yield a token per call. The final possible output must be an EOF `Token`.
 * 
 * @param lexer 
 * @return Token 
 */
Token lexer_yield_token(Lexer *lexer);

/**
 * @brief Resets the parser with a new source pointer and new indexing state.
 * 
 * @param lexer 
 * @param source_cstr 
 */
void lexer_reset(Lexer *lexer, char *source_cstr);

#endif