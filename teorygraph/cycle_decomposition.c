#include <stdio.h>
#include <stdlib.h>
 
#define MAXN 100005
#define MAXM 200005
#define MAXARC (2 * MAXM)
 

static int head[MAXN];
static int nxt[MAXARC];
static int arcTo[MAXARC];
 
static int deg[MAXN];
static int used[MAXM];
static int iter_[MAXN];     
static int visited[MAXN];
 
static int bfsQueue[MAXN];
 

static int routeVertex[MAXM + 5];  
static int routeNext[MAXM + 5];    
static int nodeCount = 0;          
 
static int n, m;
 

static inline int readInt(void) {
    int c = getchar_unlocked();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = getchar_unlocked();
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar_unlocked(); }
    int x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getchar_unlocked();
    }
    return x * sign;
}
 
static char outBuf[16];
static inline void printIntFast(int x, int isLast) {
    int len = 0;
    if (x == 0) {
        outBuf[len++] = '0';
    } else {
        while (x > 0) { outBuf[len++] = '0' + (x % 10); x /= 10; }
    }
    for (int i = len - 1; i >= 0; i--) putchar_unlocked(outBuf[i]);
    putchar_unlocked(isLast ? '\n' : ' ');
}
 

static inline int hasUnusedEdge(int v) {
    int arc = iter_[v];
    while (arc != -1 && used[arc / 2]) arc = nxt[arc];
    iter_[v] = arc;
    return arc != -1;
}
 

static void extractCycle(int start, int *headOut, int *tailOut) {
    int cur = start;
    int headIdx = -1, tailIdx = -1;
 
    do {
        int arc = iter_[cur];
        while (arc != -1 && used[arc / 2]) arc = nxt[arc];
        used[arc / 2] = 1;
        iter_[cur] = nxt[arc];
 
        int to = arcTo[arc];
        int node = nodeCount++;
        routeVertex[node] = to;
        routeNext[node] = -1;
 
        if (headIdx == -1) headIdx = node; else routeNext[tailIdx] = node;
        tailIdx = node;
 
        cur = to;
    } while (cur != start);
 
    *headOut = headIdx;
    *tailOut = tailIdx;
}
 
int main(void) {
    n = readInt();
    m = readInt();
 
    for (int v = 0; v <= n; v++) head[v] = -1;
 
    for (int i = 0; i < m; i++) {
        int a = readInt();
        int b = readInt();
 
        int arcAB = 2 * i;
        arcTo[arcAB] = b;
        nxt[arcAB] = head[a];
        head[a] = arcAB;
 
        int arcBA = 2 * i + 1;
        arcTo[arcBA] = a;
        nxt[arcBA] = head[b];
        head[b] = arcBA;
 
        deg[a]++;
        deg[b]++;
    }
 
    if (m == 0) {
        printf("1\n");
        return 0;
    }
 
    
    for (int v = 1; v <= n; v++) {
        if (deg[v] % 2 != 0) {
            printf("IMPOSSIBLE\n");
            return 0;
        }
    }
 
    
    int posDegCount = 0;
    for (int v = 1; v <= n; v++) if (deg[v] > 0) posDegCount++;
 
    int qHead = 0, qTail = 0;
    visited[1] = 1;
    bfsQueue[qTail++] = 1;
    int visitedPosDeg = (deg[1] > 0) ? 1 : 0;
 
    while (qHead < qTail) {
        int v = bfsQueue[qHead++];
        for (int arc = head[v]; arc != -1; arc = nxt[arc]) {
            int to = arcTo[arc];
            if (!visited[to]) {
                visited[to] = 1;
                if (deg[to] > 0) visitedPosDeg++;
                bfsQueue[qTail++] = to;
            }
        }
    }
 
    if (visitedPosDeg != posDegCount) {
        printf("IMPOSSIBLE\n");
        return 0;
    }
 
    
    for (int v = 1; v <= n; v++) iter_[v] = head[v];
 
   
    routeVertex[0] = 1;
    routeNext[0] = -1;
    nodeCount = 1;
 
    
    if (hasUnusedEdge(1)) {
        int h, t;
        extractCycle(1, &h, &t);
        routeNext[0] = h;
        routeNext[t] = -1;
    }
 
    
    int p = 0;
    while (p != -1) {
        int v = routeVertex[p];
        while (hasUnusedEdge(v)) {
            int h, t;
            extractCycle(v, &h, &t);
            
            routeNext[t] = routeNext[p];
            routeNext[p] = h;
        }
        p = routeNext[p];
    }
 
    if (nodeCount != m + 1) {

        printf("IMPOSSIBLE\n");
        return 0;
    }
 
    
    int cnt = 0;
    for (int cur = 0; cur != -1; cur = routeNext[cur]) cnt++;
 
    int idx = 0;
    for (int cur = 0; cur != -1; cur = routeNext[cur]) {
        idx++;
        printIntFast(routeVertex[cur], idx == cnt);
    }
 
    return 0;
}
