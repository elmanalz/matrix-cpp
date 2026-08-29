# =============================================================================
#  Makefile for matrix-cpp
#
#  Usage:
#     make            build both programs
#     make run        build and run the test suite
#     make run-demo   build and run the demonstration program
#     make clean      delete everything that was built
#
#  On MSYS2 you may need to install make first:   pacman -S make
# =============================================================================

# --- Settings ----------------------------------------------------------------

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -MMD -MP

# MinGW adds .exe to output binaries; Linux and macOS do not.
# The OS variable is set to Windows_NT on Windows, and MSYS2 inherits it.
ifeq ($(OS),Windows_NT)
    EXT := .exe
else
    EXT :=
endif

DEMO  := demo$(EXT)
TESTS := tests$(EXT)

# --- Build rules -------------------------------------------------------------

# The first target is what a bare `make` builds, so keep `all` at the top.
all: $(DEMO) $(TESTS)

$(DEMO): main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(TESTS): tests.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# --- Convenience targets -----------------------------------------------------

run: $(TESTS)
	./$(TESTS)

run-demo: $(DEMO)
	./$(DEMO)

clean:
	rm -f $(DEMO) $(TESTS) *.d

# These are not filenames, they are command names.
.PHONY: all run run-demo clean

# --- Header dependency tracking ----------------------------------------------
# -MMD (in CXXFLAGS) makes the compiler write a .d file next to each program
# listing every header that source included -- here, matrix.h. Including those
# files below teaches make to rebuild when a header changes, without you having
# to list headers by hand. -MP adds dummy rules so deleting a header doesn't
# break the build. The leading '-' means "no error if the files don't exist yet".
-include demo.d tests.d
