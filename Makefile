# Make environment for ash

CC=gcc
CFLAGS= -c -g -Wall -pthread $(INCLUDES)

# Files
OBJECT_FILES= src/ash.o src/shell.o

# Productions
all : ash

debug: $(OBJECT_FILES)
	$(CC) $^ -fsanitize=address -o $@

ash : $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) -o $@

ash.o : src/shell.c src/shell.h
	$(CC) $(CFLAGS) $< -o $@

shell.o : src/ash.c src/headers.h
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm -v ash $(OBJECT_FILES)