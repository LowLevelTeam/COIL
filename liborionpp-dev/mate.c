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
    // Build liborionobj-dev first (dependency)
    StaticLib orionobj_library = CreateStaticLib((StaticLibOptions){
      .output = "liborionobj-dev.a",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionobj_library, "../liborionobj-dev/include");
    AddFile(orionobj_library, "./../liborionobj-dev/src/*.c");
    InstallStaticLib(orionobj_library);

    // Build liborionpp-dev (depends on liborionobj-dev)
    StaticLib orionpp_library = CreateStaticLib((StaticLibOptions){
      .output = "liborionpp-dev.a",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_library, "./include", "../liborionobj-dev/include");
    
    // Add source files organized by module
    AddFile(orionpp_library, "./src/*.c");

    InstallStaticLib(orionpp_library);

    // Build test executable
    Executable orionpp_test = CreateExecutable((ExecutableOptions){
      .output = "test",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orionpp_test, "./include", "../liborionobj-dev/include");
    AddFile(orionpp_test, "./tests/test.c");
    AddLibraryPaths(orionpp_test, "./build");
    LinkSystemLibraries(orionpp_test, "orionpp-dev");
    LinkSystemLibraries(orionpp_test, "orionobj-dev");
    InstallExecutable(orionpp_test);
    
    // Build simple example executable
    Executable simple_example = CreateExecutable((ExecutableOptions){
      .output = "simple_example",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(simple_example, "./include", "../liborionobj-dev/include");
    AddFile(simple_example, "./examples/simple.c");
    AddLibraryPaths(simple_example, "./build");
    LinkSystemLibraries(simple_example, "orionpp-dev");
    LinkSystemLibraries(simple_example, "orionobj-dev");
    InstallExecutable(simple_example);
    
    if (args.execute_commands) {
      RunCommand(orionpp_test.outputPath);
      RunCommand(simple_example.outputPath);
    }
  }
  EndBuild();
}