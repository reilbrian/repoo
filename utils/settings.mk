# Libraries
LIBS=commons pthread readline m

# Custom libraries' paths
SHARED_LIBPATHS=
STATIC_LIBPATHS=

# Compiler flags
CDEBUG=-g -Wall -DDEBUG -fdiagnostics-color=always
CRELEASE=-O3 -Wall -DNDEBUG

# Source files (*.c) to be excluded from tests compilation
TEST_EXCLUDE=src/main.c
