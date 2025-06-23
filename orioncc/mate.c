#define MATE_IMPLEMENTATION
#include "../mate.h"
#include "../matearg.h"

i32 main(int argc, const char *argv[]) {
  struct Arguments args;
  if (argparse(argc, argv, &args) == 1) {
    return 1;
  }

  StartBuild();
  {
    Executable orioncc_program = CreateExecutable((ExecutableOptions){
      .output = "occ", // Orion C Compiler
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orioncc_program, "./include");
    AddFile(orioncc_program, "./src/*.c");
    if (isLinux()) {
      LinkSystemLibraries(orioncc_program, "m"); // Add math only if on linux since MSVC includes this on STD
    }
    InstallExecutable(orioncc_program);

    if (args.execute_commands) {
      // Test both binary and human formats
      const char *command1 = "./build/occ ./examples/test.c -o ./build/test_binary.orionpp";
      String fullcommand1 = { .length=60, .data=command1 };

      const char *command2 = "./build/occ ./examples/test.c -f human -o ./build/test_human.hopp";
      String fullcommand2 = { .length=65, .data=command2 };

      RunCommand(fullcommand1);
      RunCommand(fullcommand2);
      
      // Show file sizes for comparison
      const char *command3 = "ls -la ./build/test_binary.orionpp ./build/test_human.hopp";
      String fullcommand3 = { .length=58, .data=command3 };
      RunCommand(fullcommand3);
    }
  }
  EndBuild();
}