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
    AddIncludePaths(orioncc_program, "./include");
    AddFile(orioncc_program, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orioncc_program, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orioncc_program);
  }
  EndBuild();
}
