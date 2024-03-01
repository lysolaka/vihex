#include <cstring>

#include "vihex.h"

int cursesEditor(char *filename) {
  return 0;
}
/*
 * ATTENTION: Never touch this main file mess again, I promise I will learn for the future
 */
int main(int argc, char *argv[]) {
  int retVal;
  if (argc < 2 || argc > 5) {
    printHelp();
    retVal =  -1;
  } else if (argc == 2) {
    if (!std::strcmp(argv[1], "-h") || !std::strcmp(argv[1], "--help")) {
      printHelp();
      retVal = -1;
    } else {
      retVal = cursesEditor(argv[1]);
    }
  } else if (argc == 3) {
    if (!std::strcmp(argv[1], "-p") || !std::strcmp(argv[1], "--print")) {
      retVal = printCout(argv[2], NORMAL);
    } else if (!std::strcmp(argv[1], "-pq")) {
      retVal = printCout(argv[2], QUIET);
    } else if (!std::strcmp(argv[1], "-pf")) {
      retVal = printCout(argv[2], NORMAL_FULL);
    } else if (!std::strcmp(argv[1], "-pqf")) {
      retVal = printCout(argv[2], QUIET_FULL);
    } else if (!std::strcmp(argv[1], "-e") || !std::strcmp(argv[1], "--editor")) {
      retVal = cursesEditor(argv[2]);
    } else {
      printHelp();
      retVal = -1;
    }
  } else if (argc == 4) {
    if (!std::strcmp(argv[1], "-p") || !std::strcmp(argv[1], "--print")) {
      if (!std::strcmp(argv[2], "-q") || !std::strcmp(argv[2], "--quiet")) {
        retVal = printCout(argv[3], QUIET);
      } else if (!std::strcmp(argv[2], "-f") || !std::strcmp(argv[2], "--full")) {
        retVal = printCout(argv[3], NORMAL_FULL);
      }
    } else {
      printHelp();
      retVal = -1;
    }
  } else if (argc == 5) {
    if (!std::strcmp(argv[1], "-p") || !std::strcmp(argv[1], "--print")) {
      if (!std::strcmp(argv[2], "-q") || !std::strcmp(argv[2], "--quiet")) {
        if (!std::strcmp(argv[3], "-f") || !std::strcmp(argv[3], "--full")) {
          retVal = printCout(argv[4], QUIET_FULL);
        }
      } else if (!std::strcmp(argv[2], "-f") || !std::strcmp(argv[2], "--full")) {
        if (!std::strcmp(argv[3], "-q") || !std::strcmp(argv[3], "--quiet")) {
          retVal = printCout(argv[4], QUIET_FULL);
        }
      }
    } else {
      printHelp();
      retVal = -1;
    }
  }
  return retVal;
}
