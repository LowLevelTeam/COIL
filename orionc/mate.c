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
    Executable orion_compiler = CreateExecutable((ExecutableOptions){
      .output = "orionc",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orion_compiler, "./include");
    AddFile(orion_compiler, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orion_compiler, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orion_compiler);

    if (args.execute_commands) {
      RunCommand("orionc ./build/arith.orion -o ./build/arith.native");
      RunCommand("orionc ./build/cf.orion -o ./build/cf.native");
    }
  }
  EndBuild();
}