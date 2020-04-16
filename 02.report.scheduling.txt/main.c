#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <heap.h>
#include <queue.h>

#define maxn 1000000
#define inf 1000000007


int n;
float a[maxn], b[maxn]; // arrival, waiting

void fcfs(int n, float a[], float b[]) {
    int i,j;
    float *turning = malloc(sizeof(float)*(n+1)); // turning time of each process
    float current = 0;

    for (i=0; i<n; i++) {
        if (current < a[i]) current = a[i];
        current = current + b[i];
        turning[i] = current - a[i];
    }

    float waitTime = 0, turnTime = 0;
    for (int i=0; i<n; i++) {
        turnTime += turning[i];
        waitTime += turning[i] - b[i];
    }
    printf("FCFS turning: "); for (i=0;i<n;i++) printf("%f ",turning[i]);
    printf("\n");
    printf("FCFS: waiting = %f, turning = %f\n\n", waitTime/n, turnTime/n);
}

//---------------------         SHORTEST TIME FIRST
void stf(int n, float a[], float b[]) {
    int i, j;
    struct Data pid;
    float *turning;     // turning time of each process
    float current, shortest;
    struct Heap *heap;  // min heap

    // for each object in heap, key is remaining time, value is data.id = the process id
    heap = makeHeap(n);
    turning = malloc(sizeof(float) * (n+1));
    for (i=0; i<n; i++) turning[i] = 0;
    current = 0;

    // for stf(), data.x and data.y are not used
    i = 0;
    while (i < n || !empty(heap)) {
        while (i < n && a[i] <= current) {    // put the processes that appear during the previous run to heap
            insert(heap, b[i], Data(i, 0));
            i = i + 1;
        }

        if (empty(heap)) {       // for example, a process the starts very late, and all previous processes have finish
            if (current < a[i]) current = a[i];
            current = current + b[i];
            turning[i] = b[i];
            i++;
        }
        else {                  // keep choosing shortest job first
            shortest = topData(heap, &pid);
            pop(heap);
            current += shortest;
            turning[pid.id] = current - a[pid.id];
        }
    }

    float waitTime = 0, turnTime = 0;
    for (int i=0; i<n; i++) {
        turnTime += turning[i];
        waitTime += turning[i] - b[i];
    }
    printf("SJF turning: "); for (i=0;i<n;i++) printf("%f ",turning[i]);
    printf("\n");
    printf("SJF: waiting = %f, turning = %f\n\n", waitTime/n, turnTime/n);
}

//---------------------         SHORTEST REMAINING TIME FIRST
void srtf(int n, float a[], float b[]) {
    int i, j;
    struct Data data, proc;
    float *turning;             // turning time of each process
    float last, shortest;
    struct Heap *heap;

    // for each process in heap, key is remaining time, value (data.id) is the process id
    heap = makeHeap(n);
    turning = malloc(sizeof(float) * (n+1));
    for (i=0; i<n; i++) turning[i] = 0;

    // add a process at infinity time to make it easier to program
    a[n] = inf;
    b[n] = 0;

    last = 0;
    for (i=0; i<=n; i++) {
        if (empty(heap)) {
            insert(heap, b[i], Data(i, 0));
        }
        else {
            // if (i==n) -> a[i] == inf -> all remaining processes will be processed
            while (!empty(heap) && last < a[i]) {       // while there's waiting process and time to spent before this process appear
                shortest = topData(heap, &proc);        // get process with least remaining time
                pop(heap);

                if (a[i] - last >= shortest) {          // if enough time since the last event to finish this process
                    turning[proc.id] = last + shortest - a[proc.id];
                    last = last + shortest;
                }
                else {
                    insert(heap, shortest - (a[i] - last), Data(proc.id, 0)); // spend the available time, and push that process back into heap
                    last = a[i];
                }
            }

            insert(heap, b[i], Data(i, 0));
        }
        last = a[i];
    }

    float waitTime = 0, turnTime = 0;
    for (int i=0; i<n; i++) {
        turnTime += turning[i];
        waitTime += turning[i] - b[i];
    }
    printf("SRTF turning: "); for (i=0;i<n;i++) printf("%f ",turning[i]);
    printf("\n");
    printf("SRTF: waiting = %f, turning = %f\n\n", waitTime/n, turnTime/n);
}

//---------------------         ROUND ROBIN - need queue
void rr(int n, float a[], float b[], float s) {
    int i,j;
    float *turning;
    float current;
    struct Queue *q;
    struct Data data, prevData;

    turning = malloc(sizeof(float) * (n+1));
    for (i=0; i<n; i++) turning[i] = 0;

    // for RR(), data.x is the remaining time of the process
    q = makeQueue(n);
    current = 0;
    i = 0;
    while (i<n || !qempty(q)) {
        if (qempty(q) && current < a[i]) current = a[i];
        while (i<n && a[i] <= current) {        // push processes that have appeared during the previous slice into queue
            push_front(q, Data(i, b[i]));
            i++;
        }

        data = front(q);
        pop_front(q);

        if (data.x <= s) { // can finish this process immediately
            turning[data.id] = current + data.x - a[data.id];
            current += data.x;
        }
        else {
            data.x -= s;    // use the available time
            current += s;
            while (i<n && a[i] <= current) { // push all the processes that appeared during this slice into queue
                push_front(q, Data(i, b[i]));
                i++;
            }
            push_back(q, data); // push this process back into queue
        }
    }

    float waitTime = 0, turnTime = 0;
    for (int i=0; i<n; i++) {
        turnTime += turning[i];
        waitTime += turning[i] - b[i];
    }
    printf("RR turning: "); for (i=0;i<n;i++) printf("%f ",turning[i]);
    printf("\n");
    printf("RR: waiting = %f, turning = %f\n\n", waitTime/n, turnTime/n);
}

int main()
{
    setbuf(stdout, NULL);
    freopen("scheduling.inp","r",stdin);
 //   freopen("scheduling.out","w",stdout);
    int i,j,k;

    // number of processes
    // each line are 2 number: arrival time + waiting time
    scanf("%d",&n);
    for (i=0;i<n;i++) scanf("%f%f",&a[i],&b[i]);

    fcfs(n,a,b);
    stf(n,a,b);
    srtf(n,a,b);
    rr(n,a,b,1);

    return 0;
}
