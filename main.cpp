#include <cstring>

#include "vihex.h"

void cursesEditor(char *filename) {
  return;
}

int main(int argc, char *argv[]) {
  // Parsing arguments
  // Case 1: error
  if (argc < 2 || argc > 4) {
    printHelp();
    return -1;
  // Case 2: passed filename or a flag
  } else if (argc == 2) {
    if (!std::strcmp(argv[1], "-h") || !std::strcmp(argv[1], "--help"))
      printHelp();
    else
      cursesEditor(argv[1]);
  // Case 2: passed filename and: a flag or double flag
  } else if (argc == 3) {
    if (!std::strcmp(argv[1], "-p") || !std::strcmp(argv[1], "--print"))
      printCout(argv[2], NORMAL);
    else if (!std::strcmp(argv[1], "-pq"))
      printCout(argv[2], QUIET);
    else if (!std::strcmp(argv[1], "-e") || !std::strcmp(argv[1], "--editor"))
      cursesEditor(argv[2]);
    else
      printHelp();
  // Case 3: passed filename and two flags
  } else {
    if (!std::strcmp(argv[1], "-p") || !std::strcmp(argv[1], "--print") &&
        !std::strcmp(argv[2], "-q") || !std::strcmp(argv[2], "-quiet"))
      printCout(argv[3], QUIET);
    else
      printHelp();
  }

  return 0;
}
