# Vector/Array Operations (0x90-0x9F)

## Purpose

This document defines the vector and array operations in COIL, which enable efficient parallel processing of multiple data elements and manipulation of array structures.

## Instruction List

| Opcode | Mnemonic | Assembly Syntax | Description |
|--------|----------|-----------------|-------------|
| 0x90   | VLOAD    | `VLOAD dest, src[, TYPE_PARAM0][, TYPE_PARAM5]` | Load vector |
| 0x91   | VSTORE   | `VSTORE dest, src[, TYPE_PARAM0][, TYPE_PARAM5]` | Store vector |
| 0x96   | VEXTRACT | `VEXTRACT dest, src, index[, TYPE_PARAM5]` | Extract element |
| 0x97   | VINSERT  | `VINSERT dest, src, value, index[, TYPE_PARAM5]` | Insert element |
| 0x98   | VSHUFFLE | `VSHUFFLE dest, src, mask[, TYPE_PARAM5]` | Shuffle elements |
| 0x99   | VCMP     | `VCMP dest, left, right, cmp_type[, TYPE_PARAM5]` | Compare vectors |
| 0x9A   | VBLEND   | `VBLEND dest, first, second, mask[, TYPE_PARAM5]` | Blend vectors |
| 0x9B   | VMASKMOV | `VMASKMOV dest, src, mask[, TYPE_PARAM5]` | Masked move |
| 0x9C   | VDOT     | `VDOT dest, left, right[, TYPE_PARAM5]` | Dot product |
| 0x9D   | VCROSS   | `VCROSS dest, left, right[, TYPE_PARAM5]` | Cross product |
| 0x9E   | VHSUM    | `VHSUM dest, src[, TYPE_PARAM5]` | Horizontal sum |

## Detailed Descriptions

### VLOAD (0x90)
Load data from memory into a vector.

#### Assembly Syntax
```
VLOAD dest, src[, TYPE_PARAM0=vector_control][, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x90                  ; Opcode for VLOAD
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination vector
[dest_value]          ; Destination value
[src_type]            ; Type of source address
[src_value]           ; Source address value
[TYPE_PARAM0]         ; Optional vector control
[control_value]       ; Vector control value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Vector Control Parameters
- `TYPE_PARAM0=VECTOR_CTRL_ALIGNED (0x01)`: Aligned memory access
- `TYPE_PARAM0=VECTOR_CTRL_UNALIGNED (0x02)`: Unaligned memory access
- `TYPE_PARAM0=VECTOR_CTRL_BROADCAST (0x03)`: Broadcast scalar to all elements
- `TYPE_PARAM0=VECTOR_CTRL_GATHER (0x04)`: Gather from indexed memory locations

#### Example
```
; Assembly (aligned load)
VLOAD TYPE_V128=TYPE_FP32, vector, [array_ptr]

; Binary
0x90      ; VLOAD
0x02      ; Two operands
0x3025    ; TYPE_V128=TYPE_FP32
[id]      ; Vector identifier
0xA600    ; TYPE_PTR (memory address)
[id]      ; Address identifier

; Assembly (broadcast scalar)
VLOAD TYPE_V256=TYPE_INT32, vector, [scalar_value], TYPE_PARAM0=VECTOR_CTRL_BROADCAST

; Binary
0x90      ; VLOAD
0x03      ; Three operands
0x3103    ; TYPE_V256=TYPE_INT32
[id]      ; Vector identifier
0xA600    ; TYPE_PTR (memory address)
[id]      ; Address identifier
0xFE00    ; TYPE_PARAM0
0x03      ; VECTOR_CTRL_BROADCAST
```

### VSTORE (0x91)
Store vector data to memory.

#### Assembly Syntax
```
VSTORE dest, src[, TYPE_PARAM0=vector_control][, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x91                  ; Opcode for VSTORE
0x02/0x03/0x04        ; Two, three, or four operands
[dest_type]           ; Type of destination address
[dest_value]          ; Destination address value
[src_type]            ; Type of source vector
[src_value]           ; Source vector value
[TYPE_PARAM0]         ; Optional vector control
[control_value]       ; Vector control value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (aligned store)
VSTORE [array_ptr], TYPE_V256=TYPE_FP32, vector

; Binary
0x91      ; VSTORE
0x02      ; Two operands
0xA600    ; TYPE_PTR (memory address)
[id]      ; Address identifier
0x3125    ; TYPE_V256=TYPE_FP32
[id]      ; Vector identifier
```

### VEXTRACT (0x96)
Extract a scalar element from a vector.

#### Assembly Syntax
```
VEXTRACT dest, src, index[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x96                  ; Opcode for VEXTRACT
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination scalar
[dest_value]          ; Destination value
[src_type]            ; Type of source vector
[src_value]           ; Source vector value
[index_type]          ; Type of index
[index_value]         ; Index value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly (extract second element)
VEXTRACT TYPE_INT32, scalar_result, TYPE_V128=TYPE_INT32, vector, 1

; Binary
0x96      ; VEXTRACT
0x03      ; Three operands
0x0300    ; TYPE_INT32
[id]      ; Scalar result identifier
0x3003    ; TYPE_V128=TYPE_INT32
[id]      ; Vector identifier
0x1020    ; TYPE_UNT8+IMM
0x01      ; Index 1
```

### VDOT (0x9C)
Compute the dot product of two vectors.

#### Assembly Syntax
```
VDOT dest, left, right[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x9C                  ; Opcode for VDOT
0x03/0x04             ; Three or four operands
[dest_type]           ; Type of destination scalar
[dest_value]          ; Destination value
[left_type]           ; Type of left vector
[left_value]          ; Left vector value
[right_type]          ; Type of right vector
[right_value]         ; Right vector value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
VDOT TYPE_FP32, result, vec1, vec2

; Binary
0x9C      ; VDOT
0x03      ; Three operands
0x2500    ; TYPE_FP32
[id]      ; Result identifier
0x3025    ; TYPE_V128=TYPE_FP32
[id]      ; Vec1 identifier
0x3025    ; TYPE_V128=TYPE_FP32
[id]      ; Vec2 identifier
```

### VCMP (0x99)
Compare vectors and produce a mask.

#### Assembly Syntax
```
VCMP dest, left, right, cmp_type[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x99                  ; Opcode for VCMP
0x04/0x05             ; Four or five operands
[dest_type]           ; Type of destination mask vector
[dest_value]          ; Destination value
[left_type]           ; Type of left operand vector
[left_value]          ; Left operand value
[right_type]          ; Type of right operand vector/scalar
[right_value]         ; Right operand value
[cmp_type]            ; Type of comparison type
[cmp_value]           ; Comparison type value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Comparison Types
- `0`: Equal
- `1`: Not Equal
- `2`: Less Than
- `3`: Less Than or Equal
- `4`: Greater Than
- `5`: Greater Than or Equal

#### Example
```
; Assembly (compare for equality)
VCMP TYPE_V128=TYPE_BIT, result_mask, vec1, vec2, 0

; Binary
0x99      ; VCMP
0x04      ; Four operands
0x3040    ; TYPE_V128=TYPE_BIT
[id]      ; Result mask identifier
0x3003    ; TYPE_V128=TYPE_INT32
[id]      ; Vec1 identifier
0x3003    ; TYPE_V128=TYPE_INT32
[id]      ; Vec2 identifier
0x1020    ; TYPE_UNT8+IMM
0x00      ; Comparison type 0 (equals)
```

### VHSUM (0x9E)
Compute the horizontal sum of all elements in a vector.

#### Assembly Syntax
```
VHSUM dest, src[, TYPE_PARAM5=condition]
```

#### Binary Encoding
```
0x9E                  ; Opcode for VHSUM
0x02/0x03             ; Two or three operands
[dest_type]           ; Type of destination scalar
[dest_value]          ; Destination value
[src_type]            ; Type of source vector
[src_value]           ; Source vector value
[TYPE_PARAM5]         ; Optional condition
[condition_value]     ; Condition value
```

#### Example
```
; Assembly
VHSUM TYPE_INT32, sum, TYPE_V128=TYPE_INT32, vector

; Binary
0x9E      ; VHSUM
0x02      ; Two operands
0x0300    ; TYPE_INT32
[id]      ; Sum identifier
0x3003    ; TYPE_V128=TYPE_INT32
[id]      ; Vector identifier
```

## Vector Types

COIL supports several vector types:

| Type | Size (Bytes) | Description |
|------|--------------|-------------|
| TYPE_V128 | 16 | 128-bit vector |
| TYPE_V256 | 32 | 256-bit vector |
| TYPE_V512 | 64 | 512-bit vector |

Vector types are parameterized by their element type:
- `TYPE_V128=TYPE_INT32`: 128-bit vector of 4x 32-bit integers
- `TYPE_V256=TYPE_FP32`: 256-bit vector of 8x 32-bit floats
- `TYPE_V512=TYPE_INT16`: 512-bit vector of 32x 16-bit integers

## Common Vector Patterns

### Vector Initialization

```
; Initialize vector from memory
VLOAD TYPE_V128=TYPE_INT32, vec, [array_ptr]

; Initialize vector with broadcast
VLOAD TYPE_V128=TYPE_INT32, vec, [scalar_ptr], TYPE_PARAM0=VECTOR_CTRL_BROADCAST
```

### Vector Element-wise Processing

```
; Element-wise operations
; Add two vectors element by element
ADD TYPE_V128=TYPE_FP32, result, vec1, vec2

; Multiply vector by scalar
MUL TYPE_V256=TYPE_INT32, result, vec, 2
```

### Vector Reduction

```
; Compute sum of all elements
VHSUM TYPE_FP32, total, TYPE_V256=TYPE_FP32, vector

; Compute dot product
VDOT TYPE_FP32, dot_result, vec1, vec2
```

### Vector Conditional Processing

```
; Create mask based on comparison
VCMP TYPE_V128=TYPE_BIT, mask, vec, 0, 4  ; vec > 0

; Apply mask to selectively update
VBLEND TYPE_V128=TYPE_FP32, result, vec_if_true, vec_if_false, mask
```

## Hardware Mapping

While vector operations are universal across processor types, their implementation varies based on available hardware:

1. **On SIMD-capable CPUs**:
   - TYPE_V128 maps to 128-bit registers (e.g., XMM on x86)
   - TYPE_V256 maps to 256-bit registers (e.g., YMM on x86)
   - TYPE_V512 maps to 512-bit registers (e.g., ZMM on x86)

2. **On processors without native SIMD**:
   - Operations are decomposed into scalar instructions
   - Each element is processed individually

The COIL processor automatically handles these mappings based on the target architecture.