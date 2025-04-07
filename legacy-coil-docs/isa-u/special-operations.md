# Special Operations

## Overview

Special Operations (0x90-0x9F) provide specialized functionality that doesn't fit into other instruction categories. These operations include cryptographic functions, random number generation, compression, and other specialized algorithms that are needed across different platforms but are not part of the basic arithmetic, logic, or memory operations.

## Instruction Set

| Opcode | Mnemonic | Description |
|--------|----------|-------------|
| 0x90   | RNG      | Random number generation |
| 0x91   | HASH     | Hash function |
| 0x92   | ENCRYPT  | Encryption operation |
| 0x93   | DECRYPT  | Decryption operation |
| 0x94   | CRYPT    | Cryptographic operation |
| 0x95   | SIGN     | Digital signature |
| 0x96   | VERIFY   | Signature verification |
| 0x97   | COMPRESS | Data compression |
| 0x98   | EXPAND   | Data expansion |
| 0x99   | CHECKSUM | Checksum calculation |

## Detailed Specifications

### RNG (0x90)

Generates a random number.

**Encoding:**
```
// Basic random number generation
[0x90][0x02][dest: Operand][range: Operand]

// Seeded random number generation
[0x90][0x03][dest: Operand][range: Operand][seed: Operand]
```

**Behavior:**
- Generates a random number within the specified range [0, range-1]
- For integer types: Uniform distribution of integers
- For floating-point types: Uniform distribution of floating-point values in [0.0, 1.0)
- With seed operand: Deterministic random sequence based on seed
- Without seed operand: Implementation-defined source of randomness
- Sets no flags

### HASH (0x91)

Computes a hash value.

**Encoding:**
```
// Basic hash
[0x91][0x03][dest: Operand][data: Operand][method: Operand]
```

**Behavior:**
- Computes a hash of the data using the specified method
- Data can be any type (scalar, vector, memory region)
- Method specifies the hash algorithm to use
- Common methods include: 0=FNV-1a, 1=MurmurHash, 2=SHA-1, 3=SHA-256, 4=MD5
- Destination receives the hash value
- Sets no flags

### ENCRYPT (0x92)

Performs encryption operation.

**Encoding:**
```
// Basic encryption
[0x92][0x03][dest: Operand][data: Operand][key: Operand]

// Encryption with algorithm selection
[0x92][0x04][dest: Operand][data: Operand][key: Operand][algorithm: Operand]
```

**Behavior:**
- Encrypts data using the specified key and algorithm
- Data can be any type (scalar, vector, memory region)
- Algorithm specifies the encryption method (default=0: implementation-defined)
- Common algorithms: 0=AES, 1=DES, 2=ChaCha20, 3=Implementation-defined
- Writes encrypted result to destination
- Sets no flags

### DECRYPT (0x93)

Performs decryption operation.

**Encoding:**
```
// Basic decryption
[0x93][0x03][dest: Operand][data: Operand][key: Operand]

// Decryption with algorithm selection
[0x93][0x04][dest: Operand][data: Operand][key: Operand][algorithm: Operand]
```

**Behavior:**
- Decrypts data using the specified key and algorithm
- Data must be previously encrypted with matching algorithm and key
- Algorithm specifies the decryption method (default=0: implementation-defined)
- Common algorithms: 0=AES, 1=DES, 2=ChaCha20, 3=Implementation-defined
- Writes decrypted result to destination
- Sets no flags

### CRYPT (0x94)

Performs general cryptographic operation.

**Encoding:**
```
[0x94][0x05][dest: Operand][operation: Operand][data: Operand][key: Operand][params: Operand]
```

**Behavior:**
- Performs the specified cryptographic operation
- Operation specifies the cryptographic function to perform
- Common operations: 0=HMAC, 1=KDF, 2=AUTH_ENCRYPT, 3=AUTH_DECRYPT
- Params provides additional parameters required by specific operations
- Writes operation result to destination
- Sets flags based on operation success

### SIGN (0x95)

Creates a digital signature.

**Encoding:**
```
[0x95][0x03][dest: Operand][data: Operand][key: Operand]
```

**Behavior:**
- Creates a digital signature for the data using the key
- Data can be any type (scalar, vector, memory region)
- Key must be a valid signing key for the implementation
- Writes signature to destination
- Sets flags based on operation success

### VERIFY (0x96)

Verifies a digital signature.

**Encoding:**
```
[0x96][0x03][result: Operand][data: Operand][signature: Operand][key: Operand]
```

**Behavior:**
- Verifies that signature is valid for data using the key
- Data can be any type (scalar, vector, memory region)
- Key must be a valid verification key (typically public key)
- Result receives 1 if signature is valid, 0 if invalid
- Sets Zero flag based on verification result (Z=1 if invalid)

### COMPRESS (0x97)

Compresses data.

**Encoding:**
```
// Basic compression
[0x97][0x03][dest: Operand][data: Operand][size: Operand]

// Compression with algorithm selection
[0x97][0x04][dest: Operand][data: Operand][size: Operand][algorithm: Operand]
```

**Behavior:**
- Compresses data using specified algorithm
- Data points to the source data to compress
- Size specifies the size of the source data in bytes
- Algorithm specifies the compression method (default=0: implementation-defined)
- Common algorithms: 0=DEFLATE, 1=LZ4, 2=ZSTD, 3=Implementation-defined
- Writes compressed data to destination
- Sets no flags

### EXPAND (0x98)

Decompresses data.

**Encoding:**
```
// Basic decompression
[0x98][0x03][dest: Operand][data: Operand][max_size: Operand]

// Decompression with algorithm selection
[0x98][0x04][dest: Operand][data: Operand][max_size: Operand][algorithm: Operand]
```

**Behavior:**
- Decompresses data using specified algorithm
- Data points to the compressed source data
- Max_size specifies the maximum size for the decompressed output
- Algorithm specifies the decompression method (default=0: implementation-defined)
- Common algorithms: 0=DEFLATE, 1=LZ4, 2=ZSTD, 3=Implementation-defined
- Writes decompressed data to destination
- Sets flags based on operation success

### CHECKSUM (0x99)

Calculates a checksum.

**Encoding:**
```
[0x99][0x03][dest: Operand][data: Operand][method: Operand]
```

**Behavior:**
- Calculates a checksum of the data using the specified method
- Data can be any type (scalar, vector, memory region)
- Method specifies the checksum algorithm to use
- Common methods include: 0=CRC32, 1=Adler32, 2=Fletcher32, 3=Implementation-defined
- Destination receives the checksum value
- Sets no flags

## Implementation Requirements

Special operations have these implementation requirements:

1. **Algorithm Quality**: Random numbers, cryptographic functions, and hashes must meet quality standards appropriate for their purpose
2. **Side-Channel Protection**: Cryptographic operations should protect against timing and other side-channel attacks where possible
3. **Algorithm Selection**: Implementations must document which specific algorithms are supported for each operation
4. **Fallbacks**: Implementations should provide reasonable behavior when specialized hardware support is unavailable

## Notes on Implementation

Implementations may take different approaches to special operations:

1. **Hardware Acceleration**: Use specialized hardware when available
2. **Software Fallback**: Provide software implementations when hardware support is unavailable
3. **Library Integration**: Map operations to high-quality library implementations
4. **Simplified Implementations**: Provide basic implementations where full cryptographic strength is not required

## Related Components

- [Arithmetic Operations](./arithmetic-operations.md) - Basic computational operations
- [Bit Manipulation](./bit-manipulation.md) - Bitwise operations used in many special algorithms
- [Vector Operations](./vector-operations.md) - Vector operations often used with special functions