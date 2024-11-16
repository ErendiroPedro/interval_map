# Interval Map

A robust C++ implementation of an interval map data structure that efficiently manages key-value associations across intervals. This data structure is particularly useful for applications requiring range-based value assignments and queries.

## Overview

The interval map maintains a mapping of intervals to values, where each interval is defined by a begin and end key. It provides efficient operations for:
- Assigning values to intervals `[keyBegin, keyEnd)`
- Querying values for any given key
- Handling overlapping intervals automatically
- Maintaining a canonical form for optimal memory usage

## Features

- **Efficient Value Assignment**: Assign values to half-open intervals `[keyBegin, keyEnd)`
- **Fast Lookup**: O(log n) complexity for value queries
- **Memory Efficient**: Maintains canonical form by merging adjacent intervals with same values
- **Type-Safe**: Template-based implementation supporting various key and value types
- **Comprehensive Testing**: Includes unit tests and randomized testing for validation

## Project Structure

interval_map/
├── CMakeLists.txt              # CMake build configuration
├── README.md                   # Project description
├── include/                    # Header files
│   ├── interval_map.h          # Template class declaration
│   ├── interval_map_impl.h     # Implementation details for the template class
│   └── interval_map_tester.h   # Test suite header
├── src/                        # Source files
│   ├── main.cpp                # Main program (example usage)
│   └── interval_map_tester.cpp # Test suite implementation
└── build/                      # Build output directory
    └── bin/                    # Executable files
        └── interval_map        # The generated executable

## Requirements

- C++17 or later
- CMake 3.10 or later
- A modern C++ compiler (GCC, Clang, or MSVC)

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/yourusername/interval-map.git
cd interval_map
```

2. Create and enter the build directory:
```bash
mkdir build && cd build
```

3. Generate build files and compile:
```bash
cmake ..
make
```

4. Run the program:
```bash
./bin/interval_map_program
```

## Usage Example

```cpp
#include "interval_map.h"

int main() {
    // Create an interval map with default value 'A'
    interval_map<int, char> imap('A');
    
    // Assign values to intervals
    imap.assign(1, 5, 'B');    // [1,5) -> 'B'
    imap.assign(5, 10, 'C');   // [5,10) -> 'C'
    
    // Query values
    std::cout << "Value at 0: " << imap[0] << std::endl;  // Prints: A
    std::cout << "Value at 3: " << imap[3] << std::endl;  // Prints: B
    std::cout << "Value at 7: " << imap[7] << std::endl;  // Prints: C
    std::cout << "Value at 10: " << imap[10] << std::endl; // Prints: A
    
    return 0;
}
```

## Testing

The project includes a comprehensive test suite that verifies:

1. **Basic Functionality**
   - Simple interval assignments
   - Value queries
   - Default value behavior

2. **Edge Cases**
   - Empty intervals
   - Invalid intervals (end before begin)
   - Single-point intervals

3. **Complex Scenarios**
   - Overlapping intervals
   - Adjacent intervals
   - Boundary conditions

4. **Stress Testing**
   - Random interval assignments
   - Large number of operations
   - Memory efficiency validation

Run the tests with:
```bash
./bin/interval_map_program --run-tests
```

## Implementation Details

The interval map maintains the following invariants:

1. The map is always in canonical form (no adjacent intervals with the same value)
2. Empty intervals are ignored
3. Overlapping intervals are handled by overwriting existing values
4. The default value is used for any key not covered by an interval

### Time Complexity

- Assignment: O(log n) average case
- Query: O(log n)
- Space Complexity: O(n) where n is the number of distinct intervals

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request