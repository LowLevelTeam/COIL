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
    Executable oriondump_program = CreateExecutable((ExecutableOptions){
      .output = "oriondump",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(oriondump_program, "./include");
    AddIncludePaths(oriondump_program, "../liborionobj-dev/include");
    AddFile(oriondump_program, "./src/*.c");
    
    // Link with liborionobj-dev
    AddLibraryPaths(oriondump_program, "../liborionobj-dev/build");
    LinkSystemLibraries(oriondump_program, "orionobj-dev");
    
    if (isLinux()) {
      LinkSystemLibraries(oriondump_program, "m");
    }
    InstallExecutable(oriondump_program);

    // if (args.execute_commands) {
    //   // Test with various file types
    //   printf("=== Testing oriondump with different file types ===\n");
    //   
    //   // Test Orion object files (.orion)
    //   RunCommand("./build/oriondump -c ../liborionobj-dev/build/example_orionpp.orion");
    //   RunCommand("./build/oriondump -H ../liborionobj-dev/build/example_orionpp.orion");
    //   
    //   // Test Orion++ binary files (.orionpp) if they exist
    //   RunCommand("./build/oriondump -c ../orioncc/build/test_binary.orionpp 2>/dev/null || echo 'No .orionpp files found'");
    //   
    //   // Test Orion++ human files (.hopp) if they exist
    //   RunCommand("./build/oriondump -c ../orioncc/build/test_human.hopp 2>/dev/null || echo 'No .hopp files found'");
    //   
    //   // Show help
    //   RunCommand("./build/oriondump --help");
    // }
  }
  EndBuild();
}