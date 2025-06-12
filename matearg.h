
struct Arguments {
  STDFlag stdlevel;
  DebugFlag debuglevel;
  WarningsFlag warninglevel;
  ErrorFormatFlag errorfmt;
  OptimizationFlag optlevel;
};

void argreset(Arguments *args) {
  args->stdlevel = FLAG_STD_C2X;
  args->debuglevel = FLAG_DEBUG;
  args->warninglevel = FLAG_WARNINGS_VERBOSE;
  args->errorfmt = FLAG_ERROR;
  args->optlevel = FLAG_OPTIMIZATION;
}


int argparse(int argc, char **argv, Arguments *args) {
  argreset(args);

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

  return 0;
}