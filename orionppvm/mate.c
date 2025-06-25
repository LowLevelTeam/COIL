#define MATE_IMPLEMENTATION
#include "../mate.h"
#include "../matearg.h"

i32 main(int argc, const char *argv[]) {
  struct Arguments args;
  if (argparse(argc, argv, &args) == 1) {
    return 1;
  }

  StartBuild();
  {
    Executable orionpp_vm = CreateExecutable((ExecutableOptions){
      .output = "ovm++", // Orion++ Virtual Machine
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_vm, "./include", "../liborion-dev/include/");
    AddLibraryPaths(orionpp_vm, "../liborion-dev/build/");
    AddFile(orionpp_vm, "./src/*.c");
    AddFile(orionpp_vm, "./app/main.c");
    if (isLinux()) {
      LinkSystemLibraries(orionpp_vm, "m"); // Add math library on Linux
    }
    LinkSystemLibraries(orionpp_vm, "orion-dev");
    InstallExecutable(orionpp_vm);
    
    // Also create a test executable
    Executable orionpp_vm_test = CreateExecutable((ExecutableOptions){
      .output = "ovm++-test",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_vm_test, "./include", "../liborion-dev/include/");
    AddLibraryPaths(orionpp_vm_test, "../liborion-dev/build/");
    AddFile(orionpp_vm_test, "./src/*");  
    AddFile(orionpp_vm_test, "./tests/test_vm.c");
    if (isLinux()) {
      LinkSystemLibraries(orionpp_vm_test, "m");
    }
    LinkSystemLibraries(orionpp_vm_test, "orion-dev");
    InstallExecutable(orionpp_vm_test);
  }
  EndBuild();
}