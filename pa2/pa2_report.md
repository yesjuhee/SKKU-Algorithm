# PA2 Report

## 1. 서론

PA2는 k(2≤ k ≤ 5)개의 DNA Sequences로 부터 Longest Common Subsequence(LCS)를 구하고 정렬하는 문제이다. 이 보고서를 통해 PA2를 해결하는 소스코드의 구성에 대해 설명하고, 그 성능을 분석하고자 한다.

## 2. 전체 소스코드 구성

해당 문제를 해결하기 위해 작성한 소스코드 목록은 다음과 같다.

```
Makefile
pa2.c
stack.c
stack.h
```

정렬된 DNA sequences를 저장하고 출력하기 위해 스택 자료 구조를 구현했으며, `Makefile`로 컴파일 한 후 `./pa2` 실행 파일을 실행시키면 된다. 문제를 해결하기 위한 주요 소스코드들은 모두 `pa2.c`에 작성되었다.

## 3. pa2.c 구성

`pa2.c`를 구성하는 함수들은 아래와 같다.

```c
void lcs_length_2(char *x, char *y);
void lcs_length_3(char *s1, char *s2, char *s3);
void lcs_length_4(char *s1, char *s2, char *s3, char *s4);
void lcs_length_5(char *s1, char *s2, char *s3, char *s4, char *s5);

void backtrack_lcs_length_2(int **c, int i, int j, Stack *x_out, Stack *y_out, Stack *out, char *x, char *y);
void backtrack_lcs_length_3(int ***c, int i, int j, int k, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *out, char *s1, char *s2, char *s3);
void backtrack_lcs_length_4(int ****c, int i, int j, int k, int l, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *s4_out, Stack *out, char *s1, char *s2, char *s3, char *s4);
void backtrack_lcs_length_5(unsigned char *****c, int i, int j, int k, int l, int m, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *s4_out, Stack *s5_out, Stack *out, char *s1, char *s2, char *s3, char *s4, char *s5);

void init_2d_array(int ***c, int m, int n);
void init_3d_array(int ****c, int l1, int l2, int l3);
void init_4d_array(int *****c, int l1, int l2, int l3, int l4);
void init_5d_array(unsigned char ******c, int l1, int l2, int l3, int l4, int l5);

void free_2d_array(int ***c, int m);
void free_3d_array(int ****c, int l1, int l2);
void free_4d_array(int ****c, int l1, int l2, int l3);
void free_5d_array(unsigned char *****c, int l1, int l2, int l3, int l4);

void file_in(int *k, char dna_sequences[5][130]);
void file_out(Stack *stack_array, int len);
```

입력된 `k`값에 해당하는 함수들이 사용된다.

- `lcs_length_k`: k개의 DNA sequences를 입력 받아 Dynamic Programming을 이용해 LCS length를 구한다. DP 과정에서 `c` 로 정의되는 `k` 차원의 배열을 만든다. `lcs_length_k`에서 완성된 `c` 테이블을 이용하여 `backtrack_lcs_length_k`를 호출한다.
- `backtrack_lcs_length_k`: `lcs_length_k`에서 완성된 `c` 테이블을 Backtracking하여 정렬된 DNA sequences와 LCS를 파일에 출력한다.
- `init_kd_array`, `free_kd_array`: LCS 길이를 구하기 위한 `c` 테이블을 초기화하고, free하는데 사용된다.
- `file_in`, `file_out`: 파일을 읽고 쓰는데 사용된다.

## 4. lcs_length_k

LCS 길이를 구하는 원리는 k값이 2~5일 때 모두 동일하다. 아래 소스코드는 `k = 3`일 때의 예시를 가져온 것이다.

```c
  int len1 = strlen(s1);
  int len2 = strlen(s2);
  int len3 = strlen(s3);
  int ***c;
  init_3d_array(&c, len1, len2, len3);

  for (int i = 1; i < len1; i++) {
      for (int j = 1; j < len2; j++) {
          for (int k = 1; k < len3; k++) {
              if (s1[i - 1] == s2[j - 1] && s2[j - 1] == s3[k - 1] && s3[k - 1] == s1[i - 1]) {
                  c[i][j][k] = c[i - 1][j - 1][k - 1] + 1;
              } else {
                  int maximum = max(c[i - 1][j][k], c[i][j - 1][k]);
                  maximum = max(maximum, c[i][j][k - 1]);
                  c[i][j][k] = maximum;
              }
          }
      }
  }
```

- `c[i][j][k]`는 첫번째 문자열의 i번째 prefix, 두번째 문자열의 j번째 prefix, 세번째 문자열의 k번째 prefix에서 LCS 길의 최대값을 나타낸다.
- `c[i][j][k]`의 값은 다음과 같이 결정된다.
  - 만약 각 문자열의 i번째, j번째, k번째 문자가 같다면
    `c[i][j][k] = c[i-1][j-1][k-1] + 1`
  - 아니라면
    `c[i][j][k] = max(c[i-1][j][k], c[i][j-1][k], c[i][j][k-1])`

`k`의 값에 따라서 `c`에서 사용되는 인덱스의 수, 즉 배열의 차원을 수정하여 소스코드를 구현하였다. 해당 코드는 각 문자열의 길이만큼 `for`문을 중첩하여 사용하기 때문에 DNA sequence의 최대 길이가 n이라고 하면 k개의 DNA sequences가 입력으로 주어졌을 때 **시간 복잡도는 $O(n^k)$ 이다.**

### 5. backtrack_lcs_k

위에서 작성된 `c` 테이블을 백트렉킹으로 탐색하며 아웃풋 텍스트 파일에 출력할 문자열을 Stack을 이용해 기록한다. 해당 함수는 `c`에 저장된 마지막 요소로부터 `c`의 첫번째 요소까지 탐색하기 때문에 DNA sequence의 최대 길이가 n이라고 하면 k개의 DNA sequences가 입력으로 주어졌을 때 **시간 복잡도는 $O(n*k)$ 이다.**
