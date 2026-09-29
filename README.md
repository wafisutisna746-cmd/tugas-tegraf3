# Graph Theory Assignment – Week 4: Hamiltonian Tour
## Dungeon Generator & Validator

---

### Group Members
|    NRP     |           Nama             |
| :--------: |       :------------:       |
| 5025251260 | Aqilah Ibrahim             |
| 5025251161 | Rizqi Arya Kuskhilbyano    |
| 5025251009 | Athar Rozy Rasyidan                    |
| 5025251017 | Wafi Fawwaz Sutisna                    |

---

## 1. Project Overview

In this project, we model a procedural dungeon as a graph:
* **Rooms** = **Vertices ($V$)**
* **Tunnels** = **Edges ($E$)**

The game rule states:
> *"The player must clear the entire dungeon by visiting every room exactly once without reusing any tunnel."*

In Graph Theory, this is the **Hamiltonian Path Problem** (visiting every vertex exactly once). Because the Hamiltonian Path problem is **NP-complete**, finding solutions cannot be done with simple linear algorithms.

---

## 2. Algorithm 1: Procedural Dungeon Generator (`generator.c`)

### Why this algorithm?
If we connect rooms completely at random, there is a very high chance the dungeon will be unsolvable (no Hamiltonian path). At the same time, if we only make a single straight line of rooms, the game will be boring and repetitive.

To solve this, we use the **Backbone-First** approach:
1. First, we create one guaranteed route that visits all rooms (the backbone). This guarantees the dungeon is **always solvable**.
2. Then, we add random extra tunnels. This adds forks, loops, and alternative paths so each generated dungeon feels fresh and different.

### How it works:
1. Create a list of rooms: `[0, 1, 2, ..., n-1]`.
2. Shuffle this list randomly using the **Fisher-Yates shuffle**.
3. Connect adjacent rooms in the shuffled list (`order[i]` to `order[i+1]`). This creates the guaranteed clearing route.
4. Add a few extra random tunnels between rooms that are not connected yet.
5. Save the resulting list of rooms and tunnels to `dungeon.txt`.

---

## 3. Algorithm 2: Dungeon Validator (`validator.c`)

### Why DFS with Backtracking?
* **Why not regular DFS?**  
  In a regular DFS, a visited room is marked `visited = 1` forever. If DFS takes a wrong turn into a dead end, that room stays blocked, and the program will mistakenly say "no path exists" even if another route was possible.
* **Why DFS with Backtracking?**  
  Backtracking marks a room as visited when stepping forward, but **unmarks it (`visited = 0`) when retreating**. This allows the algorithm to test all possible combinations and discover **all** valid Hamiltonian paths.

### How it works:
1. Read the number of rooms, tunnels, and edges from the file into an adjacency matrix `adj[MAX][MAX]`.
2. Try starting from every possible room ($0$ to $n-1$).
3. In the recursive function `find_routes(current_room, step)`:
   - If `step == n`: All rooms have been visited! Print this valid route.
   - For every neighbor room:
     - If it is connected and not yet visited, mark `visited[next] = 1`.
     - Recursively call `find_routes(next, step + 1)`.
     - **Backtrack:** Reset `visited[next] = 0` so other paths can use this room.
4. If one or more routes are found, print them and conclude **VALID**.
5. If no route is found, print:  
   `Statement: No valid path exists.`

---

## 4. Algorithm Comparison

### Generator Algorithms
| Method | Guarantee Solvable? | Route Variety | Speed |
| :--- | :--- | :--- | :--- |
| **Backbone + Random Edges (Our Choice)** | **100% Guaranteed** | **High** (many alternate routes) | Very Fast ($O(N + E)$) |
| **Pure Random Edges** | Low (mostly broken/unsolvable) | High | Slow (requires retrying many times) |
| **Simple Line/Cycle** | Guaranteed | **None** (always the same boring line) | Fast |

### Validator Algorithms
| Method | Pros | Cons |
| :--- | :--- | :--- |
| **DFS with Backtracking (Our Choice)** | Simple, low memory ($O(N)$), finds and prints **all** routes | Factorial time $O(N!)$, best for $N \le 12$ rooms |
| **Standard DFS** | Fast ($O(V + E)$) | **Gives wrong answers** (cannot solve Hamiltonian Path) |
| **Bitmask Dynamic Programming** | Faster for slightly larger graphs ($N \approx 20$) | More complex code, takes more memory ($O(N \cdot 2^N)$) |

---

## 5. How to Compile and Run

### 1. Compile with GCC:
```bash
gcc -O2 generator.c -o generator.exe
gcc -O2 validator.c -o validator.exe
```

### 2. Run the Generator:
Generate a dungeon with 6 rooms and 2 extra tunnels:
```bash
./generator.exe 6 2 dungeon.txt
```

### 3. Run the Validator:
Check the generated dungeon:
```bash
./validator.exe dungeon.txt
```

---

## 6. Sample Test Cases (Valid & Invalid)

All test files are stored in the `samples/` folder.

### Sample 1: Valid Dungeon (`samples/sample_valid_1.txt`)
* **Input:** 5 rooms, 6 tunnels.
* **Output:**
```text
=== VALIDATING DUNGEON (samples/sample_valid_1.txt) ===
Rooms count: 5, Tunnels count: 6

List of possible clearing routes:
  Route 1: 0 -> 1 -> 2 -> 3 -> 4
  Route 2: 0 -> 4 -> 3 -> 2 -> 1
  ...
=== RESULT ===
Status: VALID
Found 14 possible route(s).
```

### Sample 2: Invalid Dungeon – Disconnected (`samples/sample_invalid_disconnected.txt`)
* **Input:** 5 rooms, 4 tunnels connecting rooms 0, 1, 2, 3; Room 4 is completely isolated with 0 tunnels.
* **Output:**
```text
=== VALIDATING DUNGEON (samples/sample_invalid_disconnected.txt) ===
Rooms count: 5, Tunnels count: 4

List of possible clearing routes:

=== RESULT ===
Status: INVALID
No valid path exists.
```

### Sample 3: Invalid Dungeon – Bottleneck / Hub (`samples/sample_invalid_triangles.txt`)
* **Input:** 7 rooms, 9 tunnels forming 3 separate triangles that meet at only one center room (Room 0). Every room has at least 2 tunnels, but you cannot visit all rooms without revisiting Room 0 multiple times.
* **Output:**
```text
=== VALIDATING DUNGEON (samples/sample_invalid_triangles.txt) ===
Rooms count: 7, Tunnels count: 9

List of possible clearing routes:

=== RESULT ===
Status: INVALID
No valid path exists.
```
