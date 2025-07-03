# Orion Human Compiler

Creates a simple human variation of the orion binary format for creating test orion++ binaries.

Also creates a standard for a decompiling language in cases of orion++ dumping. 

Strings are handled in a string table for binary formats but in the human variations they will of course be embedded directly for easy human oriented reading and writing. This means the string table can't be controlled from this human variation but of course this is not expected to be useful since this language is not something that people should be using instead direct binary translation is supported.

## Syntax

### Header Syntax

Needs to placed at the beggining of the file
```
// this is valid for no features and default version
{header>}

// this is valid for specific features and default version
{header>
  features: {
    CSTL,
    STL,
  }
}

// to specific everything
{header>
  version: 1.0.0
  features: {
    CSTL,
    STL,
  }
}
```

### Type Syntax

After the header follows type definitions
```
{type>
  // simple alias
  int = $prim.I32

  // shell type
  lib_int = extrn 10 // external reference number 10

  // composite type
  package = type comp.struct<
    //

    $qual.ptr<$qual.const<@int>> // c equivalant const int*
    $qual.const<$qual.ptr<@int>> // would mean you cant change the pointers address but the value can be changed

    @int

    // methods
    !package-create = $10 // the 10th defined function in code section since functions use numeric identifiers. could also be an extrn string
    !package-destroy = $11
  >
}
```

### ABI Syntax

After the type definitions follows the ABI definitions
```
{abi>

}
```

### EXTRN Syntax

After the abi definitions follows the external imports
```
{extrn>

}
```

### INTRN Syntax

After the external definitions follows the internal exports
```
{intrn>

}
```

### Data Syntax

After the internal exports is the data section. 

```
{data>

}
```

### Code Syntax

Finally code follows after data
```
{code>

}
```

#### Instruction Syntax

Instruction Syntax follows the root opcode and then module opcode.
The root opcode defines the module to target and the second module defines the instruction.
```
ISA.NOP
```

#### Operands Syntax



