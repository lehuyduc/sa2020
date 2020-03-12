#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <data.h>

// circular Q
struct Queue {
    int capacity;
    int size, first, last;
    struct Data *datas;
};

struct Queue *makeQueue(int capacity) {
    struct Queue *q = malloc(sizeof(struct Queue));
    q->capacity = capacity;
    q->first = 0;
    q->last = -1;
    q->size = 0;
    q->datas = malloc(sizeof(struct Data)*(capacity+1));
    return q;
};

void push(struct Queue *q, struct Data data) {
    q->last += 1;
    if (q->last==q->capacity) q->last = 0;
    q->datas[q->last] = data;
}

void qpop(struct Queue *q) {
    q->first += 1;
    if (q->first==q->capacity) q->first = 0;
}

void pop_front(struct Queue *q) {
    qpop(q);
}

struct Data front(struct Queue *q) {
    return q->datas[q->first];
};

struct Data back(struct Queue *q) {
    return q->datas[q->last];
};


void printq(struct Queue *q) {
    int n = q->size;
    for (int i=q->first; i<= q->last; i++) {
        if (i==n) i = 0;
        printf("%f ", q->datas[i].x);
        if (i==q->last) break;
    }
    printf("\n");
}
