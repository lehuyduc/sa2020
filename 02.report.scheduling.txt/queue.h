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

bool qempty(struct Queue *q) {
    return q->size==0;
}

void push_back(struct Queue *q, struct Data data) {
    q->last += 1;
    if (q->last==q->capacity) q->last = 0;
    q->datas[q->last] = data;
    q->size += 1;
}

void push_front(struct Queue *q, struct Data data) {
    q->first -= 1;
    if (q->first==-1) q->first = q->capacity-1;
    q->datas[q->first] = data;
    q->size += 1;
}

void pop_front(struct Queue *q) {
    q->first += 1;
    if (q->first==q->capacity) q->first = 0;
    q->size -= 1;
}

void pop_back(struct Queue *q) {
    q->last -= 1;
    if (q->last==-1) q->last = q->capacity-1;
    q->size -= 1;
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
