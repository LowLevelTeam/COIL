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
    StaticLib orionlib = CreateStaticLib((StaticLibOptions){
      .output = "liborion-dev.a",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionlib, "./include");
    AddFile(orionlib, "./src/*.c");
    InstallStaticLib(orionlib);

    Executable orionlib_test = CreateExecutable((ExecutableOptions){
      .output = "test",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionlib_test, "./include");
    AddFile(orionlib_test, "./tests/*.c");
    AddLibraryPaths(orionlib_test, "./build");
    LinkSystemLibraries(orionlib_test, "orion-dev");
    InstallExecutable(orionlib_test);
    
    if (args.execute_commands) {
      RunCommand(orionlib_test.outputPath);
    }
  }
  EndBuild();
  
  return 0;
}