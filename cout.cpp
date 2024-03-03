#include <cstddef>
#include <cstdio>
#include <iostream>

#include "vhx_storage.h"
#include "vihex.h"

void printHelp() {
  std::cout << "Usage:\n "
            << "vihex [options] <file>\n\n"
            << "Options:\n"
            << "-e, --editor  Opens the specified file in a curses editor\n"
            << "-p, --print   Prints the hex conents to stdout\n"
            << "-q, --quiet   Used alongside -p, prints the hex contents to "
            << "stdout\n              ommiting offset labels\n"
            << "-f, --full    Used alongside -p, prints full contents of the "
               "file, not skipping\n              duplicate zero lines\n"
            << "-h, --help    Displays this text\n\n"
            << "Note:\n Specifying the -e option is not necessary, "
            << "the curses editor is the default.\n"
            << " The -p and -q options may be combined into -pq.\n"
            << " The -f option may be added to -pq or -p, to form -pqf or -pf."
            << '\n';
}

int printCout(char *filename, coutFormat format) {
  std::FILE *hxFile;
  // Used for all formats:
  size_t lineBreak = 0;
  // Used for non-quiet formats:
  size_t lineNum = 0;
  // Used for non-full formats:
  bool linePrint = true;
  bool zeroLineEncountered = false;
  bool indicateZeroLine = true;

  hxFile = std::fopen(filename, "r");
  if (hxFile == nullptr) {
    std::cout << "Failed opening the file: " << filename << '\n'
              << "Use: vihex -h for help\n";
    return -1;
  }
  // Reading the file:
  // For optimization get the length of the file
  while (!std::feof(hxFile)) {
    std::fgetc(hxFile);
    lineBreak++; // WARNING! Reused lineBreak variable
  }
  // Get back to the beggining
  std::rewind(hxFile);
  // Create the buffer
  vhx::cout_buffer hxBuffer(lineBreak);
  // Read in data
  for (size_t i = 0; i < lineBreak; i++)
    hxBuffer[i] = std::fgetc(hxFile);

  std::fclose(hxFile);

  // Resetting reused variable
  lineBreak = 0;
  // Handling formats
  switch (format) {
  case NORMAL:
    // This solution doesn't print the first line number
    std::printf("%.7zx ", lineNum * 16); // Printing it here
    for (auto i : hxBuffer) {
      if (lineBreak == 16) {
        // Things to happen at the end of a line
        lineBreak = 0;
        lineNum++;
        if (linePrint)
          std::putchar('\n');
        if (hxBuffer.isZeroLine(lineNum)) {
          // Need to display at least one line of zeros to indicate
          if (zeroLineEncountered) {
            linePrint = false;
            // Makes the '*' symbol appear only once per skipped lines
            if (indicateZeroLine) {
              std::printf("*\n");
              indicateZeroLine = false;
            }
          } else {
            // Marks next zero line to be skipped
            zeroLineEncountered = true;
          }
        } else {
          // If the line isn't a zero line, reset the variables to default
          linePrint = true;
          zeroLineEncountered = false;
          indicateZeroLine = true;
        }
        // Display the line number
        if (linePrint)
          std::printf("%.7zx ", lineNum * 16);
      }
      if (linePrint)
        std::printf("%.2x ", (unsigned char)i);
      lineBreak++;
    }
    // Print the last byte offset
    std::printf("\n%.7zx", lineNum * 16 + lineBreak);
    break;
  // This one is similar to the above one, without line numbers
  case QUIET:
    for (auto i : hxBuffer) {
      if (lineBreak == 16) {
        lineBreak = 0;
        lineNum++;
        if (linePrint)
          std::putchar('\n');
        if (hxBuffer.isZeroLine(lineNum)) {
          if (zeroLineEncountered) {
            linePrint = false;
            if (indicateZeroLine) {
              std::printf("*\n");
              indicateZeroLine = false;
            }
          } else {
            zeroLineEncountered = true;
          }
        } else {
          linePrint = true;
          zeroLineEncountered = false;
          indicateZeroLine = true;
        }
      }
      if (linePrint)
        std::printf("%.2x ", (unsigned char)i);
      lineBreak++;
    }
    break;
  case NORMAL_FULL:
    // The first line number doesn't get printed
    std::printf("%.7zx ", lineNum);
    for (auto i : hxBuffer) {
      if (lineBreak == 16) {
        // End of the line behaviour:
        lineBreak = 0;
        // Put a new line
        std::putchar('\n');
        // Increment the line number
        lineNum++;
        std::printf("%.7zx ", lineNum * 16);
      }
      std::printf("%.2x ", (unsigned char)i);
      lineBreak++;
    }
    // Print the last byte offset
    std::printf("\n%.7zx", lineNum * 16 + lineBreak);
    break;
  // Works the same as previous one, without printing the line number
  case QUIET_FULL:
    for (auto i : hxBuffer) {
      std::printf("%.2x ", (unsigned char)i);
      lineBreak++;
      if (!(lineBreak % 16))
        std::putchar('\n');
    }
    break;
  }
  std::putchar('\n');

  return 0;
}
