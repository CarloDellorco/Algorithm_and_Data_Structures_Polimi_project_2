# Highway Path Planner

Final project for **Algorithms and Data Structures** (Politecnico di Milano, A.Y. 2022-2023).

The project is written in **C11** and uses only the C standard library. It reads commands from `stdin` and writes results to `stdout`. No multithreading is used.

## The problem

A highway is a sequence of service stations. Each station is identified by its distance (a non-negative integer, in km) from the start of the highway, and no two stations share the same distance.

Each station has a fleet of rental electric cars (at most 512), each with an autonomy expressed as a positive integer number of km. Renting a car at station `s` allows the driver to reach every station whose distance from `s` is less than or equal to the car's autonomy.

A trip is a sequence of stops. At every stop the driver rents a new car, and the highway can be driven in **both directions** without going back during the trip. The goal is to find the path between two stations with the **fewest stops**.

### Tie-breaking

If several paths have the same minimum number of stops, the path that favors the stops closest to the start of the highway is chosen, comparing paths starting from their final stops. This rule does not depend on the driving direction.

Example: with stations at 20, 30, 45 and 50, the correct path from 20 to 50 is `20 30 50` (and not `20 45 50`), while from 50 to 20 it is `50 30 20`.

## Commands

All integers fit in 32 bits. One command per line.

| Command | Description | Output |
|---|---|---|
| `aggiungi-stazione d n a1 ... an` | Adds a station at distance `d` with `n` cars of autonomy `a1 ... an`. Does nothing if the station already exists. | `aggiunta` / `non aggiunta` |
| `demolisci-stazione d` | Removes the station at distance `d`, if it exists. | `demolita` / `non demolita` |
| `aggiungi-auto d a` | Adds a car with autonomy `a` to the station at `d`, if it exists. Duplicate autonomies are allowed. | `aggiunta` / `non aggiunta` |
| `rottama-auto d a` | Removes one car with autonomy `a` from the station at `d`, if both exist. | `rottamata` / `non rottamata` |
| `pianifica-percorso s t` | Plans the path from station `s` to station `t` (both exist). Does not modify stations or fleets. | Stops in travel order, separated by spaces, or `nessun percorso` |

For `pianifica-percorso`, start and end are always included. If they coincide, the station is printed only once.

### Example

```
aggiungi-stazione 20 3 5 10 15
aggiunta
aggiungi-stazione 30 0
aggiunta
pianifica-percorso 30 20
nessun percorso
```

## Implementation overview

- Stations are stored in a singly linked list sorted by distance.
- Each station owns a linked list of its cars and caches its maximum autonomy, which is recomputed when the best car is scrapped.
- Path planning copies the station list into an array of pointers and runs a Dijkstra-style search where every reachable hop has cost 1. Reachability is checked in both directions around a station, using its maximum autonomy.
- All dynamically allocated memory is released before exit.

## Repository structure

```
.
├── CMakeLists.txt      # build configuration
├── Files/              # test inputs (open_31.txt, ...) and expected outputs (open_31.output.txt, ...)
└── main.c
```

## Build

With CMake, from the project root:

```bash
cmake -S . -B build
cmake --build build
```

The executable is `build/Algorithm_project_2023` (`.exe` on Windows).

Direct compilation with `gcc`:

```bash
gcc -Wall -Werror -O2 -g3 source.c -o executable
```

## Running and testing

### Windows (cmd)

```bat
build\Algorithm_project_2023.exe < Files\open_31.txt > open_31.actual.txt
fc Files\open_31.output.txt open_31.actual.txt
```

If everything is correct, `fc` reports `FC: no differences encountered` (`FC: nessuna differenza riscontrata` on an Italian system).

### Linux / macOS

```bash
./build/Algorithm_project_2023 < Files/open_31.txt > open_31.actual.txt
diff Files/open_31.output.txt open_31.actual.txt
```

If `diff` prints nothing, the two files are identical.

## Debugging and profiling (optional)

```bash
# Memory access errors
gcc -g3 -fsanitize=address source.c -o executable

# Memory leaks
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./executable < Files/open_31.txt

# Time per function / allocated memory
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./executable < Files/open_31.txt
valgrind --tool=massif --massif-out-file=massif.out ./executable < Files/open_31.txt
```

## Notes

Individual project developed for a university course.