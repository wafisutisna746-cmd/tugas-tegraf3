#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 30

int adj[MAX][MAX];

// Standard Fisher-Yates shuffle
void shuffle(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    int n = 7;      // default rooms
    int extra = 3;  // default extra tunnels

    // Parse command line arguments or prompt user
    if (argc >= 2) {
        n = atoi(argv[1]);
    } else {
        printf("Number of rooms (default 7): ");
        if (scanf("%d", &n) != 1 || n < 2) n = 7;
    }

    if (argc >= 3) {
        extra = atoi(argv[2]);
    } else {
        printf("Extra tunnels (default 3): ");
        if (scanf("%d", &extra) != 1 || extra < 0) extra = 3;
    }

    if (n >= MAX) n = MAX - 1;

    // Clear adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }

    // Step 1: Create a random Hamiltonian path backbone to ensure solvability
    int order[MAX];
    for (int i = 0; i < n; i++) {
        order[i] = i;
    }
    shuffle(order, n);

    int m = 0;
    for (int i = 0; i < n - 1; i++) {
        int u = order[i];
        int v = order[i + 1];
        adj[u][v] = 1;
        adj[v][u] = 1;
        m++;
    }

    // Step 2: Add random extra tunnels for branching and variety
    int attempts = 0;
    while (extra > 0 && attempts < 500) {
        int u = rand() % n;
        int v = rand() % n;
        if (u != v && !adj[u][v]) {
            adj[u][v] = 1;
            adj[v][u] = 1;
            m++;
            extra--;
        }
        attempts++;
    }

    // Write generated dungeon to file
    const char *out_name = (argc >= 4) ? argv[3] : "dungeon.txt";
    FILE *fp = fopen(out_name, "w");
    if (!fp) {
        printf("Error: Could not create output file '%s'\n", out_name);
        return 1;
    }

    fprintf(fp, "%d %d\n", n, m);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (adj[i][j]) {
                fprintf(fp, "%d %d\n", i, j);
            }
        }
    }
    fclose(fp);

    printf("=========================================\n");
    printf(" DUNGEON GENERATOR\n");
    printf(" Rooms   : %d\n", n);
    printf(" Tunnels : %d\n", m);
    printf(" Output  : %s\n", out_name);
    printf("=========================================\n");
    printf("Tunnel list:\n");
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (adj[i][j]) {
                printf("  Room %d <--> Room %d\n", i, j);
            }
        }
    }
    printf("\nDungeon generated successfully!\n");

    return 0;
}
