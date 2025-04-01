# COIL Byte Code
To support fast JIT and possible interpretation COIL processors support compiling instead of native code into COIL Byte Code.

The goal of this is to cut down on the complex compilation required by type oriented systems and instead revert to a more standard opcode variation with an instruction for each type. The variable system still exists being the last remenant as if the registers and stack could be known before hand then JIT would be almost useless anyways.

This CBC language is just a small, lightweight and compact COIL made specifically to be interpreted or compiled at runtime being fast and efficient at the cost of being potentially less portable then COIL.

CBC is made to exist with other native machine code inside the COIL output object (.coilo) where it is stored in a section relating to the data being compiled and then executed at runtime. It is important to note that it is more similar to a WASM like language, where as COIL really serves as the replacement to assembley languages and may even be able to be compiled to WASM in the future along with other pseduo machine languages, the CBC language really serves to replace WASM, JVM and python byte code as a more efficient alternative for runtime execution environments allowing COIL to even be the backend of a language like python or other incredibly dynamic languages.

