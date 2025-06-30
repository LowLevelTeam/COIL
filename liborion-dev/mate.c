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

    Executable orionobj_test = CreateExecutable((ExecutableOptions){
      .output = "test",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionobj_test, "./include");
    AddFile(orionobj_test, "./tests/test.c");
    AddLibraryPaths(orionobj_test, "./build");
    LinkSystemLibraries(orionobj_test, "orion-dev");
    InstallExecutable(orionobj_test);
    
    if (args.execute_commands) {
      RunCommand(orionobj_test.outputPath);
    }
  }
  EndBuild();
}