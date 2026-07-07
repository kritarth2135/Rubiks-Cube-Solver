CC = clang
CFLAGS = -Wall -Werror -std=c23 -g

OBJECTS = main.o rubiks_cube.o moves.o pattern_database.o

rubiks_cube_solver: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o rubiks_cube_solver

rubiks_cube.o: rubiks_cube.c
	$(CC) $(CFLAGS) -c rubiks_cube.c

moves.o: moves.c
	$(CC) $(CFLAGS) -c moves.c

pattern_database.o: pattern_database.c
	$(CC) $(CFLAGS) -c pattern_database.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o
	rm -f rubiks_cube_solver