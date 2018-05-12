# Executable file name

EXE = BeatTheWall

# Source code files

SRC = main.c setup.c render.c movement.c gameplay.c

# Compiler and flags

CC = cc
CFLAGS = -g -Wall
LFLAGS = -lSDL2 -lm

# Convert .c -> .o files

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule for making object files

OBJ = $(SRC:.c=.o)

# Rule for making executable

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LFLAGS)

# Rule for clean up
clean:
	rm -f $(OBJ) $(EXE)

# Dependencies

main.o:	main.c	setup.h	render.h	movement.h	gameplay.h	data.h
setup.o:	setup.c	setup.h	data.h
render.o:	render.c	render.h	setup.h	data.h
movement.o:	movement.c	movement.h	render.h	setup.h	data.h
gameplay.o:	gameplay.c	gameplay.h	movement.h	render.h	setup.h	data.h
