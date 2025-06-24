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

    // Executable orionpp_example = CreateExecutable((ExecutableOptions){
    //   .output = "example",
    //   .std = args.stdlevel,
    //   .debug = args.debuglevel,
    //   .warnings = args.warninglevel,
    //   .error = args.errorfmt,
    //   .optimization = args.optlevel
    // });
    // AddIncludePaths(orionpp_example, "./include");
    // AddFile(orionpp_example, "./examples/example.c");
    // AddLibraryPaths(orionpp_example, "./build");
    // LinkSystemLibraries(orionpp_example, "orionpp-dev");
    // InstallExecutable(orionpp_example);
    // 
    // if (args.execute_commands) {
    //   RunCommand(orionpp_example.outputPath);
    // }
  }
  EndBuild();
}