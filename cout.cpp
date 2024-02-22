#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "vihex.h"

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
            << "the curses editor is the default\n"
            << " The -p and -q options can be combined into -pq\n";
}

int printCout(char *filename, coutFormat format) {
  std::FILE *hxFile;

  hxFile = std::fopen(filename, "r");
  if (hxFile == nullptr) {
    std::cout << "Failed opening the file: " << filename << '\n'
              << "Use: vihex -h for help\n";
    return -1;
  }

  std::vector<std::array<char, 16>> hxBuffer;
  struct {
    uint32_t rows;
    uint8_t colsLastRow;
  } hxBufferInfo;

  char currentChar = 0;
  uint32_t rowLabel = 0;

  hxBufferInfo.rows = 0;
  hxBufferInfo.colsLastRow = 0;
  hxBuffer.resize(1);
  // Loop reading the file
  do {
    currentChar = std::fgetc(hxFile);
    // Handling getting offset bigger than 0xF
    if (hxBufferInfo.colsLastRow > 15) {
      hxBufferInfo.colsLastRow = 0;
      hxBufferInfo.rows++;
      hxBuffer.resize(hxBufferInfo.rows + 1);
    }
    hxBuffer[hxBufferInfo.rows][hxBufferInfo.colsLastRow] = currentChar;
    hxBufferInfo.colsLastRow++;
  } while (!std::feof(hxFile));
  std::fclose(hxFile);

  switch (format) {
    case NORMAL:
      for (uint32_t y = 0; y < hxBufferInfo.rows - 1; y++) {
        std::printf("%.7X ", rowLabel);
        for (uint8_t x = 0; x < 16; x++)
          std::printf("%.2X ", (unsigned char) hxBuffer[y][x]);
        std::putc('\n', stdout);
        rowLabel += 0x10;
      }
      std::printf("%.7X ", rowLabel);
      for (uint8_t x = 0; x < hxBufferInfo.colsLastRow; x++)
        std::printf("%.2X ", (unsigned char)
                             hxBuffer[hxBufferInfo.rows - 1][x]);
      std::putc('\n', stdout);
      rowLabel += hxBufferInfo.colsLastRow;
      std::printf("%.7X ", rowLabel);
      break;
    case QUIET:
        for (uint32_t y = 0; y < hxBufferInfo.rows - 1; y++) {
        for (uint8_t x = 0; x < 16; x++)
          std::printf("%.2X ", (unsigned char) hxBuffer[y][x]);
        std::putc('\n', stdout);
      }
      for (uint8_t x = 0; x < hxBufferInfo.colsLastRow; x++)
        std::printf("%.2X ", (unsigned char)
                             hxBuffer[hxBufferInfo.rows - 1][x]);
      break;
  }

  return 0;
}
