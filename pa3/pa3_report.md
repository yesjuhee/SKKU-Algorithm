# PA3 Report

## 1. 서론

해당 과제를 통해 Huffman Code 알고리즘을 이용해 텍스트 파일을 압축하고, 압축된 파일을 다시 복원하는 프로그램을 구현했다.

## 2. main 함수 설명

```c
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
```

해당 과제의 메인 함수는 크게 3 파트로 나눌 수 있다.

1. `hw3_input.txt` 를 읽고 해당 텍스트 파일을 바탕으로 허프만 트리를 만든다.
2. `hw3_output1.txt` 파일에 허프만 트리와 인코딩 된 텍스트를 작성한다.
3. 2에서 작성된 내용을 읽어서 허프만 트리를 재구성하고, 트리를 이용해 인코딩한 코드를 디코딩하여 `hw3_output2.txt` 파일에 작성한다.

메인 함수에서는 각 단계에 맞춰 파일을 열고 단계에 해당하는 사용자 정의 함수들을 호출하여 파일에 쓰는 로직을 위주로 작성하였다.

## 3. 주요 알고리즘 설명

### 3.1 Frequencies 계산

이력 파일의 각 문자에 대한 빈도를 계산한다. 이를 위해 ASCII 코드 범위에 해당하는 크기의 정수 배열을 초기화하고 파일을 읽으면서 해당하는 인덱스의 정수 값을 증가시킨다.

```c
#define ASCII_SIZE 127
int frequencies[ASCII_SIZE] = { 0 };

void calculate_frequencies(const char *file_name) {
  FILE *input_file = fopen(file_name, "r");
  if (!input_file) {
      fprintf(stderr, "File open error\n");
      exit(1);
  }

  int ch;
  while ((ch = fgetc(input_file)) != EOF) {
      if (ch >= 0 && ch < ASCII_SIZE) {
          frequencies[ch]++;
      }
  }

  fclose(input_file);
}
```

### 3.2 Huffman Tree 생성

빈도 계산 후, 각 문자를 노드로 가지는 Huffman 트리를 생성한다. 우선순위 큐를 이용하여 빈도가 가장 낮은 두 노드를 반복적으로 결합하여 새로운 부모 노드를 생성하고, 이를 큐에 다시 삽입한다. 이 과정을 통해 최종적으로 Huffman 트리를 완성할 수 있다. 우선순위 큐는 최소 힙을 이용해 직접 구현하였다.

```c
typedef struct Node {
  char symbol;
  int freq;
  struct Node *left;
  struct Node *right;
} Node;

Node *build_huffman_tree() {
  for (int i = 0; i < ASCII_SIZE; i++) {
      if (frequencies[i] > 0) {
          Node *new_node = create_node(i, frequencies[i], NULL, NULL);
          heap_insert(new_node);
      }
  }

  while (HEAP_SIZE > 1) {
      Node *left = heap_delete();
      Node *right = heap_delete();
      Node *merged = create_node(0, left->freq + right->freq, left, right);
      heap_insert(merged);
  }

  return heap[1];
}
```

### 3.3 트리의 직렬화 및 파일 기록

Huffman Tree를 파일에 문자열로 저장하기 위해 트리를 직렬화한다. 트리를 Preoder Traversal로 순회하며 각 노드의 Symbol 값을 정수로 기록하는 방식을 사용하였다. 실제 심볼에 해당하는 leaf node는 해당 아스키 코드를 사용하고, 내부 노드는 0을 사용하여 표시하였다.

```c
void write_huffman_tree(Node *node, FILE *output) {
  if (!node) return;

  if (!node->left && !node->right) {
      fprintf(output, "%d ", node->symbol);
  } else {
      fprintf(output, "0 ");
  }

  write_huffman_tree(node->left, output);
  write_huffman_tree(node->right, output);
}
```

### 3.4 트리 재구성

압축 파일을 읽어 Huffman Tree를 재구성한 후, 이를 이용해 압축된 데이터를 복원한다.

```c
Node *read_huffman_tree(FILE *input) {
  int symbol;
  if (fscanf(input, "%d ", &symbol) != 1) {
      return NULL;
  }

  if (symbol != 0) {
      return create_node((char)symbol, 0, NULL, NULL);
  } else {
      Node *left = read_huffman_tree(input);
      Node *right = read_huffman_tree(input);
      return create_node(0, 0, left, right);
  }
}
```

## 4. 성능 분석

1. **시간 복잡도**: 빈도 계산은 O(n), 트리 생성은 O(nlog n)의 시간 복잡도를 가진다.
2. **공간 복잡도**: 추가적인 노드 및 힙 구조체를 사용하므로, 입력 데이터의 크기에 비례하는 공간 복잡도를 가진다.
