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

```
$ make run-demo
== Matrix multiplication ==

P * Q  (2x3 times 3x2 gives 2x2) (2x2)
58.00 64.00
139.00 154.00

(P * Q)^T == Q^T * P^T ?  yes
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

## Design

**Flat, contiguous storage.** Elements live in a single `std::vector<double>` of length
`rows * cols`, indexed row-major as `data[r * cols + c]` — not a vector-of-vectors. That is one
allocation instead of one per row, and contiguous memory is far friendlier to the CPU cache. It
is also the layout NumPy and BLAS use, for the same reason. A pleasant side effect is that the
rectangular shape is guaranteed by construction: rows cannot have different lengths because
there are no rows, only a shape and a buffer.

**Rule of Zero.** The class declares no destructor, no copy constructor, and no assignment
operator, and contains no raw pointers. `std::vector` already manages its own storage correctly,
so the compiler-generated versions are correct by construction — copies are deep, and leaks and
double-frees are not merely avoided but unrepresentable. The `Value semantics` section of the
demo program shows a copy being modified without touching the original.

**Encapsulated invariant.** `rows_`, `cols_`, and `data_` are private, so the invariant
`data_.size() == rows_ * cols_` cannot be broken from outside. This matters more than it looks:
if a caller could set `rows_` directly, `at()`'s bounds check would still pass while describing a
buffer that no longer exists.

**Tolerance-based comparison.** `operator==` compares elementwise within a tolerance rather than
bitwise, because exact floating-point equality is not a useful test after arithmetic — `0.1 + 0.2`
is not `0.3`. `approx_equal(other, tol)` exposes the tolerance for callers who need a different
one.

**Header-only.** Every member function is defined inside the class body, which makes it
implicitly `inline`, and the two free operators are marked `inline` explicitly. The header can
therefore be included from any number of translation units without violating the
one-definition rule. Using the library means copying one file.

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

## Tests

43 checks in `tests.cpp`, using a small hand-written harness rather than an external framework.
Three kinds, and the mix is deliberate:

- **Known answers** — multiplications worked out by hand and hardcoded.
- **Properties** that must hold for *any* input, which is the stronger form:
  `(A + B) - B == A`, `A * I == A`, `transpose(transpose(A)) == A`, and
  `(A · B)ᵀ == Bᵀ · Aᵀ`. The last is hard to satisfy by accident.
- **Failure modes** — out-of-range access and shape mismatches must throw the correct exception
  type, and a wrapped negative index (`size_t(-1)`) must be rejected.

A further section pins down const-correctness: those checks compile only while the relevant
methods keep their `const`, so the property cannot regress silently.

```
$ make run
43 / 43 checks passed
ALL TESTS PASSED
```

## Layout

```
matrix.h     the Matrix class (header-only)
main.cpp     demonstration program
tests.cpp    test suite
Makefile     build rules
```

## Roadmap

- Cache-blocked matrix multiplication, benchmarked against the naive triple loop
- Move semantics, so large temporaries are not copied
- A templated element type (`Matrix<float>`, `Matrix<double>`)
- A relative rather than absolute comparison tolerance, which matters once results grow large

## License

MIT.
