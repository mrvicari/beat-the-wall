# Executable file name

EXE = TwoPlayerPong

# Source code files

SRC = twoPlayerPong.c

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
