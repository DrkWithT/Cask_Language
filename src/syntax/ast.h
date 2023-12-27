#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stdint.h>

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
            void *value; /// @todo add GenericArray in node??
        } array;
        
        struct
        {
            void *members;
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
            ;
        } aggr_decl;

        struct
        {
            char *name;
            void *param_list;
            struct cask_stmt_t *body;
        } func_decl;
        
        struct
        {
            char *name;
            Expression *value;
        } param_decl;

        struct
        {
            Expression *condition;
            struct cask_stmt_t *body;
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
            void *body_list;
        } block;
    } contents;
    
    StatementType type;
} Statement;

typedef struct cask_ast_node_t
{
    void *data;
    bool is_stmt;
} ASTNode;

/// @todo Create helper functions to manage memory, add, delete, set, etc.

#endif