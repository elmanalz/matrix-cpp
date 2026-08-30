# matrix-cpp

A dense matrix class in modern C++17, written from scratch to study value semantics and RAII.
Header-only, no dependencies.

```cpp
#include "matrix.h"

Matrix a(2, 3);
a.at(0, 0) = 1.0;
a.at(1, 2) = 6.0;

Matrix b = a * 2.0;                  // scalar multiplication
Matrix c = a + b;                    // elementwise addition
Matrix d = a * a.transpose();        // 2x3 times 3x2 gives 2x2

std::cout << d;
```


## Features

| Operation | Signature |
|---|---|
| Construction | `Matrix(size_t rows, size_t cols)` — zero-filled |
| Element access | `at(r, c)` — mutable and `const` overloads, bounds-checked |
| Shape | `rows()`, `cols()` |
| Addition, subtraction | `A + B`, `A - B` |
| Matrix multiplication | `A * B` — O(r₁·c₁·c₂) |
| Scalar multiplication | `A * s` and `s * A` |
| Transpose | `A.transpose()` |
| Comparison | `A == B`, `A != B` — within a tolerance |
| Stream output | `std::cout << A` |

Every failure throws: `std::out_of_range` for a bad index, `std::invalid_argument` for a shape
mismatch. No operation silently returns a wrong result.

## Build

Requires a C++17 compiler and `make`.

```bash
make            # build the demo and the test suite
make run        # build and run the tests
make run-demo   # build and run the demonstration program
make clean      # remove the binaries
```

Or with no build system at all:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic tests.cpp -o tests && ./tests
```

Compiles with zero warnings under `-Wall -Wextra -pedantic`.

## Layout

```
matrix.h     the Matrix class (header-only)
main.cpp     demonstration program
tests.cpp    test suite
Makefile     build rules
```

## License

MIT.
