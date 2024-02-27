#include <cstddef>
// Classes for storing data
namespace vhx {
class cell {
public:
  char value;

  cell(char x) : value(x) {}
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
   * Invalidates the iterator
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
   * Invalidates the iterator
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
      // Here the index doesm't matter, the buffer doesn't exist yet
      cellIndex = new cell *[size + 1]; // Simple initialization
      size++;
    }
  }

  /*
   * Erases an element at a specified index and pushes back the buffer
   * Invalidates the iterator
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
};
} // namespace vhx
