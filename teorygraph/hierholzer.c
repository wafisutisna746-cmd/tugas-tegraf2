
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXN 100005
#define MAXM 200005

static int n, m;
static int ea[MAXM], eb[MAXM];
static int deg[MAXN];
static int start_[MAXN];
static int adjTo[2 * MAXM];
static int adjId[2 * MAXM];
static int used[MAXM];
static int ptr_[MAXN];               

static int *circuitRev;              
static int circuitLen = 0;

static int queueBuf[MAXN];
static int visitedMark[MAXN];
static int visitedStamp = 0;

static void bfsFrom(int src) {
    visitedStamp++;
    int head = 0, tail = 0;
    queueBuf[tail++] = src;
    visitedMark[src] = visitedStamp;
    while (head < tail) {
        int u = queueBuf[head++];
        for (int i = start_[u]; i < start_[u + 1]; i++) {
            int to = adjTo[i];
            if (visitedMark[to] != visitedStamp) {
                visitedMark[to] = visitedStamp;
                queueBuf[tail++] = to;
            }
        }
    }
}

static int feasibleAndConnected(void) {
    for (int v = 1; v <= n; v++) {
        if (deg[v] % 2 != 0) return 0;
    }
    bfsFrom(1);
    for (int v = 1; v <= n; v++) {
        if (deg[v] > 0 && visitedMark[v] != visitedStamp) return 0;
    }
    return 1;
}


static void dfs(int u) {
    while (ptr_[u] < start_[u + 1]) {
        int i = ptr_[u]; 
        int eid = adjId[i];
        int to = adjTo[i];
        ptr_[u]++; 
        if (used[eid]) continue;
        used[eid] = 1;
        dfs(to);
    }
    circuitRev[circuitLen++] = u; 
}

typedef struct {
    int ok;
} ThreadArgs;

static void *solve(void *argPtr) {
    ThreadArgs *args = (ThreadArgs *)argPtr;

    if (m == 0) {
        args->ok = 1;
        circuitRev[circuitLen++] = 1;
        return NULL;
    }

    if (!feasibleAndConnected()) {
        args->ok = 0;
        return NULL;
    }

    dfs(1);

    if (circuitLen != m + 1) {
        args->ok = 0;
        return NULL;
    }

    args->ok = 1;
    return NULL;
}

int main(void) {
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 0; i < m; i++) {
        if (scanf("%d %d", &ea[i], &eb[i]) != 2) return 0;
        deg[ea[i]]++;
        deg[eb[i]]++;
    }

    start_[1] = 0;
    for (int v = 1; v <= n; v++) start_[v + 1] = start_[v] + deg[v];
    {
        int *cursor = (int *)malloc((size_t)(n + 2) * sizeof(int));
        for (int v = 1; v <= n; v++) cursor[v] = start_[v];
        for (int i = 0; i < m; i++) {
            int a = ea[i], b = eb[i];
            adjTo[cursor[a]] = b; adjId[cursor[a]] = i; cursor[a]++;
            adjTo[cursor[b]] = a; adjId[cursor[b]] = i; cursor[b]++;
        }
        for (int v = 1; v <= n; v++) ptr_[v] = start_[v];
        free(cursor);
    }

    circuitRev = (int *)malloc((size_t)(m + 2) * sizeof(int));

    ThreadArgs args;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 64 * 1024 * 1024); 
    pthread_t thread;
    pthread_create(&thread, &attr, solve, &args);
    pthread_join(thread, NULL);

    if (!args.ok) {
        printf("IMPOSSIBLE\n");
        free(circuitRev);
        return 0;
    }

    
    for (int i = 0; i < circuitLen; i++) {
        int v = circuitRev[circuitLen - 1 - i];
        printf("%d%c", v, (i + 1 == circuitLen) ? '\n' : ' ');
    }
    free(circuitRev);
    return 0;
}

