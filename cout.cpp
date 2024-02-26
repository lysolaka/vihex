#include <cstddef>
#include <cstdio>
#include <iostream>

#include "vihex.h"
#include "vhx_storage.h"

void printHelp() {
  std::cout << "Usage:\n "
            << "vihex [options] <file>\n\n"
            << "Options:\n"
            << "-e, --editor  Opens the specified file in a curses editor\n"
            << "-p, --print   Prints the hex conents to stdout\n"
            << "-q, --quiet   Used alongside -p, prints the hex contents to "
            << "stdout\n              ommiting offset labels\n"
            << "-h, --help    Displays this text\n\n"
            << "Note:\n Specifying the -e option is not necessary, "
            << "the curses editor is the default.\n"
            << " The -p and -q options may be combined into -pq.\n";
}

int printCout(char *filename, coutFormat format) {
  std::FILE *hxFile;
  vhx::buffer hxBuffer;
  size_t lineBreak = 0;
  size_t lineNum = 0;

  hxFile = std::fopen(filename, "r");
  if (hxFile == nullptr) {
    std::cout << "Failed opening the file: " << filename << '\n'
              << "Use: vihex -h for help\n";
    return -1;
  }

  while (!std::feof(hxFile))
    hxBuffer.append(std::getc(hxFile));
  // Erase the EOF character at the end of the buffer
  hxBuffer.erase(hxBuffer.getSize() - 1);

  std::fclose(hxFile);

  switch (format) {
    case NORMAL:
      std::printf("%.7zX ", lineNum);
      for (auto i : hxBuffer) {
        std::printf("%.2X ", (unsigned char) i->value);
        lineBreak++;
        if (!(lineBreak % 16)) {
          std::putchar('\n');
          lineNum += 16;
          std::printf("%.7zX ", lineNum);
        }
      }
      std::printf("\n%.7zX", lineNum + lineBreak % 16);
      break;
    case QUIET:
      for (auto i : hxBuffer) {
        std::printf("%.2X ", (unsigned char) i->value);
        lineBreak++;
        if (!(lineBreak % 16))
          std::putchar('\n');
      }
      break;
  }
  std::putchar('\n');

  return 0;
}
