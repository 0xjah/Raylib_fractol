CC := clang
CFLAGS := -Wall -Wextra

# Only use cflags during compilation, libs during linking
CFLAGS_COMPILE := $(shell pkg-config --cflags raylib)
LIBS := $(shell pkg-config --libs raylib) -lm

SRC := mandelbrot.c
OBJ := mandelbrot.o
MAIN := mandelbrot

$(MAIN): $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(LIBS)

$(OBJ): $(SRC) mandelbrot.h
	$(CC) $(CFLAGS) $(CFLAGS_COMPILE) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(MAIN)

