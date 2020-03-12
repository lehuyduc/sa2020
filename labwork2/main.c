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
    float *turning = malloc(sizeof(float)*(n+1));
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
    float *turning; // amount of time each process had to wait
    float current, shortest;
    struct Heap *heap;

    // for each object in heap, key is remaining time, value is the process id
    heap = makeHeap(2*n);
    turning = malloc(sizeof(float) * (n+1));
    for (i=0; i<n; i++) turning[i] = 0;
    current = 0;

    // for stf(), data.x and data.y are not used
    i = 0;
    while (i < n || !empty(heap)) {
        while (i < n && a[i] <= current) {
            insert(heap, b[i], Data(i, 0));
            i = i + 1;
        }

        if (empty(heap)) {
            if (current < a[i]) current = a[i];
            current = current + b[i];
            turning[i] = b[i];
            i++;
        }
        else {
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
    float *turning; // amount of time each process had to wait
    float last, shortest;
    struct Heap *heap;

    // for each object, key is remaining time, value is the process id
    heap = makeHeap(n);
    turning = malloc(sizeof(float) * (n+1));
    for (i=0; i<n; i++) turning[i] = 0;

    // add a process at infinity time to make it easier to process
    // in srtf(), data.x is the moment the process is pushed into the heap
    // heap contains the shortest remaining time
    a[n] = inf;
    b[n] = 0;

    last = 0;
    for (i=0; i<=n; i++) {
        if (empty(heap)) {
            insert(heap, b[i], Data(i, 0));
        }
        else {
            // if (i==n) -> a[i] == inf -> all remaining processes will be processed
            while (!empty(heap) && last < a[i]) {    // while there's waiting process and time to spent before this process appear
                shortest = topData(heap, &proc);        // get process with least remaining time
                pop(heap);

                if (a[i] - last >= shortest) {       //if enough time since the last event to finish this process
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
// test queue
void testQueue() {
    struct Queue *q;

    q = makeQueue(100);
    push(q, Data(1, 1));
    push(q, Data(2, 2));
    qpop(q);
    printq(q);
    push(q, Data(3, 3));
    push(q, Data(4, 4));
    printq(q);
    qpop(q);
    qpop(q);
    printq(q);
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

    testQueue();
    return 0;
}
