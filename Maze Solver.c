#include <stdio.h>
#include <stdlib.h>
#define MAX 50  // maximum maze size
// Directions (Down, Up, Right, Left)
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
// Structure to store coordinates
typedef struct {
    int x, y;
} Point;
// ---------- STACK (for DFS) ----------
typedef struct {
    Point data[MAX];
    int top;
} Stack;
void initStack(Stack *s) {
    s->top = -1;
}
int isEmptyStack(Stack *s) {
    return s->top == -1;
}
void push(Stack *s, Point p) {
    if (s->top < MAX - 1) s->data[++s->top] = p;
}
Point pop(Stack *s) {
    return s->data[s->top--];
}
// ---------- QUEUE (for BFS) ----------
typedef struct {
    Point data[MAX * MAX];
    int front, rear;
} Queue;
void initQueue(Queue *q) {
    q->front = q->rear = 0;
}
int isEmptyQueue(Queue *q) {
    return q->front == q->rear;
}
void enqueue(Queue *q, Point p) {
    q->data[q->rear++] = p;
}
Point dequeue(Queue *q) {
    return q->data[q->front++];
}
// ---------- Helper Functions ----------
int isValid(int maze[MAX][MAX], int visited[MAX][MAX], int n, int m, int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 0 && !visited[x][y]);
}
// ---------- DFS using Stack ----------
int dfs(int maze[MAX][MAX], int n, int m, Point start, Point end) {
    Stack s;
    initStack(&s);
    int visited[MAX][MAX] = {0};
    push(&s, start);
    visited[start.x][start.y] = 1;
    printf("\nDFS Path: ");
    while (!isEmptyStack(&s)) {
        Point curr = pop(&s);
        printf("(%d,%d) ", curr.x, curr.y);
        if (curr.x == end.x && curr.y == end.y) {
            printf("\nReached the end using DFS!\n");
            return 1;
        }
        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];
            if (isValid(maze, visited, n, m, nx, ny)) {
                visited[nx][ny] = 1;
                Point next = {nx, ny};
                push(&s, next);
            }
        }
    }
    printf("\nNo Path Found using DFS.\n");
    return 0;
}
// ---------- BFS using Queue ----------
int bfs(int maze[MAX][MAX], int n, int m, Point start, Point end) {
    Queue q;
    initQueue(&q);
    int visited[MAX][MAX] = {0};
    Point parent[MAX][MAX];
    int distance[MAX][MAX] = {0};

    // Initialize parent array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            parent[i][j].x = -1;
            parent[i][j].y = -1;
        }
    }

    enqueue(&q, start);
    visited[start.x][start.y] = 1;
    printf("\nBFS Path: ");
    while (!isEmptyQueue(&q)) {
        Point curr = dequeue(&q);
        printf("(%d,%d) ", curr.x, curr.y);
        if (curr.x == end.x && curr.y == end.y) {
            printf("\nReached the end using BFS!\n");

            // Reconstruct and print shortest path
            Point path[MAX * MAX];
            int pathLen = 0;
            Point temp = end;

            while (temp.x != start.x || temp.y != start.y) {
                path[pathLen++] = temp;
                temp = parent[temp.x][temp.y];
            }
            path[pathLen++] = start;

            printf("Shortest Path: ");
            for (int i = pathLen - 1; i >= 0; i--) {
                printf("(%d,%d)", path[i].x, path[i].y);
                if (i > 0) printf(" -> ");
            }
            printf("\nNumber of steps: %d\n", distance[end.x][end.y]);

            return 1;
        }
        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];
            if (isValid(maze, visited, n, m, nx, ny)) {
                visited[nx][ny] = 1;
                parent[nx][ny] = curr;
                distance[nx][ny] = distance[curr.x][curr.y] + 1;
                Point next = {nx, ny};
                enqueue(&q, next);
            }
        }
    }
    printf("\nNo Path Found using BFS.\n");
    return 0;
}
// ---------- MAIN ----------
int main() {
    int n, m;
    int maze[MAX][MAX];
    Point start, end;
    printf("Enter maze size (rows cols): ");
    scanf("%d %d", &n, &m);
    printf("Enter maze matrix (0 = path, 1 = wall):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &maze[i][j]);
    printf("Enter start point (row col): ");
    scanf("%d %d", &start.x, &start.y);
    printf("Enter end point (row col): ");
    scanf("%d %d", &end.x, &end.y);
    printf("\n---------- Solving Maze ----------\n");
    // Run DFS
    dfs(maze, n, m, start, end);
    // Run BFS
    bfs(maze, n, m, start, end);
    printf("\n----------------------------------\n");
    return 0;
}
