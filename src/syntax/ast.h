#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

#include "collection/vectors.h"

/* AST type codes */

typedef enum cask_datatype_e
{
    CASK_DATATYPE_NIL,
    CASK_DATATYPE_INTEGER,
    CASK_DATATYPE_FLOAT,
    CASK_DATATYPE_STRING
} DataType;

typedef enum cask_comptype_e
{
    CASK_COMPTYPE_SINGLE,
    CASK_COMPTYPE_AGGREGATE,
    CASK_COMPTYPE_ARRAY
} CompositeType;

typedef enum cask_expr_type_e
{
    CASK_EXPR_LITERAL_SPECIAL,  // nil, true, false 
    CASK_EXPR_LITERAL_INTEGER,
    CASK_EXPR_LITERAL_FLOAT,
    CASK_EXPR_LITERAL_STRING,
    CASK_EXPR_LITERAL_ARRAY,
    CASK_EXPR_LITERAL_AGGREGATE,
    CASK_EXPR_IDENTIFIER,
    CASK_EXPR_ACCESS,
    CASK_EXPR_TERM,
    CASK_EXPR_FACTOR,
    CASK_EXPR_COMPARISON,
    CASK_EXPR_EQUALITY,
    CASK_EXPR_CONDITIONAL
} ExpressionType;

typedef enum cask_stmt_type_e
{
    CASK_STMT_PRIMITIVE_DECL,
    CASK_STMT_AGGREGATE_DECL,
    CASK_STMT_FUNCTION_DECL,
    CASK_STMT_PARAMETER_DECL,
    CASK_STMT_WHILE,
    CASK_STMT_IF,
    CASK_STMT_ELSE,
    CASK_STMT_BLOCK
} StatementType;

/* AST structures */

typedef struct cask_expr_t
{
    union
    {
        struct
        {
            bool is_nil;
            bool boolean;
        } special;

        struct
        {
            int32_t value;
        } integer;

        struct
        {
            float value;
        } realnum;
        
        struct
        {
            char *value;
        } string;
        
        struct
        {
            Vector values;
        } array;
        
        struct
        {
            Vector literals;
        } aggregate;

        struct
        {
            char *value;
        } identifier;

        struct
        {
            struct cask_expr_t *target;
            struct cask_expr_t *key;
            bool uses_member;
        } access;

        struct
        {
            struct cask_expr_t *left;
            struct cask_expr_t *right;
            char op;
        } term;

        struct
        {
            struct cask_expr_t *left;
            struct cask_expr_t *right;
            char op;
        } factor;

        struct
        {
            struct cask_expr_t *left;
            struct cask_expr_t *right;
            char op;
        } comparison;

        struct
        {
            struct cask_expr_t *left;
            struct cask_expr_t *right;
            char op;
        } equality;

        struct
        {
            struct cask_expr_t *left;
            struct cask_expr_t *right;
            char op;
        } conditional;
    } contents;

    ExpressionType type;
    bool is_lvalue;
} Expression;

void expression_init_special_ltrl(Expression *expr, bool is_nil, bool bool_flag);
void expression_init_integer_ltrl(Expression *expr, int32_t value);
void expression_init_realnum_ltrl(Expression *expr, float value);
void expression_init_array_ltrl(Expression *expr);
void expression_init_aggr_ltrl(Expression *expr);
void expression_init_identifier_ltrl(Expression *expr, char *name);
void expression_init_term(Expression *expr, Expression *left, Expression *right, char op);
void expression_init_factor(Expression *expr, Expression *left, Expression *right, char op);
void expression_init_comparison(Expression *expr, Expression *left, Expression *right, char op);
void expression_init_equality(Expression *expr, Expression *left, Expression *right, char op);
void expression_init_conditional(Expression *expr, Expression *left, Expression *right, char op);
bool expression_append_child(Expression *expr, ExpressionType type, Expression *child);
void expression_dispose(Expression *expr, ExpressionType type);

typedef struct cask_stmt_t
{
    union
    {
        struct
        {
            char *name;
            Expression *value;
            uint16_t type_mask; // CompositeType : uint8_t, DataType : uint8_t
        } prim_decl;

        struct
        {
            char *name;
            Vector members;
        } aggr_decl;

        struct
        {
            char *name;
            Vector params;
            Vector stmt_body;
        } func_decl;
        
        struct
        {
            char *name;
            uint16_t type_mask;
        } param_decl;

        struct
        {
            Expression *condition;
            struct cask_stmt_t *block;
        } while_ctrl;
        
        struct
        {
            Expression *condition;
            struct cask_stmt_t *block;
        } if_ctrl;

        struct
        {
            struct cask_stmt_t *block;
        } else_ctrl;

        struct
        {
            Vector stmts;
        } block;
    } contents;
    
    StatementType type;
} Statement;

void statement_init_prim_decl(Statement *stmt, char *name, Expression *value, CompositeType high_type, DataType low_type);
void statement_init_aggr_decl(Statement *stmt, char *name);
void statement_init_func_decl(Statement *stmt, char *name);
void statement_init_param_decl(Statement *stmt, char *name, CompositeType high_type, DataType low_type);
void statement_init_while_ctrl(Statement *stmt, Expression *conditional);
void statement_init_if_ctrl(Statement *stmt, Expression *conditional);
void statement_init_else_ctrl(Statement *stmt);
bool statement_append_child(Statement *stmt, StatementType type, Statement *child);
void statement_dispose(Statement *stmt, StatementType type);

typedef struct cask_program_unit_t
{
    Vector statements;
    char *name;
} ProgramUnit;

void program_unit_init(ProgramUnit *prog_unit);
const char *program_unit_view_name(const ProgramUnit *punit);
bool program_unit_append(ProgramUnit *prog_unit, Statement *stmt_ptr);
void program_unit_dispose(ProgramUnit *prog_unit);

#endif