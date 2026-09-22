#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100005
#define MAXM 200005

static int n, m;
static int ea[MAXM], eb[MAXM];      
static int deg[MAXN];                
static int start_[MAXN];             
static int adjTo[2 * MAXM];          
static int adjId[2 * MAXM];          
static int used[MAXM];               

static int queueBuf[MAXN];
static int visitedMark[MAXN];
static int visitedStamp = 0;


static void bfsReachable(int src, int forbidId) {
    visitedStamp++;
    int head = 0, tail = 0;
    queueBuf[tail++] = src;
    visitedMark[src] = visitedStamp;
    while (head < tail) {
        int u = queueBuf[head++];
        for (int i = start_[u]; i < start_[u + 1]; i++) {
            int eid = adjId[i];
            if (used[eid] || eid == forbidId) continue;
            int to = adjTo[i];
            if (visitedMark[to] != visitedStamp) {
                visitedMark[to] = visitedStamp;
                queueBuf[tail++] = to;
            }
        }
    }
}

static int isReachable(int v) {
    return visitedMark[v] == visitedStamp;
}


static int isBridge(int u, int to, int eid) {
    bfsReachable(u, eid);
    return !isReachable(to);
}

static int feasible(void) {
    for (int v = 1; v <= n; v++) {
        if (deg[v] % 2 != 0) return 0;
    }
    bfsReachable(1, -1);
    for (int v = 1; v <= n; v++) {
        if (deg[v] > 0 && !isReachable(v)) return 0;
    }
    return 1;
}

int main(void) {
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 0; i < m; i++) {
        if (scanf("%d %d", &ea[i], &eb[i]) != 2) return 0;
        deg[ea[i]]++;
        deg[eb[i]]++;
    }

    /* build CSR adjacency: start_[v]..start_[v+1]-1 are v's incident edges */
    start_[1] = 0;
    for (int v = 1; v <= n; v++) start_[v + 1] = start_[v] + deg[v];
    {
        int cursor[MAXN];
        for (int v = 1; v <= n; v++) cursor[v] = start_[v];
        for (int i = 0; i < m; i++) {
            int a = ea[i], b = eb[i];
            adjTo[cursor[a]] = b; adjId[cursor[a]] = i; cursor[a]++;
            adjTo[cursor[b]] = a; adjId[cursor[b]] = i; cursor[b]++;
        }
    }

    if (!feasible()) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    int *path = (int *)malloc((size_t)(m + 1) * sizeof(int));
    int pathLen = 0;
    path[pathLen++] = 1;

    int cur = 1;
    int remaining = m;
    
    int candTo[MAXN], candId[MAXN];

    while (remaining > 0) {
        int cnt = 0;
        for (int i = start_[cur]; i < start_[cur + 1]; i++) {
            int eid = adjId[i];
            if (!used[eid]) {
                candTo[cnt] = adjTo[i];
                candId[cnt] = eid;
                cnt++;
            }
        }

        int chosen = -1;
        if (cnt == 1) {
            chosen = 0; 
        } else {
            for (int i = 0; i < cnt; i++) {
                if (!isBridge(cur, candTo[i], candId[i])) {
                    chosen = i;
                    break;
                }
            }
            if (chosen == -1) chosen = 0; 
        }

        used[candId[chosen]] = 1;
        cur = candTo[chosen];
        path[pathLen++] = cur;
        remaining--;
    }

    for (int i = 0; i < pathLen; i++) {
        printf("%d%c", path[i], (i + 1 == pathLen) ? '\n' : ' ');
    }
    free(path);
    return 0;
}

