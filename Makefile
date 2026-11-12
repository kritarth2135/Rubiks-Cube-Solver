CC = clang
CFLAGS = -Wall -Werror -std=c23 -g -I.
LIBS = -lncurses

DIRS = cube solver db_generation korfs_solver ui
SRCS = main.c $(wildcard $(foreach dir, $(DIRS), $(dir)/*.c))
HDRS = $(wildcard $(foreach dir, $(DIRS), $(dir)/*.h))
EXECUTIBLE = rubiks_cube_solver

$(EXECUTIBLE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXECUTIBLE) $(LIBS)

.PHONY: clean
clean:
	rm -f rubiks_cube_solver
