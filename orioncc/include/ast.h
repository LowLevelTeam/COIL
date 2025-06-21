#ifndef ORION_AST_H
#define ORION_AST_H

#include "common.h"
#include "lexer.h"

// Forward declaration
struct ASTNode;

// AST Node types
typedef enum {
  // Program structure
  AST_PROGRAM,
  AST_FUNCTION,
  AST_PARAMETER,
  
  // Statements
  AST_COMPOUND_STMT,
  AST_IF_STMT,
  AST_WHILE_STMT,
  AST_FOR_STMT,
  AST_RETURN_STMT,
  AST_BREAK_STMT,
  AST_CONTINUE_STMT,
  AST_EXPRESSION_STMT,
  AST_VAR_DECL,
  
  // Expressions
  AST_ASSIGN_EXPR,
  AST_BINARY_EXPR,
  AST_UNARY_EXPR,
  AST_CALL_EXPR,
  AST_VAR_REF,
  AST_INTEGER_LITERAL,
  AST_ARRAY_ACCESS,
  AST_POST_INCREMENT,
  AST_POST_DECREMENT
} ASTNodeType;

// Type information
typedef enum {
  TYPE_VOID,
  TYPE_INT,
  TYPE_INT_ARRAY,
  TYPE_UNKNOWN
} DataType;

typedef struct TypeInfo {
  DataType base_type;
  int array_size;  // -1 for unknown size, 0 for non-array
  bool is_const;
} TypeInfo;

// AST Node structure
typedef struct ASTNode {
  ASTNodeType type;
  SourcePosition pos;
  TypeInfo type_info;
  
  union {
    // Program
    struct {
      struct ASTNode **functions;
      size_t function_count;
      size_t function_capacity;
    } program;
    
    // Function
    struct {
      char *name;
      struct ASTNode **parameters;
      size_t param_count;
      size_t param_capacity;
      struct ASTNode *body;
      TypeInfo return_type;
    } function;
    
    // Parameter
    struct {
      char *name;
      TypeInfo param_type;
    } parameter;
    
    // Compound statement
    struct {
      struct ASTNode **statements;
      size_t stmt_count;
      size_t stmt_capacity;
    } compound;
    
    // If statement
    struct {
      struct ASTNode *condition;
      struct ASTNode *then_stmt;
      struct ASTNode *else_stmt;
    } if_stmt;
    
    // While statement
    struct {
      struct ASTNode *condition;
      struct ASTNode *body;
    } while_stmt;
    
    // For statement
    struct {
      struct ASTNode *init;
      struct ASTNode *condition;
      struct ASTNode *update;
      struct ASTNode *body;
    } for_stmt;
    
    // Return statement
    struct {
      struct ASTNode *expression;
    } return_stmt;
    
    // Expression statement
    struct {
      struct ASTNode *expression;
    } expr_stmt;
    
    // Variable declaration
    struct {
      char *name;
      struct ASTNode *initializer;
      TypeInfo var_type;
      bool is_array;
      int array_size;
    } var_decl;
    
    // Assignment expression
    struct {
      struct ASTNode *lvalue;
      struct ASTNode *rvalue;
      TokenType assign_op;  // =, +=, -=, etc.
    } assign;
    
    // Binary expression
    struct {
      TokenType operator;
      struct ASTNode *left;
      struct ASTNode *right;
    } binary;
    
    // Unary expression
    struct {
      TokenType operator;
      struct ASTNode *operand;
      bool is_prefix;  // true for ++x, false for x++
    } unary;
    
    // Function call
    struct {
      char *name;
      struct ASTNode **arguments;
      size_t arg_count;
      size_t arg_capacity;
    } call;
    
    // Variable reference
    struct {
      char *name;
    } var_ref;
    
    // Integer literal
    struct {
      int value;
    } integer;
    
    // Array access
    struct {
      struct ASTNode *array;
      struct ASTNode *index;
    } array_access;
  };
} ASTNode;

// AST creation and management
ASTNode *ast_create_node(ASTNodeType type, SourcePosition pos);
void ast_free_node(ASTNode *node);
ASTNode *ast_copy_node(const ASTNode *node);

// Program nodes
ASTNode *ast_create_program(SourcePosition pos);
OrionError ast_program_add_function(ASTNode *program, ASTNode *function);

// Function nodes
ASTNode *ast_create_function(const char *name, TypeInfo return_type, SourcePosition pos);
OrionError ast_function_add_parameter(ASTNode *function, ASTNode *parameter);
void ast_function_set_body(ASTNode *function, ASTNode *body);

// Statement nodes
ASTNode *ast_create_compound_stmt(SourcePosition pos);
OrionError ast_compound_add_statement(ASTNode *compound, ASTNode *stmt);
ASTNode *ast_create_if_stmt(ASTNode *condition, ASTNode *then_stmt, ASTNode *else_stmt, SourcePosition pos);
ASTNode *ast_create_while_stmt(ASTNode *condition, ASTNode *body, SourcePosition pos);
ASTNode *ast_create_for_stmt(ASTNode *init, ASTNode *condition, ASTNode *update, ASTNode *body, SourcePosition pos);
ASTNode *ast_create_return_stmt(ASTNode *expression, SourcePosition pos);
ASTNode *ast_create_var_decl(const char *name, TypeInfo type, ASTNode *initializer, SourcePosition pos);

// Expression nodes
ASTNode *ast_create_assign_expr(ASTNode *lvalue, ASTNode *rvalue, TokenType op, SourcePosition pos);
ASTNode *ast_create_binary_expr(TokenType op, ASTNode *left, ASTNode *right, SourcePosition pos);
ASTNode *ast_create_unary_expr(TokenType op, ASTNode *operand, bool is_prefix, SourcePosition pos);
ASTNode *ast_create_call_expr(const char *name, SourcePosition pos);
OrionError ast_call_add_argument(ASTNode *call, ASTNode *arg);
ASTNode *ast_create_var_ref(const char *name, SourcePosition pos);
ASTNode *ast_create_integer_literal(int value, SourcePosition pos);

// Type utilities
TypeInfo type_create(DataType base_type);
TypeInfo type_create_array(DataType base_type, int size);
bool type_is_compatible(TypeInfo a, TypeInfo b);
const char *type_to_string(TypeInfo type);

// AST utilities
const char *ast_node_type_to_string(ASTNodeType type);
bool ast_is_statement(const ASTNode *node);
bool ast_is_expression(const ASTNode *node);
bool ast_is_lvalue(const ASTNode *node);

// AST debugging and printing
void ast_print_node(const ASTNode *node, int indent);
void ast_print_tree(const ASTNode *root);
void ast_dump_to_file(const ASTNode *root, FILE *file);

// AST traversal
typedef void (*ast_visitor_func)(ASTNode *node, void *context);
void ast_traverse_preorder(ASTNode *root, ast_visitor_func visitor, void *context);
void ast_traverse_postorder(ASTNode *root, ast_visitor_func visitor, void *context);

#endif // ORION_AST_H
