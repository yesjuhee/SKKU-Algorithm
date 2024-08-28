# PA1 Report

## 1. 서론

Priority Queue는 가장 크거나 작은 element의 삽입, 삭제, 검색과 같은 작업을 지원하는 기본 데이터 구조이다. Min-Max-Median Priority Queue(MMMPQ)는 최소, 최대 및 중앙값에 빠르게 접근할 수 있는 자료구조이다. 이 보고서를 통해 Symmetric min-max heap(SMMH)를 사용하여 MMMPQ를 구현하는 방법을 설명하고 성능을 분석하고자 한다.

## 2. SMMH(symmetric min-max heap)

SMMH은 힙의 루트를 비워두고, 루트의 왼쪽 자식에는 최소값을, 오른쪽 자식에는 최대값을 저장하는 힙이다. 이 구조를 이용하여 힙의 최대값과 최소값에 모두 O(1)의 시간으로 접근 할 수 있다.

SMMH는 다음과 같은 세가지 성질을 가진다. 또한 세가지 성질을 만족하는 노드는 SMMH라고 할 수 있다.

P1. 왼쪽 자식은 오른쪽 자식보다 작거나 같다.

P2. 조부모 g를 갖는 노드 v에 대해 g의 왼쪽 자식의 값은 v의 값보다 작거나 같다.

P3. 조부모 g를 갖는 노드 v에 대해 g의 오른쪽 자식의 값은 v의 값보다 크거나 같다.

다음은 소스코드에서 SMMH를 구현한 함수들의 정의부이다. min heap, max heap과 마찬가지로 SMMH도 삽입, 삭제, 검색 연산을 제공한다. 또한 마찬가지로 삽입과 삭제의 시간복잡도는 **O(lgn)**이고 검색의 시간 복잡도는 **O(1)**이다.

```c
// symmetric min-max heap
void swap(int idx1, int idx2, int is_h1);
int parent_idx(int idx);
int left_child_idx(int idx);
int right_child_idx(int idx);
int is_right_child(int idx);
int last_idx(int* h);

void h_insert(int element, int is_h1);
int h_delete_min(int is_h1);
int h_delete_max(int is_h1);
int h_find_min(int is_h1);
int h_find_max(int is_h1);
```

min heap, max heap의 삽입/삭제 연산과 비슷하게, SMMH의 삽입/삭제 연산도 heapify-up과 heapify-down을 활용한다. 이때 힙이 P1, P2, P3의 성질을 유지하도록 해야 한다.

## 3. MMMPQ(min-max-median priority queue)

다음은 소스코드에서 MMMPQ를 구현하기 위해 요구된 함수들의 정의부이다.

```c
//  min-max-median priority queue
void insert(int element);

int delete_min();
int delete_max();
int delete_median();

int find_min();
int find_max();
int find_median();
```

MMMPQ를 구현하기 위해 다음과 같이 두개의 SMMH `h1` 과 `h2`를 정의하였다. 큐의 요소들을 `h1`과 `h2`를 이용해서 관리할 수 있다.

```c
// data structure
int h1[50000 / 2];
int h2[50000 / 2];
```

두 힙은 다음과 같은 조건을 만족해야 한다.

1.  **`h1`의 모든 요소는 `h2`의 모든 요소보다 작다. (조건1)**
2.  **두 힙에 저장되는 요소의 수는 항상 같거나 `h1`이 하나가 많다. (조건2)**

조건1과 조건2가 만족되는 경우 전체 큐의 최소값은 `h1`의 최소값, 전체 큐의 최대값은 `h2`의 최대값, 전체 큐의 중앙값은 `h1`의 최대값을 이용해 접근할 수 있다.

MMMPQ의 삽입, 삭제, 검색 연산을 살펴보자.

### insert

```c
void insert(int new_element) {
    // insert
    int total_count = h1[0] + h2[0];
    if (total_count == 0) {         // 빈 힙이 있는 경우 따로 처리
        h_insert(new_element, 1);
        return;
    } else if (total_count == 1) {
        h_insert(new_element, 0);
        return;
    }

    if (total_count % 2 == 0) {     // 짝수개일 경우
        h_insert(new_element, 1);   // h1에 insert
    } else {                        // 홀수개일 경우
        h_insert(new_element, 0);   // h2에 insert
    }

    // print();

    // 비교
    int h1_max = h_find_max(1);
    int h2_min = h_find_min(0);
    if (h1_max > h2_min) {          // h1의 최대값이 h2의 최소값보다 클 경우 둘을 swap
        h1_max = h_delete_max(1);
        h2_min = h_delete_min(0);
        h_insert(h1_max, 0);
        h_insert(h2_min, 1);
    }
}
```

1. 만약 큐가 비어있다면 예외 처리를 해준다. 힙의 0번째 인덱스에는 힙의 노드 개수를 저장하기 때문에 `h1[0]`과 `h2[0]` 의 합이 0일 때 예외 처리를 해준다.

   ```c
   int total_count = h1[0] + h2[0];
   if (total_count == 0) {         // 빈 힙이 있는 경우 따로 처리
       h_insert(new_element, 1);
       return;
   } else if (total_count == 1) {
       h_insert(new_element, 0);
       return;
   }
   ```

2. `new_element` 를 `h1` 과 `h2` 중 어디에 삽입할 것인지 결정한다. **조건1**이 항상 만족될 수 있도록, 전체 저장된 개수가 짝수개일 경우 `h1`에 `new_element`를 삽입하고, 홀수개일 경우 `h2`에 삽입한다.

   ```c
   if (total_count % 2 == 0) {     // 짝수개일 경우
       h_insert(new_element, 1);   // h1에 insert
   } else {                        // 홀수개일 경우
       h_insert(new_element, 0);   // h2에 insert
   }
   ```

3. 알맞은 힙에 `new_element`를 삽입한 후, **조건1**을 만족하는지 확인해야 한다. `h1`의 모든 요소가 `h2`의 모든 요소보다 작아야 하므로 `h1`의 최대값이 `h2`의 최소값보다 클 경우 둘을 교체한다.

   ```c
   // 비교
   int h1_max = h_find_max(1);
   int h2_min = h_find_min(0);
   if (h1_max > h2_min) {          // h1의 최대값이 h2의 최소값보다 클 경우 둘을 swap
       h1_max = h_delete_max(1);
       h2_min = h_delete_min(0);
       h_insert(h1_max, 0);
       h_insert(h2_min, 1);
   }
   ```

SMMH를 이용해 구현한 MMMPQ의 삽입 연산은 여러번의 힙 연산(삽입, 삭제, 검색)이 수행된다. 힙 연산은 **O(lgn)**의 시간 복잡도를 가지므로 **삽입 연산의 시간 복잡도는 O(lgn)이다.**

## Delete

```c
int delete_min() {
    int min = h_delete_min(1);      // h1에서 delete minimum
    if (h1[0] < h2[0]) {            // h1은 항상 h2보다 많거나 같도록 유지
        h_insert(h_delete_min(0), 1);
    }
    return min;
}
```

1. 큐의 최소값은 `h1`의 최소값이므로 `h1`에서 `h_delete_min`을 수행한 결과를 반환한다.
2. 반환 후 두 힙이 **조건1**을 만족시키는지 확인한다.

```c
int delete_max() {
    if ((h1[0] + h2[0]) == 1) {     // 예외 처리 : 큐에 원소가 한 개 남았을 때
        return h_delete_max(1);     // h1에서 delete maximum
    }
    int max = h_delete_max(0);      // h2에서 delete maximum
    if (h1[0] - h2[0] > 1) {        // 두 힙의 개수 차이는 최대 1개로 유지
        h_insert(h_delete_max(1), 0);
    }
    return max;
}
```

1. 큐의 최대값은 `h2`의 최대값이므로 `h2`에서 `h_delete_max`을 수행한 결과를 반환한다.
2. 반환 후 두 힙이 **조건1**을 만족시키는지 확인한다.

```c
int delete_median() {
    int medi = h_delete_max(1);     // h1에서 delete max
    if(h1[0] < h2[0]) {             // h1은 항상 h2보다 많거나 같도록 유지
        h_insert(h_delete_min(0), 1);
    }
    return medi;
}
```

1. 큐의 중앙값은 `h1`의 최대값이므로 `h1`에서 `h_delete_max`을 수행한 결과를 반환한다.
2. 반환 후 두 힙이 **조건1**을 만족시키는지 확인한다.

삭제를 하는 세 연산 모두 SMMH의 삽입/삭제 연산이 최대 3번까지 실행될 수 있으므로 **삭제 연산의 시간 복잡도는 O(lgn)이다.**

## Find

```c
int find_min() {
    return h_find_min(1);           // h1의 minimum return
}
int find_max() {
    if (h1[0] + h2[0] == 1) {       // 예외 처리 : 남은 원소가 한 개 일 때
        return h1[2];               // h1에서 return
    }
    return h_find_max(0);           // h2의 maximum return
}
int find_median() {
    return h_find_max(1);           // h1의 maximum return
}
```

검색 연산은 힙의 특정 노드에 인덱스를 이용해 접근하면 되기 때문에 **검색 연산의 시간 복잡도는 O(1)이다.**
