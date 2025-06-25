/**
 * @file include/ast.h
 * @brief Abstract Syntax Tree definitions
 */

#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdint.h>

// Forward declarations
typedef struct ASTNode ASTNode;

// AST Node types
typedef enum {
  AST_PROGRAM,
  AST_FUNCTION,
  AST_VARIABLE_DECL,
  AST_ASSIGNMENT,
  AST_BINARY_OP,
  AST_UNARY_OP,
  AST_CALL,
  AST_IDENTIFIER,
  AST_NUMBER,
  AST_STRING,
  AST_CHAR,
  AST_BLOCK,
  AST_IF,
  AST_WHILE,
  AST_FOR,
  AST_RETURN,
  AST_EXPRESSION_STMT
} ASTNodeType;

// Binary operators
typedef enum {
  BINOP_ADD,
  BINOP_SUB,
  BINOP_MUL,
  BINOP_DIV,
  BINOP_MOD,
  BINOP_EQ,
  BINOP_NE,
  BINOP_LT,
  BINOP_LE,
  BINOP_GT,
  BINOP_GE,
  BINOP_AND,
  BINOP_OR
} BinaryOperator;

// Unary operators
typedef enum {
  UNOP_MINUS,
  UNOP_NOT,
  UNOP_PRE_INC,
  UNOP_PRE_DEC,
  UNOP_POST_INC,
  UNOP_POST_DEC
} UnaryOperator;

// Data types
typedef enum {
  TYPE_VOID,
  TYPE_INT,
  TYPE_CHAR
} DataType;

// AST Node structure
struct ASTNode {
  ASTNodeType type;
  union {
    struct {
      ASTNode** statements;
      size_t statement_count;
    } program;
    
    struct {
      char* name;
      DataType return_type;
      ASTNode** parameters;
      size_t parameter_count;
      ASTNode* body;
    } function;
    
    struct {
      char* name;
      DataType type;
      ASTNode* initializer;
    } variable_decl;
    
    struct {
      char* name;
      ASTNode* value;
    } assignment;
    
    struct {
      BinaryOperator operator;
      ASTNode* left;
      ASTNode* right;
    } binary_op;
    
    struct {
      UnaryOperator operator;
      ASTNode* operand;
    } unary_op;
    
    struct {
      char* name;
      ASTNode** arguments;
      size_t argument_count;
    } call;
    
    struct {
      char* name;
    } identifier;
    
    struct {
      int64_t value;
    } number;
    
    struct {
      char* value;
    } string;
    
    struct {
      char value;
    } character;
    
    struct {
      ASTNode** statements;
      size_t statement_count;
    } block;
    
    struct {
      ASTNode* condition;
      ASTNode* then_branch;
      ASTNode* else_branch;
    } if_stmt;
    
    struct {
      ASTNode* condition;
      ASTNode* body;
    } while_stmt;
    
    struct {
      ASTNode* init;
      ASTNode* condition;
      ASTNode* update;
      ASTNode* body;
    } for_stmt;
    
    struct {
      ASTNode* value;
    } return_stmt;
    
    struct {
      ASTNode* expression;
    } expression_stmt;
  };
};

// Function declarations
ASTNode* ast_create_node(ASTNodeType type);
void ast_free_node(ASTNode* node);
void ast_print(const ASTNode* node, int indent);
ASTNode* ast_create_program(void);
ASTNode* ast_create_function(const char* name, DataType return_type);
ASTNode* ast_create_variable_decl(const char* name, DataType type, ASTNode* initializer);
ASTNode* ast_create_assignment(const char* name, ASTNode* value);
ASTNode* ast_create_binary_op(BinaryOperator op, ASTNode* left, ASTNode* right);
ASTNode* ast_create_unary_op(UnaryOperator op, ASTNode* operand);
ASTNode* ast_create_identifier(const char* name);
ASTNode* ast_create_number(int64_t value);
ASTNode* ast_create_string(const char* value);
ASTNode* ast_create_call(const char* name);
ASTNode* ast_create_block(void);
void ast_add_statement(ASTNode* parent, ASTNode* statement);
void ast_add_argument(ASTNode* call, ASTNode* argument);

#endif // AST_H