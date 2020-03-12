#pragma once
#include <data.h>

struct Heap {
    int capacity;
    int size;
    float *tree;     // the heap binary tree
    struct Data *datas;
};

int parent(int node) {return node / 2;}

int leftc(int node) {return node * 2;}

int rightc(int node) {return node * 2 + 1;}

// our heap tree is from 1->capacity
struct Heap* makeHeap(int capacity) {
    struct Heap* heap = malloc(sizeof(struct Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->tree = malloc(sizeof(int)*(capacity+1));
    heap->datas = malloc(sizeof(struct Data)*(capacity+1));

    for (int i=0; i<=capacity; i++) heap->tree[i] = 0;
    return heap;
}

void swapNode(struct Heap *heap, int a, int b) {
    float f;
    f = heap->tree[a];
    heap->tree[a] = heap->tree[b];
    heap->tree[b] = f;

    struct Data data;
    data = heap->datas[a];
    heap->datas[a] = heap->datas[b];
    heap->datas[b] = data;
}

void copy(struct Heap *heap, int a, int b) {
    heap->tree[a] = heap->tree[b];
    heap->datas[a] = heap->datas[b];
}

// update heap at node c
void upHeap(struct Heap *heap, int i) {
    float *tree = heap->tree;
    struct Data *datas = heap->datas;
    int c, r;

    c = i;
    float key = tree[c];
    struct Data value = datas[c];

    while (true) {
        r = parent(c);
        if (r==0 || tree[r] <= key) break; // if c is root, or priority(parent(c)) is > priority(c)
        copy(heap, c, r);
        c = r;
    }
    tree[c] = key;
    datas[c] = value;
}

// update heap after a value is decreased
void downHeap(struct Heap *heap, int i) {
    float *tree = heap->tree;
    struct Data *datas = heap->datas;
    int size = heap->size;
    int c, r;

    r = i;
    float key = tree[r];
    struct Data value = datas[r];

    while (true) {
        c = leftc(r);
        if (c < size && tree[c] > tree[c+1]) c = c + 1;
        if (c > size || tree[c] >= key) break;
        copy(heap, r, c);
        r = c;
    }
    tree[r] = key;
    datas[r] = value;
}

void insert(struct Heap *heap, float val, struct Data data) {
    float *tree = heap->tree;
    struct Data *datas = heap->datas;
    int c, r;

    int size;

    heap->size += 1;
    size = heap->size;
    tree[size] = val;
    datas[size] = data;
    upHeap(heap, size);
}

void pop(struct Heap *heap) {
    float *tree = heap->tree;
    struct Data *datas = heap->datas;
    int size = heap->size;

    swapNode(heap, 1, heap->size);
    heap->size -= 1;
    downHeap(heap, 1);
}

float top(struct Heap *heap) {
    return heap->tree[1];
}

float topData(struct Heap* heap, struct Data *data) {
    *data = heap->datas[1];
    return heap->tree[1];
}

bool empty(struct Heap *heap) {
    return heap->size == 0;
}

void clear(struct Heap *heap) {
    while (!empty(heap)) pop(heap);
}
