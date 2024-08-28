#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *items;
    int top;
} Stack;

void init_stack(Stack *s, int size);
void push(Stack *s, char item);
int pop(Stack *s);
int is_empty(Stack *s);
void free_stack(Stack *s);
void print_stack(Stack *s, FILE *file);