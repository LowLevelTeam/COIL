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
    StaticLib orion_obj_library = CreateStaticLib((StaticLibOptions){
      .output = "liborionobj-dev.a",
      .std = args.stdlevel,
      .debug = args.debuglevel,
      .warnings = args.warninglevel,
      .error = args.errorfmt,
      .optimization = args.optlevel
    });
    AddIncludePaths(orion_obj_library, "./include");
    AddFile(orion_obj_library, "./src/*.c");
    InstallStaticLib(orion_obj_library);
  }
  EndBuild();
}
