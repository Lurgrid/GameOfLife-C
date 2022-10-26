gol_dir = ./gol/
CC = gcc
CC_ = i686-w64-mingw32-gcc
CC_ = x86_64-w64-mingw32-gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings  \
  -O2 \
  -I$(gol_dir)
vpath %.c $(gol_dir)
vpath %.h $(gol_dir)
objects = gol.o main.o
executable = gameoflife

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c gol.h gol.c
gol.o: gol.c gol.h
