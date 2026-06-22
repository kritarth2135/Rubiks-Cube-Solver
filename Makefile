clean: rubiks_cube_solver
	rm -f *.o

rubiks_cube_solver: main.o rubiks_cube.o
	clang main.o rubiks_cube.o -o rubiks_cube_solver

object_files:
	clang -Wall -Werror -std=c23 -c main.c rubiks_cube.c
