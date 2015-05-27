CC=gcc
CFLAGS= -Wall -I.
EXE_NAME = fork_wait
FILES = fork_child_wait.o
all: $(FILES)
	$(CC) -o $(EXE_NAME) $(FILES) $(CFLAGS)

clean:
	rm -f *.o

run:
	./$(EXE_NAME)

rebuild: clean all run

