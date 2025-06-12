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
    Executable orionpp_compiler = CreateExecutable((ExecutableOptions){
      .output = "orionc++",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_compiler, "./include");
    AddFile(orionpp_compiler, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orionpp_compiler, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orionpp_compiler);

    if (args.execute_commands) {
      RunCommand("./build/orionc++ ./examples/arith.hopp -o ./build/arith.orion");
      RunCommand("./build/orionc++ ./examples/cf.hopp -o ./build/cf.orion");
    }
  }
  EndBuild();
}