#define MATE_IMPLEMENTATION
#include "mate.h"


i32 main(int argc, const char *argv[]) {
  /* Options Quick Reference
    typedef enum {
      FLAG_WARNINGS_NONE = 1, // -w
      FLAG_WARNINGS_MINIMAL,  // -Wall
      FLAG_WARNINGS,          // -Wall -Wextra
      FLAG_WARNINGS_VERBOSE,  // -Wall -Wextra -Wpedantic
    } WarningsFlag;

    typedef enum {
      FLAG_DEBUG_MINIMAL = 1, // -g1
      FLAG_DEBUG_MEDIUM,      // -g/g2
      FLAG_DEBUG,             // -g3
    } DebugFlag;

    typedef enum {
      FLAG_OPTIMIZATION_NONE = 1,  // -O0
      FLAG_OPTIMIZATION_BASIC,     // -O1
      FLAG_OPTIMIZATION,           // -O2
      FLAG_OPTIMIZATION_SIZE,      // -Os
      FLAG_OPTIMIZATION_AGGRESSIVE // -O3
    } OptimizationFlag;

    typedef enum {
      FLAG_STD_C99 = 1, // -std=c99
      FLAG_STD_C11,     // -std=c11
      FLAG_STD_C17,     // -std=c17
      FLAG_STD_C23,     // -std=c23
      FLAG_STD_C2X      // -std=c2x
    } STDFlag;

    typedef enum {
      FLAG_ERROR,    // -fdiagnostics-color=always
      FLAG_ERROR_MAX // -fdiagnostics-color=always -fcolor-diagnostics ...
    } ErrorFormatFlag;
  */
  STDFlag stdlevel = FLAG_STD_C2X;
  DebugFlag debuglevel = FLAG_DEBUG;
  WarningsFlag warninglevel = FLAG_WARNINGS_VERBOSE;
  ErrorFormatFlag errorfmt = FLAG_ERROR;
  OptimizationFlag optlevel = FLAG_OPTIMIZATION;

  { // Argument Parsing
    if (argc > 1) {
      for (int arg = 1; arg < argc; ++arg) {
        if (argv[arg][0] == '\0') {
          continue;
        }

        switch (argv[arg][1]) {
        case 'h':
          printf(
            "Build Help Menu:\n"
            " -h = Display this help menu\n"
            " -g{opt<int> = 3} = apply debug of {0:minimal,1:medium,2:full}\n"
            " -o{opt<int,char> = 2} = apply optimization of {0:none,1:basic,2:default,3:aggressive,s/S:size}\n"
            " -w{opt<int,char> = 3} = apply warning of {0:none,1:minimal,2:extra,3:pedantic}\n"
          );
          return 1;
        case 'g':
          if (argv[arg][2] != '\0') {
            if (argv[arg][2] == '1') {
              debuglevel = FLAG_DEBUG_MINIMAL;
            } else if (argv[arg][2] == '2') {
              debuglevel = FLAG_DEBUG_MEDIUM;
            } else if (argv[arg][2] == '3') {
              debuglevel = FLAG_DEBUG;
            } else {
              LogError("Debug level argument is invalid %d\n", argv[arg][2]);
              return 1;
            }
          }
          break;
        case 'o':
          if (argv[arg][2] != '\0') {
            if (argv[arg][2] == '0') {
              optlevel = FLAG_OPTIMIZATION_NONE;
            } else if (argv[arg][2] == '1') {
              optlevel = FLAG_OPTIMIZATION_BASIC;
            } else if (argv[arg][2] == '2') {
              optlevel = FLAG_OPTIMIZATION;
            } else if (argv[arg][2] == '3') {
              optlevel = FLAG_OPTIMIZATION_AGGRESSIVE;
            } else if (argv[arg][2] == 's' || argv[arg][2] == 'S') {
              optlevel = FLAG_OPTIMIZATION_SIZE;
            } else {
              LogError("Debug level argument is invalid %d\n", argv[arg][2]);
              return 1;
            }
          }
          break;
        case 'w':
          if (argv[arg][2] != '\0') {
            if (argv[arg][2] == '0') {
              warninglevel = FLAG_WARNINGS_NONE;
            } else if (argv[arg][2] == '1') {
              warninglevel = FLAG_WARNINGS_MINIMAL;
            } else if (argv[arg][2] == '2') {
              warninglevel = FLAG_WARNINGS;
            } else if (argv[arg][2] == '3') {
              warninglevel = FLAG_WARNINGS_VERBOSE;
            } else {
              LogError("Debug level argument is invalid %d\n", argv[arg][2]);
              return 1;
            }
          }
          break;
        default:
          LogError("Invalid argument %s\n", argv[arg]);
          return 1;
        }
      }
    }
  }
  
  StartBuild();
  {
    // libcoil-dev
    StaticLib libcoil = CreateStaticLib((StaticLibOptions){
      .output = "libcoil-dev",
      .std = stdlevel,
      .debug = debuglevel,
      .warnings = warninglevel,
      .error = errorfmt,
      .optimization = optlevel,
    });
    AddIncludePaths(libcoil, "./toolchain/libcoil-dev/include");
    AddFile(libcoil, "./toolchain/libcoil-dev/src/*.c");
    InstallStaticLib(libcoil);

    Executable libcoil_tests = CreateExecutable((ExecutableOptions){
      .output = "libcoil-test",
      // .flags = "./build/libcoil-dev.a",
      .std = stdlevel,
      .debug = FLAG_DEBUG,
      .warnings = FLAG_WARNINGS_VERBOSE,
      .error = FLAG_ERROR_MAX,
      .optimization = FLAG_OPTIMIZATION_BASIC
    });
    AddIncludePaths(libcoil_tests, "./toolchain/libcoil-dev/include");
    AddFile(libcoil_tests, "./toolchain/libcoil-dev/tests/*.c");
    AddLibraryPaths(libcoil_tests, "./build");
    LinkSystemLibraries(libcoil_tests, "coil-dev");
    InstallExecutable(libcoil_tests);
    RunCommand(libcoil_tests.outputPath);
    

    // other toolchain componenets in the future...
    // if (isLinux()) {
    //   LinkSystemLibraries({toolchain_executable}, "m"); // Add math only if on linux since MSVC includes this on STD
    // }
  }
  EndBuild();
}