#include <stdio.h>
#include <stdlib.h>

#define MAX 30

int n, m;
int adj[MAX][MAX];
int visited[MAX];
int path[MAX];
int path_count = 0;

// DFS with backtracking to find all Hamiltonian paths
void find_routes(int curr, int step) {
    // Base case: all rooms have been visited once
    if (step == n) {
        path_count++;
        printf("  Route %d: ", path_count);
        for (int i = 0; i < n; i++) {
            printf("%d%s", path[i], (i == n - 1) ? "" : " -> ");
        }
        printf("\n");
        return;
    }

    // Explore unvisited adjacent rooms
    for (int next = 0; next < n; next++) {
        if (adj[curr][next] && !visited[next]) {
            visited[next] = 1;
            path[step] = next;

            find_routes(next, step + 1);

            // Backtrack
            visited[next] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    const char *filename = (argc >= 2) ? argv[1] : "dungeon.txt";

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }

    if (fscanf(fp, "%d %d", &n, &m) != 2) {
        printf("Error: Invalid dungeon file format.\n");
        fclose(fp);
        return 1;
    }

    // Reset adjacency matrix & visited array
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }

    // Load tunnels (edges)
    for (int i = 0; i < m; i++) {
        int u, v;
        if (fscanf(fp, "%d %d", &u, &v) == 2) {
            adj[u][v] = 1;
            adj[v][u] = 1;
        }
    }
    fclose(fp);

    printf("=========================================\n");
    printf(" DUNGEON VALIDATOR\n");
    printf(" File: %s (%d rooms, %d tunnels)\n", filename, n, m);
    printf("=========================================\n\n");

    // Try starting from every possible room
    for (int start = 0; start < n; start++) {
        visited[start] = 1;
        path[0] = start;
        find_routes(start, 1);
        visited[start] = 0;
    }

    printf("\n-----------------------------------------\n");
    if (path_count > 0) {
        printf("Status: VALID\n");
        printf("Found %d possible clearing route(s).\n", path_count);
    } else {
        printf("Status: INVALID\n");
        printf("No valid path exists.\n");
    }
    printf("-----------------------------------------\n");

    return 0;
}
