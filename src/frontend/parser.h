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

typedef Statement *(*StmtParseFunc)(void *parser_ptr);

/* Parser decl. */

/**
 * @brief A recursive descent parser to convert a sequence of tokens into an AST unit of the program.
 */
typedef struct parser_t
{
    Lexer lexer;
    const StmtParseFunc *stmt_helpers;
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

Expression *parse_expr_special(Parser *parser_ptr);

Expression *parse_expr_numeric(Parser *parser_ptr);

Expression *parse_expr_string(Parser *parser_ptr);

Expression *parse_expr_agg(Parser *parser_ptr);

Expression *parse_expr_array(Parser *parser_ptr);

Expression *parse_expr_call(Parser *parser_ptr);

Expression *parse_expr_basic(Parser *parser_ptr);

Expression *parse_expr_access(Parser *parser_ptr);

Expression *parse_expr_term(Parser *parser_ptr);

Expression *parse_expr_factor(Parser *parser_ptr);

Expression *parse_expr_comparison(Parser *parser_ptr);

Expression *parse_expr_equality(Parser *parser_ptr);

Expression *parse_expr_conditional(Parser *parser_ptr);

Statement *parse_decl_primitive(void *parser_ptr);

Statement *parse_decl_field(void *parser_ptr);

Statement *parse_decl_aggregate(void *parser_ptr);

Statement *parse_decl_array(void *parser_ptr);

Statement *parse_decl_any_value(void *parser_ptr);

Statement *parse_decl_func(void *parser_ptr);

Statement *parse_decl_param(void *parser_ptr);

Statement *parse_stmt_import(void *parser_ptr);

Statement *parse_stmt_reassign(void *parser_ptr);

Statement *parse_stmt_while(void *parser_ptr);

Statement *parse_stmt_if(void *parser_ptr);

Statement *parse_stmt_else(void *parser_ptr);

Statement *parse_stmt_block(void *parser_ptr);

ProgramUnit *parser_parse(Parser *parser, ParserErrorCode *code_ptr);

#endif