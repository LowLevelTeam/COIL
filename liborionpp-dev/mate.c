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
    StaticLib orionpp_library = CreateStaticLib((StaticLibOptions){
      .output = "liborionpp-dev.a",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_library, "./include");
    AddFile(orionpp_library, "./src/*.c");
    InstallStaticLib(orionpp_library);

    Executable orionpp_test = CreateExecutable((ExecutableOptions){
      .output = "test",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_test, "./include");
    AddFile(orionpp_test, "./tests/test.c");
    AddLibraryPaths(orionpp_test, "./build");
    LinkSystemLibraries(orionpp_test, "orionpp-dev");
    InstallExecutable(orionpp_test);
    
    if (args.execute_commands) {
      RunCommand(orionpp_test.outputPath);
    }
  }
  EndBuild();
}