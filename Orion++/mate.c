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
      .output = "libcoil-test",
      .std = stdlevel,
      .debug = FLAG_DEBUG,
      .warnings = FLAG_WARNINGS_VERBOSE,
      .error = FLAG_ERROR_MAX,
      .optimization = FLAG_OPTIMIZATION_BASIC
    });
    AddIncludePaths(orionpp_compiler, "./include");
    AddFile(orionpp_compiler, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orionpp_compiler, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orionpp_compiler);


    char command[1024];
    snprintf(command, 1024, "%s %s -o ./build/human_orionpp_arith", orionpp_compiler.outputPath, "./examples/arith.hopp"); // test
    RunCommand(command);
    // RunCommand("./build/human_orionpp_arith");

    snprintf(command, 1024, "%s %s -o ./build/human_orionpp_cf", orionpp_compiler.outputPath, "./examples/cf.hopp"); // test compilation of 
    RunCommand(command);
    // RunCommand("./build/human_orionpp_cf");
  }
  EndBuild();
}