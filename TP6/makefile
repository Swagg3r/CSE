# Flags de compilation
CC=gcc
CFLAGS= -Wall -g
LDFLAGS=
 
# Exécutables
EXEC=test
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
 
.PHONY: clean mrproper
 
all: $(EXEC)
 
test: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
 
stdes.c: stdes.h
 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
clean:
	rm -f *.o
 
mrproper: clean
	rm -f $(EXEC)