#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void init_stack(Stack *s, int size) {
    s->items = malloc(size);
    s->top = -1;
}
void push(Stack *s, char item) {
    s->items[++s->top] = item;
}
int pop(Stack *s) {
    return s->items[s->top--];
}
int is_empty(Stack *s) {
    return s->top == -1;
}
void free_stack(Stack *s) {
    free(s->items);
}
void print_stack(Stack *s, FILE *file) {
    while (!is_empty(s)) {
        fprintf(file, "%c", pop(s));
    }
    fprintf(file, "\n");
}