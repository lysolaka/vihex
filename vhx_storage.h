#include <cstddef>
#include <cstdint>
// Classes for storing data
namespace vhx {
class cell {
public:
  char value;
  cell *self;

  cell(char x) : value(x), self(this) {}
  /*
   * NCURSES STUFF
   */
};

class buffer {
private:
  cell **cellIndex;
  size_t size;

public:
  // Constructor
  buffer() : cellIndex(nullptr), size(0) {}

  // Destructor
  ~buffer() {
    for (size_t i = 0; i < size; i++)
      delete cellIndex[i];
  }

  // Access operator
  cell &operator[](size_t idx) { return *cellIndex[idx]; }

  struct _iterator {
    using value_t = cell;
    using pointer = cell **;
    using reference = cell &;

  private:
    pointer i_ptr;

  public:
    _iterator(pointer p) : i_ptr(p) {}

    reference operator*() const { return **i_ptr; }
    _iterator &operator++() {
      i_ptr++;
      return *this;
    }
    friend bool operator!=(const _iterator &a, const _iterator &b) {
      return a.i_ptr != b.i_ptr;
    }
  };

  _iterator begin() { return _iterator(&cellIndex[0]); }
  _iterator end() { return _iterator(&cellIndex[size]); }

  // Simple get size
  size_t getSize() { return size; }

  /*
   * Adds an element with value newVal to the end of the buffer
   */
  void append(const char &newVal) {
    cell *newCell = new cell(newVal);

    if (cellIndex != nullptr) {
      cell **newIndex = new cell *[size + 1]; // Create new index

      for (size_t i = 0; i < size; i++)
        newIndex[i] = cellIndex[i]; // Copy the old index

      delete[] cellIndex; // Remove the old index
      size++;             // and set a new one
      cellIndex = newIndex;
    } else {
      cellIndex = new cell *[size + 1]; // Simple initialization
      size++;
    }

    cellIndex[size - 1] = newCell; // Add the new element
  }

  /*
   * Inserts an element with value newVal at index idx,
   * The value at that index will be newVal
   */
  void insert(const char &newVal, const size_t idx) {
    cell *newCell = new cell(newVal);

    if (cellIndex != nullptr) {
      cell **newIndex = new cell *[size + 1]; // Create new index

      for (size_t i = 0; i < idx; i++) // Copy over the old index
        newIndex[i] = cellIndex[i];    // up to idx

      newIndex[idx] = newCell; // Insert the new value

      for (size_t i = idx; i < size; i++) // Copy over the rest
        newIndex[i + 1] = cellIndex[i];

      delete[] cellIndex; // Remove the old index
      size++;             // and set a new one
      cellIndex = newIndex;
    } else {
      // Here the index doesn't matter, the buffer doesn't exist yet
      cellIndex = new cell *[size + 1]; // Simple initialization
      size++;
    }
  }

  /*
   * Erases an element at a specified index and pushes back the buffer
   */
  void erase(size_t idx) {
    delete cellIndex[idx]; // Delete the element

    cell **newIndex = new cell *[size - 1]; // Create new index
    for (size_t i = 0; i < idx; i++)        // Copy over the old index
      newIndex[i] = cellIndex[i];           // up to idx

    for (size_t i = idx; i < size - 1; i++) // Copy over the rest
      newIndex[i] = cellIndex[i + 1];       // overwriting the erased element

    size--; // Update properties
    cellIndex = newIndex;
  }

  /*
   * Erases an element by pointer
   */
  void erase(const cell *cell) {
    size_t i = 0;
    while (cellIndex[i] != cell) // First find its index
      i++;                       // and erase it by index
    erase(i);
  }

  /*
   * Checks if a line is full of zeros
   * Always returns false if the line is at the end
   * A line is a 16 cell batch, always from an offset of 0 from the beggining
   * Starting index of a line is 16 * line
   */
  bool isZeroLine(const size_t &line) {
    if ((line * 16 + 15) >= size) // Return false for the last line
      return false;               // to avoid segfault
    uint8_t zeroCount = 0;
    for (uint8_t i = 0; i < 16; i++) { // Counting the zeros
      if (cellIndex[line * 16 + i]->value == 0)
        zeroCount++;
    }
    if (zeroCount == 16) // Check and return
      return true;
    return false;
  }
};

class cout_buffer {
private:
  char *buf;
  size_t size;

public:
  // Constructor
  cout_buffer(const size_t &len) : buf(nullptr), size(len) {
    buf = new char[len];
  }

  // Destructor
  ~cout_buffer() { delete[] buf; }

  struct _iterator {
    using value_t = char;
    using ptr = char *;
    using ref = char &;

  private:
    ptr i_ptr;

  public:
    _iterator(ptr p) : i_ptr(p) {}

    ref operator*() const { return *i_ptr; }

    _iterator &operator++() {
      i_ptr++;
      return *this;
    }

    friend bool operator!=(const _iterator &a, const _iterator &b) {
      return a.i_ptr != b.i_ptr;
    }
  };

  _iterator begin() { return _iterator(buf); }
  _iterator end() { return _iterator(buf + size - 1); }

  char &operator[](size_t idx) { return buf[idx]; }

  /*
   * Checks if a line is full of zeros
   * Always returns false if the line is at the end
   * A line is a 16 cell batch, always from an offset of 0 from the beggining
   * Starting index of a line is 16 * line
   */
  bool isZeroLine(const size_t &line) {
    if ((line * 16 + 15) >= size) // Return false for the last line
      return false;               // to avoid segfault
    uint8_t zeroCount = 0;
    for (uint8_t i = 0; i < 16; i++) { // Counting the zeros
      if (buf[line * 16 + i] == 0)
        zeroCount++;
    }
    if (zeroCount == 16) // Check and return
      return true;
    return false;
  }
};
} // namespace vhx
