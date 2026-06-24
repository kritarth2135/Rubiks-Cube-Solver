CC = clang
CFLAGS = -Wall -Werror -std=c23 -g

OBJECTS = main.o rubiks_cube.o

rubiks_cube_solver: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o rubiks_cube_solver

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

rubiks_cube.o: rubiks_cube.c
	$(CC) $(CFLAGS) -c rubiks_cube.c

clean:
	rm -f *.o

clean_all:
	rm -f *.o
	rm -f rubiks_cube_solver