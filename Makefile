CC = clang
CFLAGS = -Wall -Werror -std=c23 -g
LIBS =

OBJECTS = main.o rubiks_cube.o moves.o encoding.o pattern_database.o

rubiks_cube_solver: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o rubiks_cube_solver $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f rubiks_cube_solver

clean_db:
	rm -f corner_db.bin
	rm -f first_six_edge_db.bin
	rm -f last_six_edge_db.bin