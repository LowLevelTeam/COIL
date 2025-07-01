# Orion++ Binary Format

The orion++ format is setup to be compact, fast and efficient in parsing, building and distribution.

Built similar to a basic object format with simple sectioning. Type tables are defined with type definitions, ABI Tables are defined with standard calling conventions and the final code section is for the functions and code for compilation.

## Layout

```
[HEADER]
[TYPE_TABLE]
[ABI_TABLE]
[EXTRN_TABLE] // all externally used 'items' (EXTRN for externally included)
[INTRN_TABLE] // all 'items' exposed (INTRN for internals exposed)
[STRING_TABLE] // for optional debug names
[CODE]

'items' = {TYPE, ABI, TARGET}s
```

## Header

```
[HEADER] = {
  magic[4,u8]: "OPP\xD4"
  reserved: u8
  version[3,u8]
  features: u64
  typetab_size: u64
  abitab_size: u64
  extrntab_size: u64
  intrntab_size: u64
  stringtab_size: u64
  code_size: u64
}
```

## Type Table

```
[TYPE_TABLE] <- {[ITEM]...}

[ITEM] = {
  name: offset_t = 0
  identifier: type_t = >65535
  value: [ITEM_VALUE]
}

[ITEM_VALUE]: [u16, ...] // varadic array of u16 type opcodes
```

## ABI Table

An ABI is seperated into trivial and complex. a trivial ABI would be system-v AMD64 ABI. This ABI is only for a single target and therefore has no variations. A more complex ABI would be the C ABI which would need branches in the ABI for x86-32, x86-64, ARM32, ARM64 and more.

```
[TYPE_TABLE] <- {[ITEM]...}

[ITEM] = {
  name: offset_t = 0
  identifier: type_t = >65535
  value: [ITEM_VALUE]
}

[ITEM_VALUE] = {
  target:
  callee:
  caller:
}
```

#### ABI Targets

Distros do not have much affect in the end result but help specify the exact target.

```
target = arch-os-distro

arch:
x86.16 (8086, legacy x86)
x86.32 (80386, i386, x86, x86-32, IA-32)
x86.64 (amd64, intel64, x86-64, x64, x86_64)
arm.16 (ARM Thumb)
arm.32 (ARM 32)
arm.64 (ARM 64)
riscv.32 (RISCV-32)
riscv.64 (RISCV-64)
riscv.128 (RISCV-128)
...

os-distro:
linux
window
osx
ios
andriod
chrombook
bsd
```


## EXTRN Table

```
[TYPE_TABLE] <- {[ITEM]...}

[ITEM] = {
  name: offset_t // names are required to resolve
  identifier_type: u8
  identifier: u64
  information: opt<void*> // based on identifier type
}
```

## INTRN Table

```
[TYPE_TABLE] <- {[ITEM]...}

[ITEM] = {
  name: offset_t // names are required to resolve
  identifier_type: u8
  identifier: u64
}
```

## STRING Table

```
[TYPE_TABLE] <- {[ITEM]...}

// null terminated string
[ITEM] = "...\0"
```

## Code

