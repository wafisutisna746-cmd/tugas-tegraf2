# CSES 1691 – Mail Delivery (Eulerian Circuit)

Informatics ITS Graph Theory class Group <GROUP NUMBER / NAME>

**Members**
| Name | NRP |
|---|---|
| <Name 1> | <NRP 1> |
| <Name 2> | <NRP 2> |
| <Name 3> | <NRP 3> |
| <Name 4> | <NRP 4> |

---

## 1. Problem

[CSES 1691 – Mail Delivery](https://cses.fi/problemset/task/1691): find a route from crossing 1 back to crossing 1 that uses every street exactly once, or print `IMPOSSIBLE`.

`2 ≤ n ≤ 10^5`, `1 ≤ m ≤ 2·10^5` — needs close to `O(n + m)`.

## 2. Algorithms implemented

| # | Algorithm | File |
|---|---|---|
| 1 | Fleury's Algorithm | `fleury.c` |
| 2 | Hierholzer's Algorithm (recursive) | `hierholzer.c` |
| 3 | Iterative cycle decomposition (linked-list splicing) | `decomposition.c` |

All three check feasibility first: every vertex has even degree, and every vertex with degree > 0 is reachable from vertex 1 (BFS). Fails either → `IMPOSSIBLE`.

### 2.1 How each works

**Fleury (`fleury.c`)** — at each step, take an unused edge that isn't a bridge; take a bridge only when forced. Bridge test = re-BFS from the current vertex with that edge removed. `O(E²)`, since the test reruns per edge per step.

**Hierholzer (`hierholzer.c`)** — recursive post-order DFS: follow unused edges from `u` until stuck, append `u`, backtrack. Reversed post-order = Eulerian circuit. Runs on a dedicated pthread with a 64MB stack since recursion depth can hit ~2×10^5. `O(V + E)`.

**Decomposition (`decomposition.c`)** — non-recursive version of the same idea. Route kept as a linked list; repeatedly extracts a cycle from any vertex on the route with unused edges and splices it in. Each edge iterator only moves forward, so no rescans. `O(V + E)`, no recursion.

### 2.2 Bug fixed

`fleury.c` had an unterminated `/* comment` breaking the build — closed it, no logic changed.

## 3. Prerequisites

- C99 compiler (GCC ≥ 7 / Clang ≥ 6)
- `pthread` (only for `hierholzer.c`)
- Python 3.8+ (optional, for `validate.py`)

## 4. How to run

```bash
gcc -O2 -o fleury fleury.c
gcc -O2 -o hierholzer hierholzer.c -lpthread
gcc -O2 -o decomposition decomposition.c

./fleury         < sample_input.txt
./hierholzer     < sample_input.txt
./decomposition  < sample_input.txt
```

Input: CSES format (`n m` then `m` edge lines). Output: `m+1` vertices starting/ending at 1, or `IMPOSSIBLE`.

## 5. Result of sample run

**Input** (`sample_input.txt`):
```
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
```
fleury:        1 2 3 5 4 2 6 3 1
hierholzer:    1 2 3 5 4 2 6 3 1
decomposition: 1 3 6 2 4 5 3 2 1
```
All differ from the CSES sample (`1 2 6 3 2 4 5 3 1`) and from each other — expected, since any valid circuit is accepted. Verified with `validate.py`, which checks every edge is used exactly once.

## 6. Testing

Ran all 16 cases in `tests/` through each binary and checked with `validate.py` (can't diff `.out` directly, since circuits aren't unique).

| Algorithm | Result | Worst time |
|---|---|---|
| Fleury | Correct on all, **times out** on the 6 large cases (n≈10^5) | > 5s (limit 1s) |
| Hierholzer | 16/16 | ≈ 0.08s |
| Decomposition | 16/16 | ≈ 0.04s |

Fleury is correct everywhere — it just can't finish the large graphs in time, matching its `O(E²)` bound.

```bash
python3 validate.py ./fleury tests
python3 validate.py ./hierholzer tests
python3 validate.py ./decomposition tests
```

## 7. Complexity comparison

| Algorithm | Complexity | n≈10^5, m≈2×10^5 |
|---|---|---|
| Fleury | `O(E²)` | Times out |
| Hierholzer (recursive) | `O(V+E)` | ≈ 0.08s |
| Decomposition (iterative) | `O(V+E)` | ≈ 0.04s |

Fleury doesn't scale past this problem's constraints. Both Hierholzer variants do; the iterative one is a bit faster and skips the recursion-depth/stack-size issue entirely. Full trace in `report.pdf`.

## 8. AI tools usage disclosure

<Fill in honestly, e.g.:>
- AI (Claude) helped fix a compile error in `fleury.c` and wrote `validate.py`.
- <Adjust to your group's actual process; attach prompt history as required.>

## 9. Repository structure

```
.
├── README.md
├── report.pdf
├── fleury.c
├── hierholzer.c
├── decomposition.c
├── sample_input.txt
├── validate.py
└── tests/
```
