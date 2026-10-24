# Rubiks-Cube-Solver

This is a Rubiks Cube solver written in C. Currently this solver only has an (not so efficient) implementation of the [Korf's Algorithm](https://www.cs.princeton.edu/courses/archive/fall06/cos402/papers/korfrubik.pdf).

## Compilation and running

Run below commands to clone the repo, compile and run the program

```sh
git clone https://github.com/kritarth2135/Rubiks-Cube-Solver.git
cd Rubiks-Cube-Solver/
make
./rubiks_cube_solver
```

When you first run the program you will need to wait for a couple of hours, depending on your machine, for it to generate all the databases to be used by the solver. If you don't wanna wait that long you can directly download them from the [releases page](https://github.com/kritarth2135/Rubiks-Cube-Solver/releases). After you download the databases make a new directory named `db` in the same folder and move the databases inside that folder for the program to detect and load them. If you want to change the location of databases just change the value of `DB_PATH` in `pattern_database.c.`

You can verify your downloads by comparing the SHA256 hash values of the databases with the following:

```
001ae78e2cd3146266c04679d95671340558907f427cfb02eb1e6d1fddd903c1  corner_db.bin
5a9f1b6aa16b9a97ac9c8eee2ba1496176513d9b4ba7f81b46e4b7c5ffe4c104  edge_positions_db.bin
ed5ed5677fe6063fc5a7a8eb46bc0136941d063602cd743f28ebede2bc30e1a2  first_edge_db.bin
1c811aa01800168fd4795f19f45965aecd6bb051ebd32c11ee3466fb11188325  second_edge_db.bin
```

## CLI interface

To make a move enter the [move notations](https://jperm.net/3x3/moves) in any case. Note that only the notation with `w` is accepted for wide moves. This is because the inputs are case insensitive so something like `u` will be treated as `U` rather than `Uw`. All of the 54 moves (18 face moves, 18 wide face moves, 9 middle moves and 9 cube moves) are supported.

Special inputs:

| Input | Action |
| ----- | ------ |
| `q` | Quit |
| `a` | Run the solver|

## Project Structure

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
