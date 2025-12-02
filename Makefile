CC := clang
CCFLAGS := -Wall -Wextra

LIBS := `pkg-config --libs --cflags raylib`

SRC := mandelbrot.c
OBJ := mandelbrot.o
MAIN := mandelbrot


$(MAIN):$(OBJ)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJ) $(LIBS)

$(OBJ):$(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ) $(LIBS)


clean:
	rm $(OBJ); rm $(MAIN)
