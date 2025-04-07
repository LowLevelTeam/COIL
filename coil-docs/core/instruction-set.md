# COIL Instruction Set Architecture (ISA)

The COIL ISA can potentially be complex to work around for developers. Working out which features are available by the COIL implementation can be complex for such a wide reaching structure. We try to make this easy by defining the extensions within this one file.

For certain instructions you may see a (ext ...) where ... is some identifier like ABI. This mean the instruction may not be supported by all COIL implementations or in otherwords is not a core COIL feature.

We try to make this clear from one file to give the greatest help to developers searching for instructions rather then having an instruction set spread out across multiple files and being hard to work around here is the documentation for almost all COIL instructions.

There are other files to explain more about ISA but this is unofrtunately needed as describing all the instructions supported by individual processing units from this one file would make it almost impossible to maintain and work on. Aside from the processing unit and architecture specific instruction sets which can be found individually under pu/processing_unit/architecture.md every other instruction is listed in a common format.

Split into three categories, compiler (to be worked out by teh COIL processor leaving no runtime impact), core (default runtime instructions), extended (instructions specific to certain purposes like type operations, crypto, multi dimensional etc...)




