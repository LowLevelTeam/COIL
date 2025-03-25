# COIL Object


## .coil Object
The COIL object is a complex object format containing multiple sections needed to ensure the easiest end user experience. The COIL object contains multiple sections similar to native object formats like symbol tables, string tables, etc.... The COIL Object is the input of COIL processors who then create a [COIL Output Object](#coilo-object).

## .coilo Object
The COIL Output Object is a even more comlplex object format then the default, this object format has to handled native binary code for mutliple devices in an easy to link and space efficient manner. The COIL Output Object is the output of COIL processors who then hand this to COIL compatible linkers. This object format also is somewhat compatible with JIT as needed by the version 2 of COIL where hetregneous computing will be supported and JIT becomes somewhat needed. The JIT language is just COIL but stated explicitly in the binary to be handed back to the COIL processor with the configuration at runtime to be created into native binary at load time. This is similar to how CUDA does it but PTX in this exmaple would be the COIL and will be directly transpiled into SASS.




