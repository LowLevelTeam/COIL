# Overview

## COIL
COIL a high level binary intermediate language.

Features
- Assembly+
- Advanced Type System (Compound, Complex, Fixed Width, Asbtract)
- Variable System
- ABI System

COIL is built to be close to assembly in execution control meaning no if statements etc... but also contain abstractions to help with situations where complete knowledge of the target machine is impossible like in the case of redistributable libraries or cross platform programs.

## CBC
COIL Byte Code a low level binary intermediate language.

Features
- Variable System
- ABI System
- Minimal Types (Fixed Width, Abstract)
- Specific Instructions

## CBC-*
CBC Specifics, A collection of CBC like 

### CBC-CPU

Features
- Register System
- Fixed Width Types
- Specific Instructions
- Specifics Beyond

### CBC-GPU

