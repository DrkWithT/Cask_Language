/**
 * @file ast.c
 * @author Derek Tan
 * @brief Implements AST interface and helper functions.
 * @todo Implement import AST node.
 * @todo Implement ProgramUnit funcs.
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "syntax/ast.h"

/* AST Vector impls. */

CASK_FN_VECTOR_INIT(Expression)

CASK_FN_VECTOR_GET_PTR(Expression)

CASK_FN_VECTOR_APPEND(Expression)

CASK_FN_VECTOR_DISPOSE(Expression)

CASK_FN_VECTOR_INIT(Statement)

CASK_FN_VECTOR_GET_PTR(Statement)

CASK_FN_VECTOR_APPEND(Statement)

CASK_FN_VECTOR_DISPOSE(Statement)

/* Expression impls. */

void expression_init_special_ltrl(Expression *expr, bool is_nil, bool bool_flag)
{
    expr->contents.special.boolean = bool_flag;
    expr->contents.special.is_nil = is_nil;
    expr->type = CASK_EXPR_LITERAL_SPECIAL;
    expr->is_lvalue = false;
}

void expression_init_integer_ltrl(Expression *expr, int32_t value)
{
    expr->contents.integer.value = value;
    expr->type = CASK_EXPR_LITERAL_INTEGER;
    expr->is_lvalue = false;
}

void expression_init_realnum_ltrl(Expression *expr, float value)
{
    expr->contents.realnum.value = value;
    expr->type = CASK_EXPR_LITERAL_FLOAT;
    expr->is_lvalue = false;
}

void expression_init_array_ltrl(Expression *expr)
{
    vector_init_Expression(&expr->contents.array.values);
    expr->type = CASK_EXPR_LITERAL_ARRAY;
    expr->is_lvalue = false;
}

void expression_init_aggr_ltrl(Expression *expr)
{
    vector_init_Expression(&expr->contents.aggregate.literals);
    expr->type = CASK_EXPR_LITERAL_AGGREGATE;
    expr->is_lvalue = false;
}

void expression_init_identifier_ltrl(Expression *expr, char *name)
{
    expr->contents.identifier.name = name;
    expr->type = CASK_EXPR_IDENTIFIER;
}

void expression_init_access(Expression *expr, Expression *target, Expression *key, bool has_aggr)
{
    expr->contents.access.target = target;
    expr->contents.access.key = key;
    expr->contents.access.has_aggr = has_aggr;
    expr->type = CASK_EXPR_ACCESS;
}

void expression_init_term(Expression *expr, Expression *left, Expression *right, char op)
{
    expr->contents.term.left = left;
    expr->contents.term.right = right;
    expr->contents.term.op = op;
    expr->type = CASK_EXPR_TERM;
    expr->is_lvalue = false;
}

void expression_init_factor(Expression *expr, Expression *left, Expression *right, char op)
{
    expr->contents.factor.left = left;
    expr->contents.factor.right = right;
    expr->contents.factor.op = op;
    expr->type = CASK_EXPR_FACTOR;
    expr->is_lvalue = false;
}

void expression_init_comparison(Expression *expr, Expression *left, Expression *right, char op)
{
    expr->contents.comparison.left = left;
    expr->contents.comparison.right = right;
    expr->contents.comparison.op = op;
    expr->type = CASK_EXPR_COMPARISON;
    expr->is_lvalue = false;
}

void expression_init_equality(Expression *expr, Expression *left, Expression *right, char op)
{
    expr->contents.equality.left = left;
    expr->contents.equality.right = right;
    expr->contents.equality.op = op;
    expr->type = CASK_EXPR_EQUALITY;
    expr->is_lvalue = false;
}

void expression_init_conditional(Expression *expr, Expression *left, Expression *right, char op)
{
    expr->contents.conditional.left = left;
    expr->contents.conditional.right = right;
    expr->contents.conditional.op = op;
    expr->type = CASK_EXPR_CONDITIONAL;
    expr->is_lvalue = false;
}

bool expression_append_child(Expression *expr, ExpressionType type, Expression *child)
{
    if (type == CASK_EXPR_LITERAL_ARRAY)
        return vector_append_Expression(&expr->contents.array.values, child);

    if (type == CASK_EXPR_LITERAL_AGGREGATE)
        return vector_append_Expression(&expr->contents.aggregate.literals, child);

    return false;
}

void expression_dispose(Expression *expr)
{
    switch (expr->type)
    {
    case CASK_EXPR_LITERAL_SPECIAL:
    case CASK_EXPR_LITERAL_INTEGER:
    case CASK_EXPR_LITERAL_FLOAT:
    case CASK_EXPR_LITERAL_STRING:
        free(expr);
        break;
    case CASK_EXPR_LITERAL_ARRAY:
        vector_dispose_Expression(&expr->contents.array.values, expression_dispose); /// @note Ignore inner memory since it will be reclaimed by the Cask VM.
        break;
    case CASK_EXPR_LITERAL_AGGREGATE:
        vector_dispose_Expression(&expr->contents.aggregate.literals, expression_dispose);
        break;
    case CASK_EXPR_ACCESS:
        expression_dispose(expr->contents.access.key);
        expression_dispose(expr->contents.access.target);
        break;
    case CASK_EXPR_TERM:
        expression_dispose(expr->contents.term.left);
        expression_dispose(expr->contents.term.right);
        break;
    case CASK_EXPR_FACTOR:
        expression_dispose(expr->contents.factor.left);
        expression_dispose(expr->contents.factor.right);
        break;
    case CASK_EXPR_COMPARISON:
        expression_dispose(expr->contents.comparison.left);
        expression_dispose(expr->contents.comparison.right);
        break;
    case CASK_EXPR_EQUALITY:
        expression_dispose(expr->contents.equality.left);
        expression_dispose(expr->contents.equality.right);
        break;
    case CASK_EXPR_CONDITIONAL:
        expression_dispose(expr->contents.conditional.left);
        expression_dispose(expr->contents.conditional.right);
        break;
    default:
        break;
    }
}

/* Statement impl. */

void statement_init_import(Statement *stmt, char *module_name)
{
    vector_init_Expression(&stmt->contents.import.items);
    stmt->type = CASK_STMT_IMPORT;
}

void statement_init_prim_decl(Statement *stmt, char *name, Expression *value, CompositeType high_type, DataType low_type)
{
    stmt->contents.prim_decl.name = name;
    stmt->contents.prim_decl.value = value;
    stmt->contents.prim_decl.type_mask = (high_type << 8) + low_type;
    stmt->type = CASK_STMT_PRIMITIVE_DECL;
}

void statement_init_field_decl(Statement *stmt, char *name, CompositeType high_type, DataType lower_type)
{
    stmt->contents.field_decl.name = name;
    stmt->contents.field_decl.type_mask = (high_type << 8) + lower_type;
    stmt->type = CASK_STMT_FIELD_DECL;
}

void statement_init_aggr_decl(Statement *stmt, char *name)
{
    vector_init_Statement(&stmt->contents.aggr_decl.members);
    stmt->contents.aggr_decl.name = name;
    stmt->type = CASK_STMT_AGGREGATE_DECL;
}

void statement_init_func_decl(Statement *stmt, char *name, Statement *block)
{
    vector_init_Statement(&stmt->contents.func_decl.params);
    stmt->contents.func_decl.block = block;
    stmt->contents.func_decl.name = name;
    stmt->type = CASK_STMT_FUNCTION_DECL;
}

void statement_init_param_decl(Statement *stmt, char *name, CompositeType high_type, DataType low_type)
{
    stmt->contents.param_decl.name = name;
    stmt->contents.param_decl.type_mask = (high_type << 8) + low_type;
    stmt->type = CASK_STMT_PARAMETER_DECL;
}

void statement_init_while_ctrl(Statement *stmt, Expression *conditional, Statement *block)
{
    stmt->contents.while_ctrl.block = block;
    stmt->contents.while_ctrl.condition = conditional;
    stmt->type = CASK_STMT_WHILE;
}

void statement_init_if_ctrl(Statement *stmt, Expression *conditional, Statement *block)
{
    stmt->contents.if_ctrl.block = block;
    stmt->contents.if_ctrl.condition = conditional;
    stmt->type = CASK_STMT_IF;
}

void statement_init_else_ctrl(Statement *stmt, Statement *block)
{
    stmt->contents.else_ctrl.block = block;
    vector_init_Statement(stmt->contents.else_ctrl.block);
    stmt->type = CASK_STMT_ELSE;
}

void statement_init_block(Statement *stmt)
{
    vector_init_Statement(&stmt->contents.block.stmts);
    stmt->type = CASK_STMT_BLOCK;
}

bool statement_append_child(Statement *stmt, Statement *child)
{
    if (stmt->type == CASK_STMT_AGGREGATE_DECL)
        return vector_append_Statement(&stmt->contents.aggr_decl.members, child);

    if (stmt->type == CASK_STMT_FUNCTION_DECL && child->type == CASK_STMT_PARAMETER_DECL)
        return vector_append_Statement(&stmt->contents.func_decl.params, child);
    
    if (stmt->type == CASK_STMT_BLOCK)
        return vector_append_Statement(&stmt->contents.block.stmts, child);

    return false;
}

uint8_t statement_decode_comptype(const Statement *stmt)
{
    if (stmt->type == CASK_STMT_PRIMITIVE_DECL)
    {
        return (stmt->contents.prim_decl.type_mask & (0xff << 8)) >> 8;
    }

    if (stmt->type == CASK_STMT_PARAMETER_DECL)
    {
        return (stmt->contents.param_decl.type_mask & (0xff << 8)) >> 8;
    }

    return CASK_COMPTYPE_UNKNOWN;
}

uint8_t statement_decode_datatype(const Statement *stmt)
{
    if (stmt->type == CASK_STMT_PRIMITIVE_DECL)
    {
        return (stmt->contents.prim_decl.type_mask & 0xff);
    }

    if (stmt->type == CASK_STMT_PARAMETER_DECL)
    {
        return (stmt->contents.param_decl.type_mask & 0xff);
    }

    return CASK_COMPTYPE_UNKNOWN;
}

void statement_dispose(Statement *stmt)
{
    /// @note Only free outer container memory rather than deepest nested data since the VM will own them!
    switch (stmt->type)
    {
    case CASK_STMT_PRIMITIVE_DECL:
    case CASK_STMT_FIELD_DECL:
    case CASK_STMT_PARAMETER_DECL:
        break;
    case CASK_STMT_AGGREGATE_DECL:
        vector_dispose_Statement(&stmt->contents.aggr_decl.members, NULL);
        break;
    case CASK_STMT_FUNCTION_DECL:
        vector_dispose_Statement(&stmt->contents.func_decl.params, NULL);
        statement_dispose(stmt->contents.func_decl.block);
        break;
    case CASK_STMT_WHILE:
        statement_dispose(stmt->contents.while_ctrl.block);
        expression_dispose(stmt->contents.while_ctrl.condition);
        break;
    case CASK_STMT_IF:
        statement_dispose(stmt->contents.if_ctrl.block);
        expression_dispose(stmt->contents.if_ctrl.condition);
        break;
    case CASK_STMT_ELSE:
        statement_dispose(stmt->contents.else_ctrl.block);
        break;
    case CASK_STMT_BLOCK:
        vector_dispose_Statement(&stmt->contents.block.stmts, NULL);
        break;
    default:
        break;
    }
}

/* ProgramUnit impl. */

void program_unit_init(ProgramUnit *prog_unit, char *name)
{
    vector_init_Statement(&prog_unit->statements);
    prog_unit->name = name;
}

const char *program_unit_view_name(const ProgramUnit *prog_unit)
{
    return prog_unit->name;
}

bool program_unit_append(ProgramUnit *prog_unit, Statement *stmt_ptr)
{
    return vector_append_Statement(&prog_unit->statements, stmt_ptr);
}

void program_unit_dispose(ProgramUnit *prog_unit)
{
    vector_dispose_Statement(&prog_unit->statements, statement_dispose);

    if (prog_unit->name != NULL)
    {
        free(prog_unit->name);
        prog_unit->name = NULL;
    }
}
