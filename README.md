# Rubiks-Cube-Solver

This is a Rubiks Cube solver written in C. Currently this solver only has an (not so efficient) implementation of the [Korf's Algorithm](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf).

### Project Structure

```
├── cube
│   ├── cube.c
│   ├── cube.h
│   ├── moves.c
│   └── moves.h
├── db
│   └── ...
├── db_generation
│   ├── databases.c
│   ├── encoding.c
│   ├── pattern_database.c
│   └── pattern_database.h
├── korfs_solver
│   └── korfs_solver.c
├── solver
│   ├── solver.c
│   └── solver.h
├── main.c
└── Makefile
```

### Compilation and running

Run below commands to clone the repo, compile and run the program

```sh
git clone https://github.com/kritarth2135/Rubiks-Cube-Solver.git
cd Rubiks-Cube-Solver
make
./rubiks_cube_solver
```

If you rename the executibe in the `Makefile` then replace `rubiks_cube_solver` with the new name.

### CLI interface

To make a move enter the [move notations](https://jperm.net/3x3/moves) in any case. Note that only the notation with `w` is accepted for wide moves. This is because the inputs are case insensitive so something like `u` will be treated as `U` rather than `Uw`. All of the 54 moves (18 face moves, 18 wide face moves, 9 middle moves and 9 cube moves) are supported.

Special inputs:
| Input | Action |
| ----- | ------ |
| `q` | Quit |
| `a` | Run the solver|
