/**
* @file include/orion/orinopp.h
* @brief functionality for reading and writing orion++
*
* In a lot of locations you will find (dynamic c arrays) / (pointer and size_t)
* these are used for ease of frontend and come at little to no cost thanks to the
* arena allocator backend allocating these values in a singular place.
*
* The arena allocator then becomes a free garbage collector stoppping all memory leaks
* as all lifetimes are known to be the same and freed in the same. the arrays are after all
* only partly dynamic as they can always be allocated in full on the parse of the instruction
* so more like a runtime static array.
* 
* Orion++ is stored in .opp files and does not exist in an object format and is instead just written
* simiarly to high level languages but utilize text (it can be viewed as text if needed for debugging but
* parsing orion++ disassemble text is not supported by orion++ compilers).
*
* Orion++ is made to be a simple high level target of compilers meaning you will lack
* a lot of specifics available in object formats though there are workarounds availble.
*
*
*/

#include "orionpp_detail.h"
#include "arena.h"

// -------------------------------- STRUCTS -------------------------------- //

typedef struct orionpp_header {
  unsigned char magic[ORIONPP_MAGIC_LEN];
  unsigned char version[3];
  uint8_t wordbytesize;
  uint8_t addrbytesize; // pointer size on system
  orionpp_feature_t features_1;
  orionpp_feature_t features_2; 
} orionpp_header_t;


typedef struct orionpp_ftype {
  orionpp_type_t type;
  orionpp_type_t module_type;
} orionpp_ftype_t;

typedef struct orionpp_value {
  char *val;
  size_t valsize;
  orionpp_ftype_t type;
} orionpp_value_t;

typedef struct orinopp_instruction {
  // members ordered for efficiency not format consistency
  orionpp_value_t *values;
  size_t value_count;
  orionpp_opcode_t root;
  orionpp_opcode_module_t child;
} orinopp_instruction_t;

// -------------------------------- FUNCS -------------------------------- //

void orionpp_string_opcode(char *buf, size_t bufsize, orionpp_opcode_t, orionpp_opcode_module_t);
void orionpp_string_type(char *buf, size_t bufsize, orionpp_type_t, orionpp_type_module_t);

// read in an instruction
void orionpp_readf(file_handle_t file, orinopp_instruction_t *dest);

// write out an instruction
void orionpp_writef(file_handle_t file, const orinopp_instruction_t *src);

// read in an instruction from a buffer
void orionpp_readbuf(char *buf, size_t bufsize, orinopp_instruction_t *dest);

// write out an instruction into a buffer
void orionpp_writebuf(const char *buf, size_t bufsize, const orinopp_instruction_t *src);

