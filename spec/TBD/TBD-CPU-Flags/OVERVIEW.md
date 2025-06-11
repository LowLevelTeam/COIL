# TBD-CPU-Flags

## Overivew

The default CPU architecture family being the most commonly used thanks to large architectures like ARM and x86.


## Instruction Format

The instructions are all attempted to be fixed width where possible with some exceptions like nop which is there for padding and could be 1-2-4 or even more bytes (mainly used in x86).

Hot instructions are instructions which are known to be commonly used will be fixed width where as less used instructions will tend to be focused on being space efficient. Either way to load a full instruction into memory should never be more then {PLACEHOLDER} bytes.

TBD-CPU-Flags not only has the possibliity to be insanely good for optimization but a new way to write old CISC which are notourisouly hard to work with and give a new modern format for them. TBD-CPU-Flags directly transpiles instructions into native machine code but it is not guarnteed to be exactly as you may think. If you are working on a 'function' (functions don't exist in this langauge but a label which is global in the object under hint function in the object file would be similar) being up to a specific byte or a program there are helper utilities in the program to ensure this is met but the direct instruction to be transpiled can not be guarenteed. This is because TBD-CPU-Flags exposes functionality like SIMD always and will 'emulate' (will write out the underlying instructions for example vadd vec3_a, vec3_b will be vec3_a[0] + vec3_b[0]... where each element is stored somewhere).

This also means the memory model could get confusing and abstract with situations where more stack usage then expected due to a lack of native SIMD or even situations like entire float emulation. If you want predictable programs utilizing only required features to be part of TBD-CPU-Flags is your best bet with a focus on 16 bit integers, no floats, no vectors and limited specifics.

TBD-CPU-Flags processors may use an intermediate step like TBD-CPU-Flags-Native to create a native variant of TBD-CPU-Flags with features avilable to the current processor.
