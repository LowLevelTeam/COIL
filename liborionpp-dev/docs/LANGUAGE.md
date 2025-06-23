# Orion++ Core Language Specification

## 1. Overview

Orion++ is a target-agnostic intermediate representation with two formats:
- **Human format** (.hopp): Assembly-like syntax for development
- **Binary format** (.orionpp): Compact encoding for production

## 2. Lexical Structure

### 2.1 Tokens

```ebnf
program     = { statement } ;
statement   = function | symbol | section | comment ;
comment     = "//" { any-char } newline | "/*" { any-char } "*/" ;

identifier  = letter { letter | digit | "_" } ;
variable    = "$" positive-int ;
symbol      = "@" string-literal ;
label       = "." identifier ":" ;
integer     = [ "-" ] digit { digit } ;
string-literal = "\"" { string-char | escape-seq } "\"" ;

escape-seq  = "\\" ( "\"" | "\\" | "n" | "r" | "t" | "0" ) ;
letter      = "a".."z" | "A".."Z" | "_" ;
digit       = "0".."9" ;
```

### 2.2 Variable Ranges

| Range | Type | Usage |
|-------|------|-------|
| $1-$255 | Parameters | Function arguments |
| $256+ | Locals | Local variables and temporaries |

## 3. Type System

### 3.1 Data Types

```c
typedef enum {
  ORION_TYPE_VOID = 0,    // No value
  ORION_TYPE_WORD = 1     // Machine word (32/64-bit dependent)
} OrionDataType;
```

**Note**: `word` size is determined by target architecture:
- 32-bit targets: 4 bytes
- 64-bit targets: 8 bytes

## 4. Grammar

### 4.1 Function Definition

```ebnf
function = "obj.sym" "[" visibility "," "func" "]" symbol ":"
           { hint }
           "abi.callee.enter"
           "isa.enter"
           { instruction }
           "isa.leave"
           "abi.callee.leave"
           "isa.RET"
           ".hint-endfunc()" ;

visibility = "global" | "local" | "weak" ;
hint = ".hint-abi(" abi-type ")" 
     | ".hint-args(" [ type-list ] ")"
     | ".hint-rets(" type ")" ;
abi-type = "c" | "custom" ;
type = "void" | "word" ;
type-list = type { "," type } ;
```

### 4.2 Instructions

```ebnf
instruction = core-inst | var-inst | arith-inst | control-inst | abi-inst ;

core-inst = "isa.nop" 
          | "isa.enter" 
          | "isa.leave" 
          | "isa.RET" ;

var-inst = "isa.const" variable "," integer
         | "isa.let" variable "," variable ;

arith-inst = "isa.add" variable "," variable "," variable
           | "isa.sub" variable "," variable "," variable
           | "isa.mul" variable "," variable "," variable
           | "isa.div" variable "," variable "," variable
           | "isa.neg" variable "," variable ;

control-inst = "isa.jmp" label-ref
             | "isa.call" symbol
             | "isa.beq" variable "," variable "," label-ref
             | "isa.bne" variable "," variable "," label-ref
             | "isa.blt" variable "," variable "," label-ref
             | "isa.bgt" variable "," variable "," label-ref ;

abi-inst = "abi.callee.enter" | "abi.callee.leave"
         | "abi.callee.getarg." integer
         | "abi.callee.setret." integer variable
         | "abi.caller.setup" | "abi.caller.cleanup"
         | "abi.caller.setarg." integer variable
         | "abi.caller.getret." integer variable ;

label-ref = "." identifier ;
```

## 5. Binary Format

### 5.1 File Header

```c
struct OrionHeader {
  uint32_t magic;           // 0x4F50504F ("OPPO")
  uint16_t version;         // Format version (2)
  uint16_t flags;           // Reserved
  uint32_t string_offset;   // String table offset
  uint32_t string_size;     // String table size
  uint32_t symbol_offset;   // Symbol table offset
  uint32_t symbol_size;     // Symbol table size
  uint32_t function_offset; // Function table offset
  uint32_t function_size;   // Function table size
  uint32_t code_offset;     // Instruction data offset
  uint32_t code_size;       // Instruction data size
  uint32_t entry_point;     // Entry function index
};
```

### 5.2 Instruction Encoding

```
Instruction := Opcode(8) Flags(8) Operand*
Operand     := Type(2) Value(30) | Type(2) Value(6) ExtValue(24)

Types:
00 = Immediate value
01 = Variable ID  
10 = Label offset
11 = Symbol reference
```

### 5.3 Core Opcodes

```c
// Core operations
#define OP_NOP      0x00
#define OP_ENTER    0x01  
#define OP_LEAVE    0x02
#define OP_RET      0x03

// Variable operations
#define OP_CONST    0x10
#define OP_LET      0x11

// Arithmetic
#define OP_ADD      0x20
#define OP_SUB      0x21
#define OP_MUL      0x22
#define OP_DIV      0x23
#define OP_NEG      0x24

// Control flow
#define OP_JMP      0x30
#define OP_CALL     0x31
#define OP_BEQ      0x32
#define OP_BNE      0x33
#define OP_BLT      0x34
#define OP_BGT      0x35
```

## 6. Standard Library API

### 6.1 Reader API

```c
// Context management
OrionContext* orion_load_file(const char* filename);
void orion_free_context(OrionContext* ctx);

// Function access
size_t orion_get_function_count(const OrionContext* ctx);
OrionFunction* orion_get_function(const OrionContext* ctx, size_t index);
const char* orion_function_get_name(const OrionFunction* func);

// Instruction iteration
OrionInstruction* orion_function_get_instructions(const OrionFunction* func);
size_t orion_function_get_instruction_count(const OrionFunction* func);
```

### 6.2 Writer API

```c
// Writer management
OrionWriter* orion_writer_create(void);
void orion_writer_free(OrionWriter* writer);

// Function building
OrionFunctionBuilder* orion_add_function(OrionWriter* writer, const char* name);
void orion_add_const(OrionFunctionBuilder* builder, uint32_t dest, int64_t value);
void orion_add_let(OrionFunctionBuilder* builder, uint32_t dest, uint32_t src);
void orion_add_add(OrionFunctionBuilder* builder, uint32_t dest, uint32_t a, uint32_t b);

// File output
OrionResult orion_write_binary(OrionWriter* writer, const char* filename);
OrionResult orion_write_human(OrionWriter* writer, const char* filename);
```

## 7. Examples

### 7.1 Simple Function

```assembly
// word add(word a, word b) { return a + b; }
obj.sym[global,func] @"add":
  .hint-abi(c)
  .hint-args(word, word)
  .hint-rets(word)
  
  abi.callee.enter
  isa.enter
    isa.let $1, abi.callee.getarg.0
    isa.let $2, abi.callee.getarg.1
    isa.add $256, $1, $2
    abi.callee.setret.0 $256
  isa.leave
  abi.callee.leave
  isa.RET
.hint-endfunc()
```

### 7.2 Control Flow

```assembly
// word factorial(word n) {
//   if (n <= 1) return 1;
//   return n * factorial(n - 1);
// }
obj.sym[global,func] @"factorial":
  .hint-abi(c)
  .hint-args(word)
  .hint-rets(word)
  
  abi.callee.enter
  isa.enter
    isa.let $1, abi.callee.getarg.0     // n
    isa.const $256, 1                   // constant 1
    isa.bgt $1, $256, .L_recursive      // if n > 1
    
    // Base case
    abi.callee.setret.0 $256
    isa.jmp .L_end
    
  obj.label .L_recursive:
    // Recursive call
    isa.sub $257, $1, $256              // n - 1
    abi.caller.setup
    abi.caller.setarg.0 $257
    isa.call @"factorial"
    abi.caller.getret.0 $258            // factorial(n-1)
    abi.caller.cleanup
    
    isa.mul $259, $1, $258              // n * factorial(n-1)
    abi.callee.setret.0 $259
    
  obj.label .L_end:
  isa.leave
  abi.callee.leave
  isa.RET
.hint-endfunc()
```

## 8. Implementation Notes

### 8.1 Variable ID Allocation
- Parameters: Allocated sequentially from $1
- Locals: Allocated from $256 upward
- Temporaries: Compiler-generated, continue from locals

### 8.2 Symbol Resolution
- First pass: Collect all symbol definitions
- Second pass: Resolve symbol references
- Error on undefined symbols

### 8.3 Type Checking
- All variables implicitly typed as `word`
- Operations must have compatible operands
- Function signatures validated at call sites

### 8.4 Target Word Size
- Determined during code generation phase
- All `word` values assumed to fit in target word size
- Overflow behavior is target-dependent

---

*This specification defines Orion++ v2.0 core language features. Extended features and optimizations are defined in separate documents.*