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
    Executable orioncc_program = CreateExecutable((ExecutableOptions){
      .output = "occ", // Orion C Compiler
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orioncc_program, "./include", "../liborion-dev/include/");
    AddLibraryPaths(orioncc_program, "../liborion-dev/build/");
    AddFile(orioncc_program, "./src/*.c");
    AddFile(orioncc_program, "./app/main.c");
    if (isLinux()) {
      LinkSystemLibraries(orioncc_program, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    LinkSystemLibraries(orioncc_program, "orion-dev");
    InstallExecutable(orioncc_program);

    Executable orioncc_test = CreateExecutable((ExecutableOptions){
      .output = "occ-test", // Orion C Compiler
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orioncc_test, "./include", "../liborion-dev/include/");
    AddLibraryPaths(orioncc_test, "../liborion-dev/build/");
    AddFile(orioncc_test, "./src/*.c");
    AddFile(orioncc_test, "./tests/test.c");
    if (isLinux()) {
      LinkSystemLibraries(orioncc_test, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    LinkSystemLibraries(orioncc_test, "orion-dev");
    InstallExecutable(orioncc_test);
  }
  EndBuild();
}