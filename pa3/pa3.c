#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 127
#define IN_FILE_NAME "hw3_input.txt"
#define OUT_FILE_NAME_1 "hw3_output1.txt"
#define OUT_FILE_NAME_2 "hw3_output2.txt"

#define PARENT(i) ((i) / 2)
#define LEFT_CHILD(i) ((i) * 2)
#define RIGHT_CHILD(i) ((i) * 2 + 1)
#define HEAP_SIZE heap[0]->freq

typedef struct Node {
    char symbol;
    int freq;
    struct Node *left;
    struct Node *right;
} Node;

int frequencies[ASCII_SIZE] = { 0 };
Node *heap[ASCII_SIZE + 1]; // h[0]->freq에는 전체 힙의 개수 저장, min-heap

void calculate_frequencies(FILE *input_file);
Node *create_node(char symbol, int freq, Node *left, Node *right);
void heap_insert(Node *new_node);
Node *heap_delete();
void heap_swap(int i, int j);
Node *build_huffman_tree();
void free_huffman_tree(Node *node);

void write_huffman_tree(Node *node, FILE *output_file);
void write_encoded_output(Node *root, FILE *input_file, FILE *output_file);
void generate_codes(Node *node, char *buffer, int depth, char **codes);

Node *read_huffman_tree(FILE *input_file);
void write_huffman_codes(FILE *output_file, char **codes);
void write_decoded_output(FILE *input_file, FILE *output_file, Node *root);

int main() {
    // Construct a Huffman tree
    FILE *input_file;
    if ((input_file = fopen(IN_FILE_NAME, "r")) == NULL) {
        fprintf(stderr, "file open error 1\n");
        exit(1);
    }
    calculate_frequencies(input_file);
    heap[0] = create_node(0, 0, NULL, NULL);
    Node *root = build_huffman_tree();
    fclose(input_file);

    // hw3_output1.txt
    FILE *output_file;
    if ((output_file = fopen(OUT_FILE_NAME_1, "w")) == NULL) {
        fprintf(stderr, "file open error 2\n");
        exit(1);
    }
    //  - huffman tree
    write_huffman_tree(root, output_file);
    fprintf(output_file, "\n");
    // - encoded output
    if ((input_file = fopen(IN_FILE_NAME, "r")) == NULL) {
        fprintf(stderr, "file open error 3\n");
        exit(1);
    }
    write_encoded_output(root, input_file, output_file);
    fclose(input_file);
    fclose(output_file);
    free_huffman_tree(root);

    // hw3_output2.txt
    if ((input_file = fopen(OUT_FILE_NAME_1, "r")) == NULL) {
        fprintf(stderr, "file open error 4\n");
        exit(1);
    }
    if ((output_file = fopen(OUT_FILE_NAME_2, "w")) == NULL) {
        fprintf(stderr, "file open error 5\n");
        exit(1);
    }
    // - reconstruct huffman tree
    Node *new_root = read_huffman_tree(input_file);
    // - write huffman codes
    char *new_codes[ASCII_SIZE] = { 0 };
    char new_buffer[ASCII_SIZE];
    generate_codes(new_root, new_buffer, 0, new_codes);
    write_huffman_codes(output_file, new_codes);
    // - write decoded output
    write_decoded_output(input_file, output_file, new_root);

    fclose(input_file);
    fclose(output_file);
    free_huffman_tree(new_root);

    return 0;
}

void write_decoded_output(FILE *input_file, FILE *output_file, Node *root) {
    Node *cur = root;
    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == '0') {
            cur = cur->right;
        } else if (ch == '1') {
            cur = cur->left;
        }

        if (cur->left == NULL && cur->right == NULL) { // leaf node
            fputc(cur->symbol, output_file);
            cur = root;
        }
    }
}

void write_huffman_codes(FILE *output_file, char **codes) {
    for (int i = 32; i < ASCII_SIZE; i++) {
        if (codes[i]) {
            fprintf(output_file, "%c : %s\n", i, codes[i]);
        }
    }
}

Node *read_huffman_tree(FILE *input_file) {
    int symbol;
    if (fscanf(input_file, "%d ", &symbol) != 1) {
        return NULL;
    }

    if (symbol != 0) { // leaf node
        return create_node((char)symbol, 0, NULL, NULL);
    } else {
        Node *left = read_huffman_tree(input_file);
        Node *right = read_huffman_tree(input_file);
        return create_node('\0', 0, left, right);
    }
}

void write_encoded_output(Node *root, FILE *input_file, FILE *output_file) {
    char *codes[ASCII_SIZE] = { 0 };
    char buffer[ASCII_SIZE];
    generate_codes(root, buffer, 0, codes);

    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        fprintf(output_file, "%s", codes[ch]);
    }
}

void generate_codes(Node *node, char *buffer, int depth, char **codes) {
    if (node->left == NULL && node->right == NULL) { // leaf node
        buffer[depth] = '\0';
        codes[(int)node->symbol] = strdup(buffer);
        return;
    }

    if (node->left != NULL) {
        buffer[depth] = '1';
        generate_codes(node->left, buffer, depth + 1, codes);
    }
    if (node->right != NULL) {
        buffer[depth] = '0';
        generate_codes(node->right, buffer, depth + 1, codes);
    }
}

void write_huffman_tree(Node *node, FILE *output_file) {
    if (node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) { // leaf node
        fprintf(output_file, "%d ", node->symbol);
    } else {
        fprintf(output_file, "0 ");
    }

    write_huffman_tree(node->left, output_file);
    write_huffman_tree(node->right, output_file);
}

void calculate_frequencies(FILE *input_file) {
    // Read characters from the file
    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        frequencies[ch]++;
    }
}

Node *create_node(char symbol, int freq, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Memory allocation error in create_node\n");
        exit(1);
    }
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

void heap_swap(int i, int j) {
    Node *temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void heap_insert(Node *new_node) {
    heap[++HEAP_SIZE] = new_node;
    int target_idx = HEAP_SIZE;

    while (heap[target_idx]->freq < heap[PARENT(target_idx)]->freq && target_idx > 1) {
        heap_swap(target_idx, PARENT(target_idx));
        target_idx = PARENT(target_idx);
    }
}

Node *heap_delete() {
    if (HEAP_SIZE == 0) {
        return NULL;
    }

    Node *min_node = heap[1];
    heap_swap(1, HEAP_SIZE);
    HEAP_SIZE--;
    int target_idx = 1;

    while (LEFT_CHILD(target_idx) <= HEAP_SIZE) {
        int min_child_idx = LEFT_CHILD(target_idx);

        if (RIGHT_CHILD(target_idx) <= HEAP_SIZE && heap[RIGHT_CHILD(target_idx)]->freq < heap[LEFT_CHILD(target_idx)]->freq) {
            min_child_idx = RIGHT_CHILD(target_idx);
        }

        if (heap[target_idx]->freq > heap[min_child_idx]->freq) {
            heap_swap(target_idx, min_child_idx);
            target_idx = min_child_idx;
        } else {
            break;
        }
    }

    return min_node;
}

Node *build_huffman_tree() {
    for (int i = 32; i < ASCII_SIZE; i++) {
        if (frequencies[i] > 0) {
            Node *new_node = create_node(i, frequencies[i], NULL, NULL);
            heap_insert(new_node);
        }
    }

    while (HEAP_SIZE > 1) {
        Node *left = heap_delete();
        Node *right = heap_delete();
        Node *merged = create_node('\0', left->freq + right->freq, left, right);
        heap_insert(merged);
    }

    return heap[1];
}

void free_huffman_tree(Node *node) {
    if (!node)
        return;
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    free(node);
}