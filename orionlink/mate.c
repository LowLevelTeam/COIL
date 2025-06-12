#define MATE_IMPLEMENTATION
#include "../mate.h"
#include "../matearg.h"

i32 main(int argc, const char *argv[]) {
  Arguments args;
  if (argparse(argc, argv, &args) == 1) {
    return 1;
  }

  StartBuild();
  {
    Executable orion_linker = CreateExecutable((ExecutableOptions){
      .output = "orionlink",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orion_linker, "./include");
    AddFile(orion_linker, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orion_linker, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orion_linker);

    if (args.execute_commands) {
      // the output object format is determined by system when -f is not supplied
      RunCommand("orionlink ./build/arith.native -o arith.out");
      RunCommand("orionlink ./build/cf.native -o arith.out");
    }
  }
  EndBuild();
}