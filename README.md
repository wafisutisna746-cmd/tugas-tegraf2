# Graph Theory Homework

## Identity
GROUP 8

|    NRP     |           Nama             |
| :--------: |       :------------:       |
| 5025251260 | Aqilah Ibrahim             |
| 5025251161 | Rizqi Arya Kuskhilbyano    |
| 5025251009 | Athar Rozy Rasyidan                    |
| 5025251017 | Wafi Fawwaz Sutisna                    |

# Informatics ITS Graph Theory class Group <GROUP NUMBER / NAME>

## 1. Algorithm Explanation

The problem requires finding an Eulerian circuit starting and ending at crossing 1. All implementations first check feasibility: every vertex must have an even degree, and all vertices with degree > 0 must be reachable from vertex 1. If either condition fails, the graph has no Eulerian circuit and outputs `IMPOSSIBLE`.

- **Fleury's Algorithm (`fleury.c`)**: Greedily picks unused edges, avoiding bridges unless there is no other choice. Bridge detection runs via BFS/reachability test on each candidate edge, leading to $O(E^2)$ complexity.
- **Hierholzer's Algorithm (Recursive) (`hierholzer.c`)**: Uses a post-order DFS to follow unused edges until stuck, then pushes vertices onto the circuit during backtracking. Runs in $O(V + E)$ using an expanded stack via `pthread`.
- **Iterative Cycle Decomposition (`decomposition.c`)**: An iterative formulation of Hierholzer's algorithm. It keeps the main path as a linked list and splices in sub-cycles found from vertices with remaining unused edges. Runs in $O(V + E)$ with no recursion.

---

## 2. Prerequisites

- C compiler (GCC or Clang with C99 support)
- POSIX Threads library (`-lpthread`, required for `hierholzer.c`)

---

## 3. Instructions to Run

### Compilation
```bash
gcc -O2 -o fleury fleury.c
gcc -O2 -o hierholzer hierholzer.c -lpthread
gcc -O2 -o decomposition decomposition.c
```

### Execution
```bash
./fleury < sample_input.txt
./hierholzer < sample_input.txt
./decomposition < sample_input.txt
```

---

## 4. Result of Sample Run

**Input (`sample_input.txt`)**
```text
6 8
1 2
1 3
2 3
2 4
2 6
3 5
3 6
4 5
```

**Output**
[screenshot of result]

*(Note: Any valid Eulerian circuit traversing all 8 edges once and returning to 1 is correct.)*

---
