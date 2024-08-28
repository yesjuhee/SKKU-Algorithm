#include <stdio.h>
#include <stdlib.h>

//  min-max-median priority queue
void insert(int element);

int delete_min();
int delete_max();
int delete_median();

int find_min();
int find_max();
int find_median();

// symmetric min-max heap
void swap(int idx1, int idx2, int is_h1);
int parent_idx(int idx);
int left_child_idx(int idx);
int right_child_idx(int idx);
int is_right_child(int idx);
int last_idx(int *h);

void h_insert(int element, int is_h1);
int h_delete_min(int is_h1);
int h_delete_max(int is_h1);
int h_find_min(int is_h1);
int h_find_max(int is_h1);

// data structure
int h1[500000 / 2];
int h2[500000 / 2];

/*
void print() {
    // h1 출력
    printf("h1: ");
    for (int i = 2; i <= h1[0] + 1; i++) printf("%d ", h1[i]);
    printf("\n");
    // h2 출력
    printf("h2: ");
    for (int i = 2; i <= h2[0] + 1; i++) printf("%d ", h2[i]);
    printf("\n\n");
}
*/

int main(void) {
    h1[0] = 0;
    h2[0] = 0;
    int find_count = 0; // find 연산 횟수
    int output[50000];  // find 결과 저장

    int operation_num, new_element, i;
    char operation_type, operation_target;
    scanf("%d", &operation_num);

    for (i = 0; i < operation_num; i++) {
        scanf(" %c", &operation_type);

        if (operation_type == 'I') {
            scanf("%d", &new_element);
            insert(new_element);
        } else if (operation_type == 'D') {
            scanf(" %c", &operation_target);
            if (h1[0] == 0)
                continue; // 큐가 비어있으면 삭제 연산 실행하지 않음
            if (operation_target == 'M')
                delete_min();
            else if (operation_target == 'X')
                delete_max();
            else if (operation_target == 'E')
                delete_median();
        } else if (operation_type == 'F') {
            scanf(" %c", &operation_target);
            if (h1[0] == 0)
                continue; // 큐가 비어있으면 find 연산 실행하지 않음
            else if (operation_target == 'M')
                output[find_count] = find_min();
            else if (operation_target == 'X')
                output[find_count] = find_max();
            else if (operation_target == 'E')
                output[find_count] = find_median();
            find_count++;
        } else {
            printf("잘못된 operation_type\n");
        }
    }

    // find 결과물 출력
    for (int i = 0; i < find_count; i++) {
        printf("%d\n", output[i]);
    }

    return 0;
}

void insert(int new_element) {
    // insert
    int total_count = h1[0] + h2[0];
    if (total_count == 0) { // 빈 힙이 있는 경우 따로 처리
        h_insert(new_element, 1);
        return;
    } else if (total_count == 1) {
        h_insert(new_element, 0);
        return;
    }

    if (total_count % 2 == 0) {   // 짝수개일 경우
        h_insert(new_element, 1); // h1에 insert
    } else {                      // 홀수개일 경우
        h_insert(new_element, 0); // h2에 insert
    }

    // print();

    // 비교
    int h1_max = h_find_max(1);
    int h2_min = h_find_min(0);
    if (h1_max > h2_min) { // h1의 최대값이 h2의 최소값보다 클 경우 둘을 swap
        h1_max = h_delete_max(1);
        h2_min = h_delete_min(0);
        h_insert(h1_max, 0);
        h_insert(h2_min, 1);
    }
}

int delete_min() {
    int min = h_delete_min(1); // h1에서 delete minimum
    if (h1[0] < h2[0]) {       // h1은 항상 h2보다 많거나 같도록 유지
        h_insert(h_delete_min(0), 1);
    }
    return min;
}
int delete_max() {
    if ((h1[0] + h2[0]) == 1) { // 예외 처리 : 큐에 원소가 한 개 남았을 때
        return h_delete_max(1); // h1에서 delete maximum
    }
    int max = h_delete_max(0); // h2에서 delete maximum
    if (h1[0] - h2[0] > 1) {   // 두 힙의 개수 차이는 최대 1개로 유지
        h_insert(h_delete_max(1), 0);
    }
    return max;
}
int delete_median() {
    int medi = h_delete_max(1); // h1에서 delete max
    if (h1[0] < h2[0]) {        // h1은 항상 h2보다 많거나 같도록 유지
        h_insert(h_delete_min(0), 1);
    }
    return medi;
}

int find_min() {
    return h_find_min(1); // h1의 minimum return
}
int find_max() {
    if (h1[0] + h2[0] == 1) { // 예외 처리 : 남은 원소가 한 개 일 때
        return h1[2];         // h1에서 return
    }
    return h_find_max(0); // h2의 maximum return
}
int find_median() {
    return h_find_max(1); // h1의 maximum return
}

void swap(int idx1, int idx2, int is_h1) {
    int temp;
    if (is_h1) {
        temp = h1[idx1];
        h1[idx1] = h1[idx2];
        h1[idx2] = temp;
    } else {
        temp = h2[idx1];
        h2[idx1] = h2[idx2];
        h2[idx2] = temp;
    }
}
int parent_idx(int idx) {
    return idx / 2;
}
int left_child_idx(int idx) {
    return idx * 2;
}
int right_child_idx(int idx) {
    return idx * 2 + 1;
}
int is_right_child(int idx) {
    if (idx % 2 == 1)
        return 1;
    return 0;
}
int last_idx(int *h) {
    return h[0] + 1;
}

void h_insert(int new_element, int is_h1) {
    int *h; // symmetric min-max heap
    if (is_h1) {
        h = h1;
    } else {
        h = h2;
    }

    // 총 개수 +1 하고 맨 뒤에 insert element
    h[0] += 1;
    int target_idx = last_idx(h);
    h[target_idx] = new_element;

    // P1 검증 : new_element가 왼쪽 형제보다 커야한다.
    if (is_right_child(target_idx) && new_element < h[target_idx - 1]) {
        swap(target_idx, target_idx - 1, is_h1);
        target_idx--;
    }

    // 반복해서 P2/P3 검증 하며 heapify-up
    while (1) {
        int grand_parent_idx = parent_idx(parent_idx(target_idx)); // 조부모가 없으면 0
        int gp_left_child_idx = left_child_idx(grand_parent_idx);
        int gp_right_child_idx = right_child_idx(grand_parent_idx);
        // P2 검증 : 조부모의 왼쪽 자식보다 커야한다.
        if (grand_parent_idx && new_element < h[gp_left_child_idx]) {
            swap(target_idx, gp_left_child_idx, is_h1);
            target_idx = gp_left_child_idx; // heapify-up
        }
        // P3 검증 : 조부모의 오른쪽 자식보다 작아야한다.
        else if (grand_parent_idx && new_element > h[gp_right_child_idx]) {
            swap(target_idx, gp_right_child_idx, is_h1);
            target_idx = gp_right_child_idx; // heapify-up
        } else {
            // P2/P3 조건을 만족한다.
            break;
        }
    }
}
int h_delete_min(int is_h1) {
    int *h; // symmetric min-max heap
    if (is_h1) {
        h = h1;
    } else {
        h = h2;
    }

    // empty heap : prohibited exciton
    if (h[0] == 0)
        exit(1);

    int min_idx = 2;
    int target_idx = last_idx(h); // heapify-down 할 인덱스
    int min = h[min_idx];
    h[0] -= 1;

    // swap min_idx, target_idx
    swap(min_idx, target_idx, is_h1);
    target_idx = min_idx;

    // P1, P2 반복 검증하며 heapify-down
    while (1) {
        // P1 검증 : 오른쪽 형제가 없거나, 오른쪽 형제보다 작아야 한다.
        if ((target_idx + 1) <= (last_idx(h)) && h[target_idx] > h[target_idx + 1]) {
            swap(target_idx, target_idx + 1, is_h1);
        }

        // P2 검증 : 자식이 없거나, (자신의 자식 + 형제의 자식) 중 최소값보다 작야아 한다
        // 자식이 없으면 종료한다.
        if (left_child_idx(target_idx) > last_idx(h)) {
            break;
        }
        // (자기 자식 + 형제 자식) 중 최소값 인덱스 찾기
        int m1_idx, m2_idx, local_min_idx;
        m1_idx = left_child_idx(target_idx);
        m2_idx = left_child_idx(target_idx + 1);
        if (m2_idx > last_idx(h)) {
            // 형제 자식이 존재하지 않음
            local_min_idx = m1_idx;
        } else {
            // 형제 자식이 존재함
            local_min_idx = h[m1_idx] < h[m2_idx] ? m1_idx : m2_idx;
        }
        if (h[target_idx] > h[local_min_idx]) {
            swap(target_idx, local_min_idx, is_h1);
            target_idx = local_min_idx;
        } else {
            break; // P2 만족하면 반복문 종료
        }
    }

    // return minimum
    return min;
}
int h_delete_max(int is_h1) {
    int *h; // symmetric min-max heap
    if (is_h1) {
        h = h1;
    } else {
        h = h2;
    }

    // empty heap : prohibited action
    if (h[0] == 0)
        exit(1);
    // only one element
    else if (h[0] == 1) {
        h[0] -= h[0];
        return h[2];
    }

    int max_idx = 3;
    int target_idx = last_idx(h); // heapify-down 할 인덱스
    int max = h[max_idx];
    h[0] -= 1;

    // swap max_idx, target_idx
    swap(max_idx, target_idx, is_h1);
    target_idx = max_idx;

    // P1, P2 반복 검증하며 heapify-down
    while (1) {
        // P1 검증 : 왼쪽 형제보다 커야 한다.
        if (h[target_idx - 1] > h[target_idx]) {
            swap(target_idx - 1, target_idx, is_h1);
        }

        // P3 검증 : 조카가 없거나, (자신의 자식 + 형제의 자식) 중 최대값보다 커야아 한다
        // 조카가 없으면 종료한다.
        if (left_child_idx(target_idx - 1) >= last_idx(h)) {
            break;
        }
        // (자기 자식 + 형제 자식) 중 최대값 인덱스 찾기
        int m1_idx, m2_idx, local_max_idx;
        m1_idx = right_child_idx(target_idx - 1); // 형제 자식
        m2_idx = right_child_idx(target_idx);     // 자기 자식
        if (m2_idx > last_idx(h)) {
            // 자기 자식이 존재하지 않음
            local_max_idx = m1_idx;
        } else {
            // 자기 자식이 존재함
            local_max_idx = h[m1_idx] > h[m2_idx] ? m1_idx : m2_idx;
        }
        if (h[target_idx] < h[local_max_idx]) {
            swap(target_idx, local_max_idx, is_h1);
            target_idx = local_max_idx;
        } else {
            break; // P3 만족하면 반복문 종료
        }
    }

    // return minimum
    return max;
}
int h_find_min(int is_h1) {
    int *h;
    if (is_h1) {
        h = h1;
    } else {
        h = h2;
    }

    if (h[0] == 0)
        exit(1);
    else if (h[0] == 1)
        return h[2];
    else
        return h[2];
}
int h_find_max(int is_h1) {
    int *h;
    if (is_h1) {
        h = h1;
    } else {
        h = h2;
    }

    if (h[0] == 0)
        exit(1);
    else if (h[0] == 1)
        return h[2];
    else
        return h[3];
}