rubiks_cube:
	clang -Wall -Werror -std=c99 -fsanitize=address main.c -o rubiks_cube

clean:
	rm -f *.o
