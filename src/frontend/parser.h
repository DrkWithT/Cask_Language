#ifndef PARSER_H
#define PARSER_H

#include "frontend/lexer.h"
#include "syntax/ast.h"

#define CASK_VALIDATE_TOKEN(token) (token.type >= CASK_COMMENT_TOKEN && token.type <= CASK_EOF_TOKEN)

/* Parser type decls. */

typedef enum parser_error_code_t
{
    CASK_PARSER_ERR_NONE,
    CASK_PARSER_ERR_UNKNOWN_TOKEN,
    CASK_PARSER_ERR_UNEXPECTED_TOKEN,
    CASK_PARSER_ERR_UNCLOSED_BLOCK,
    CASK_PARSER_ERR_GENERAL
} ParserErrorCode;

typedef Expression *(*ExprParseFunc)(void *parser_ptr);

typedef Statement *(*StmtParseFunc)(void *parser_ptr);

/* Parser decl. */

/**
 * @brief A recursive descent parser to convert a sequence of tokens into an AST unit of the program.
 */
typedef struct parser_t
{
    Lexer lexer;
    const StmtParseFunc *stmt_helpers;
    const ExprParseFunc *expr_helpers;
    Token previous;
    Token current;
} Parser;

void parser_init(Parser *parser);

static void parser_reset(Parser *parser);

void parser_use_file(Parser *parser, const char *file_path);

bool parser_advance(Parser *parser);

Token parser_peek_curr(const Parser *parser);

Token parser_peek_back(const Parser *parser);

StmtParseFunc parser_lookup_helper(const Parser *parser);

Expression *parse_expr_special(void *parser_ptr);

Expression *parse_expr_numeric(void *parser_ptr);

Expression *parse_expr_string(void *parser_ptr);

Expression *parse_expr_agg(void *parser_ptr);

Expression *parse_expr_array(void *parser_ptr);

Expression *parse_expr_basic(void *parser_ptr);

Expression *parse_expr_access(void *parser_ptr);

Expression *parse_expr_term(void *parser_ptr);

Expression *parse_expr_factor(void *parser_ptr);

Expression *parse_expr_comparison(void *parser_ptr);

Expression *parse_expr_equality(void *parser_ptr);

Expression *parse_expr_conditional(void *parser_ptr);

Statement *parse_decl_primitive(void *parser_ptr);

Statement *parse_decl_field(void *parser_ptr);

Statement *parse_decl_agg(void *parser_ptr);

Statement *parse_decl_array(void *parser_ptr);

Statement *parse_decl_any(void *parser_ptr);

Statement *parse_decl_func(void *parser_ptr);

Statement *parse_decl_param(void *parser_ptr);

Statement *parse_stmt_import(void *parser_ptr);

Statement *parse_stmt_while(void *parser_ptr);

Statement *parse_stmt_if(void *parser_ptr);

Statement *parse_stmt_else(void *parser_ptr);

Statement *parse_stmt_block(void *parser_ptr);

ProgramUnit *parser_parse(Parser *parser, ParserErrorCode *code_ptr);

#endif