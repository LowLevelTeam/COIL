# Orion++

Project to create a better intermediate representation than what’s currently available—with more features. Orion++ is designed to be modern, expansive, and explicit in both its syntax and usage, catering to a variety of high-level language constructs while providing clear, human-readable output.

---

## Orion++ Human

The Orion++ Human variation is a human-friendly, textual representation of the Orion++ intermediate format. It is intended for use during development, debugging, and as a target for compiler frontends—all while supporting further abstraction and transformation into lower-level representations. With an emphasis on clarity and explicitness, Orion++ Human serves as the main entry point for expressing high-level optimization hints, type declarations, and syntactical constructs.

---

### Syntax

**Design Philosophy:**  
The syntax of Orion++ Human is engineered to be as clear and self-documenting as possible. Inspired by modern programming languages, its grammar is unambiguous, emphasizing structured control flow and explicit declaration. This clarity is crucial for both human developers and tooling—making the output easier to read, debug, and transform.

**Key Elements:**
- **Module Structure:**  
  Every Orion++ Human file represents a module, beginning with a module declaration and containing type definitions, function declarations, and variable declarations.
  
- **Functions and Blocks:**  
  Function definitions start with the `func` keyword, followed by parameter and return type annotations. Blocks are delimited by braces (`{ }`), and nesting is encouraged for clarity in control flow.
  
- **Comments:**  
  Single-line comments use `//`, and multi-line comments are defined with `/* ... */`.
  
- **Control Structures:**  
  Standard constructs such as `if`, `for`, `while`, and `switch` are supported. Orion++ Human preserves the high-level structure without dictating the low-level branching (which is handled during the lowering phase).

**Example:**

```orionpp
// Module: MathUtilities
module Math {
    // Function: Returns the sum of two integers.
    func add(a: int, b: int) -> int {
        return a + b;
    }
  
    // Function: Computes factorial recursively.
    func factorial(n: int) -> int {
        if (n <= 1) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    }
}
```

This example demonstrates Orion++ Human’s clear demarcation of modules, explicit function signatures, and neatly structured control flow constructs.

---

### Type System

**Overview:**  
Orion++ Human employs a rich, hybrid type system that supports both static and dynamic typing. The type system is designed to be more expressive than those in many high-level languages, enabling fine-grained compile-time optimizations while still affording runtime flexibility.

**Key Features:**
- **Static Types:**  
  Most variables and expressions are statically typed by default. This ensures early error detection and allows the compiler to perform extensive compile-time analysis and optimization.
  
- **Dynamic Types:**  
  Orion++ also supports dynamic types for cases where runtime flexibility is needed. This feature is particularly useful in languages that rely on polymorphism or when interfacing with languages that do not enforce strict typing.
  
- **Composite Types:**  
  The system includes support for primitives (e.g., `int`, `float`, `bool`), as well as user-defined types such as `structs`, `enums`, unions, arrays, and even generics.
  
- **Advanced Type Constructs:**  
  To capture rich semantics, Orion++ Human’s type system can support union types, sum types, and even dependent types in advanced usage, enabling more precise type relationships.

**Example:**

```orionpp
// Definition of a composite type using 'struct'
struct Point {
    x: float;
    y: float;
}

// A generic container type
struct Container<T> {
    value: T;
}
  
// Function using both static and dynamic types
func computeArea(p: Point) -> float {
    return 3.14 * (p.x * p.y); // Simplified calculation for illustration.
}
```

This snippet shows explicit type declarations, the use of composite types, and illustrates how generics can be integrated into the Orion++ type system—all making the representation both powerful and understandable.

---

### Variables

**Variable Declaration and Scope:**  
In Orion++ Human, variables are declared with explicit types, which can be either written out or inferred by the compiler when the context is unambiguous. Variables are immutable by default to encourage safer, functional styles of programming; however, mutable variables are supported via a dedicated keyword.

**Key Characteristics:**
- **Immutable by Default:**  
  The `let` keyword declares immutable variables, ensuring that once a value is bound, it cannot be altered.
  
- **Mutable Variables:**  
  If a variable needs to be mutable, the keyword `var` is used.
  
- **Explicit Typing and Inference:**  
  While type annotations are encouraged for clarity, Orion++ Human allows type inference in localized contexts to reduce verbosity without sacrificing explicitness when needed.
  
- **Scoped Declarations:**  
  Variables adhere to strict scoping rules—function-level, module-level, and block-level scoping are clearly delineated to avoid accidental capturing or shadowing.

**Example:**

```orionpp
module VariablesDemo {
    // Immutable variable declaration with explicit type.
    let maxCount: int = 100;
  
    // Mutable variable declaration with type inference.
    var currentCount = 0; // Inferred as int.
  
    // A function demonstrating variable scope.
    func updateCount(delta: int) {
        // A local variable with block scoping.
        let threshold: int = maxCount - 10;
        if (currentCount + delta > threshold) {
            // A localized mutable variable.
            var temp = currentCount + delta;
            currentCount = temp;
        }
    }
}
```

This example highlights the clear syntax for declaring both immutable and mutable variables, showing the usage of explicit type annotations alongside type inference, and demonstrates clear scoping rules within function blocks.

---

## Summary

The Orion++ Human representation is a modern, human-readable intermediate form designed to encapsulate high-level language constructs. With a clear, expressive syntax, a powerful hybrid type system, and explicit variable handling, Orion++ Human sets a new standard for intermediate representations in compiler design. It provides the semantic richness and explicitness needed by modern language tooling, all while serving as an ideal platform for high-level optimizations and cross-platform IR transformation.

## Orion++ Binary

**A binary reference will come after the completion of the human variation**
