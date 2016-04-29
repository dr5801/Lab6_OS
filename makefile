#####################################################
## Lab 6      : Virtual Memory
## Programmer : Drew Rife
## Course     : CMPE 320
## Section    : 2(11-12:50pm)
## Instructor : S. Lee
#####################################################
CC = gcc
CFLAGS = -g
DEPS = VM.h
OBJ = VM.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

virtmem: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

clean:
	$(RM) $(OBJ) virtmem
