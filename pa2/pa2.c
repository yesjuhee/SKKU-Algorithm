#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_FILE_NAME "hw2_input.txt"
#define OUT_FILE_NAME "hw2_output.txt"

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

void file_out(Stack *stack_array, int len);
void file_in(int *k, char dna_sequences[5][130]);
int max(int a, int b);

int main() {
    char dna_sequences[5][130];
    int k;

    file_in(&k, dna_sequences);

    if (k == 2) {
        lcs_length_2(dna_sequences[0], dna_sequences[1]);
    } else if (k == 3) {
        lcs_length_3(dna_sequences[0], dna_sequences[1], dna_sequences[2]);
    } else if (k == 4) {
        lcs_length_4(dna_sequences[0], dna_sequences[1], dna_sequences[2], dna_sequences[3]);
    } else if (k == 5) {
        lcs_length_5(dna_sequences[0], dna_sequences[1], dna_sequences[2], dna_sequences[3], dna_sequences[4]);
    }

    return 0;
}

void lcs_length_2(char *x, char *y) {
    int m = strlen(x);
    int n = strlen(y);
    int **c;
    init_2d_array(&c, m, n);

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            if (x[i - 1] == y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
            } else {
                c[i][j] = max(c[i - 1][j], c[i][j - 1]);
            }
        }
    }

    Stack x_out;
    Stack y_out;
    Stack out;
    init_stack(&x_out, m + n);
    init_stack(&y_out, m + n);
    init_stack(&out, m + n);

    backtrack_lcs_length_2(c, m - 1, n - 1, &x_out, &y_out, &out, x, y);

    // print stack
    Stack stack_array[3] = { x_out, y_out, out };
    file_out(stack_array, 3);
    free_2d_array(&c, m);

    return;
}
void lcs_length_3(char *s1, char *s2, char *s3) {
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

    Stack s1_out;
    Stack s2_out;
    Stack s3_out;
    Stack out;
    init_stack(&s1_out, len1 + len2 + len3);
    init_stack(&s2_out, len1 + len2 + len3);
    init_stack(&s3_out, len1 + len2 + len3);
    init_stack(&out, len1 + len2 + len3);

    backtrack_lcs_length_3(c, len1 - 1, len2 - 1, len3 - 1, &s1_out, &s2_out, &s3_out, &out, s1, s2, s3);

    // print stack
    Stack stack_array[4] = { s1_out, s2_out, s3_out, out };
    file_out(stack_array, 4);
    free_3d_array(&c, len1, len2);
}
void lcs_length_4(char *s1, char *s2, char *s3, char *s4) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len3 = strlen(s3);
    int len4 = strlen(s4);
    int ****c;
    init_4d_array(&c, len1, len2, len3, len4);

    for (int i = 1; i < len1; i++) {
        for (int j = 1; j < len2; j++) {
            for (int k = 1; k < len3; k++) {
                for (int l = 1; l < len4; l++) {
                    if (s1[i - 1] == s2[j - 1] && s1[i - 1] == s3[k - 1] && s1[i - 1] == s4[l - 1] && s2[j - 1] == s3[k - 1] && s2[j - 1] == s4[l - 1] && s3[k - 1] == s4[l - 1]) {
                        c[i][j][k][l] = c[i - 1][j - 1][k - 1][l - 1] + 1;
                    } else {
                        int maximum = max(c[i - 1][j][k][l], c[i][j - 1][k][l]);
                        maximum = max(maximum, c[i][j][k - 1][l]);
                        maximum = max(maximum, c[i][j][k][l - 1]);
                        c[i][j][k][l] = maximum;
                    }
                }
            }
        }
    }

    Stack s1_out;
    Stack s2_out;
    Stack s3_out;
    Stack s4_out;
    Stack out;
    init_stack(&s1_out, len1 + len2 + len3 + len4);
    init_stack(&s2_out, len1 + len2 + len3 + len4);
    init_stack(&s3_out, len1 + len2 + len3 + len4);
    init_stack(&s4_out, len1 + len2 + len3 + len4);
    init_stack(&out, len1 + len2 + len3 + len4);

    backtrack_lcs_length_4(c, len1 - 1, len2 - 1, len3 - 1, len4 - 1, &s1_out, &s2_out, &s3_out, &s4_out, &out, s1, s2, s3, s4);

    // print stack
    Stack stack_array[5] = { s1_out, s2_out, s3_out, s4_out, out };
    file_out(stack_array, 5);
    free_4d_array(c, len1, len2, len3);
}
void lcs_length_5(char *s1, char *s2, char *s3, char *s4, char *s5) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len3 = strlen(s3);
    int len4 = strlen(s4);
    int len5 = strlen(s5);
    unsigned char *****c;
    init_5d_array(&c, len1, len2, len3, len4, len5);

    for (int i = 1; i < len1; i++) {
        for (int j = 1; j < len2; j++) {
            for (int k = 1; k < len3; k++) {
                for (int l = 1; l < len4; l++) {
                    for (int m = 1; m < len5; m++) {
                        if (s1[i - 1] == s2[j - 1] && s1[i - 1] == s3[k - 1] && s1[i - 1] == s4[l - 1] && s1[i - 1] == s5[m - 1] && s2[j - 1] == s3[k - 1] && s2[j - 1] == s4[l - 1] && s2[j - 1] == s5[m - 1] && s3[k - 1] == s4[l - 1] && s3[k - 1] == s5[m - 1] && s4[l - 1] == s5[m - 1]) {
                            c[i][j][k][l][m] = c[i - 1][j - 1][k - 1][l - 1][m - 1] + 1;
                        } else {
                            unsigned char maximum = max(c[i - 1][j][k][l][m], c[i][j - 1][k][l][m]);
                            maximum = max(maximum, c[i][j][k - 1][l][m]);
                            maximum = max(maximum, c[i][j][k][l - 1][m]);
                            maximum = max(maximum, c[i][j][k][l][m - 1]);
                            c[i][j][k][l][m] = maximum;
                        }
                    }
                }
            }
        }
    }

    // printf("lcs_length: %d\n", c[len1 - 1][len2 - 1][len3 - 1][len4 - 1][len5 - 1]);
    Stack s1_out;
    Stack s2_out;
    Stack s3_out;
    Stack s4_out;
    Stack s5_out;
    Stack out;
    init_stack(&s1_out, len1 + len2 + len3 + len4 + len5);
    init_stack(&s2_out, len1 + len2 + len3 + len4 + len5);
    init_stack(&s3_out, len1 + len2 + len3 + len4 + len5);
    init_stack(&s4_out, len1 + len2 + len3 + len4 + len5);
    init_stack(&s5_out, len1 + len2 + len3 + len4 + len5);
    init_stack(&out, len1 + len2 + len3 + len4 + len5);

    backtrack_lcs_length_5(c, len1 - 1, len2 - 1, len3 - 1, len4 - 1, len5 - 1, &s1_out, &s2_out, &s3_out, &s4_out, &s5_out, &out, s1, s2, s3, s4, s5);

    // print stack
    Stack stack_array[6] = { s1_out, s2_out, s3_out, s4_out, s5_out, out };
    file_out(stack_array, 6);
    free_5d_array(c, len1, len2, len3, len4);
}

void backtrack_lcs_length_2(int **c, int i, int j, Stack *x_out, Stack *y_out, Stack *out, char *x, char *y) {
    while (i > 0 || j > 0) {
        if (i == 0) {
            while (j > 0) {
                push(x_out, '-');
                push(y_out, y[--j]);
                push(out, ' ');
            }
            break;
        } else if (j == 0) {
            while (i > 0) {
                push(x_out, x[--i]);
                push(y_out, '-');
                push(out, ' ');
            }
            break;
        }

        if (x[i - 1] == y[j - 1]) {
            // 대각선 방향 이동
            push(x_out, x[--i]);
            push(y_out, y[--j]);
            push(out, '*');
        } else {
            if (c[i][j - 1] >= c[i - 1][j]) {
                // 왼쪽으로 이동
                push(x_out, '-');
                push(y_out, y[--j]);
                push(out, ' ');
            } else {
                // 위쪽으로 이동
                push(x_out, x[--i]);
                push(y_out, '-');
                push(out, ' ');
            }
        }
    }
}
void backtrack_lcs_length_3(int ***c, int i, int j, int k, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *out, char *s1, char *s2, char *s3) {
    while (i > 0 || j > 0 || k > 0) {
        // break point
        if (i == 0) {
            while (j > 0 || k > 0) {
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                push(s1_out, '-');
                push(out, ' ');
            }
            break;
        } else if (j == 0) {
            while (i > 0 || k > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                push(s2_out, '-');
                push(out, ' ');
            }
            break;
        } else if (k == 0) {
            while (i > 0 || j > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                push(s3_out, '-');
                push(out, ' ');
            }
            break;
        }

        // move
        if (s1[i - 1] == s2[j - 1] && s2[j - 1] == s3[k - 1] && s3[k - 1] == s1[i - 1]) {
            // 대각선 방향 이동
            push(s1_out, s1[--i]);
            push(s2_out, s2[--j]);
            push(s3_out, s3[--k]);
            push(out, '*');
        } else {
            // max 찾기
            int maximum = max(c[i - 1][j][k], max(c[i][j - 1][k], c[i][j][k - 1]));
            if (c[i - 1][j][k] == maximum) {
                push(s1_out, s1[--i]);
            } else {
                push(s1_out, '-');
            }
            if (c[i][j - 1][k] == maximum) {
                push(s2_out, s2[--j]);
            } else {
                push(s2_out, '-');
            }
            if (c[i][j][k - 1] == maximum) {
                push(s3_out, s3[--k]);
            } else {
                push(s3_out, '-');
            }
            push(out, ' ');
        }
    }
}
void backtrack_lcs_length_4(int ****c, int i, int j, int k, int l, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *s4_out, Stack *out, char *s1, char *s2, char *s3, char *s4) {
    while (i > 0 || j > 0 || k > 0 || l > 0) {
        // break point
        if (i == 0) {
            while (j > 0 || k > 0 || l > 0) {
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                push(s1_out, '-');
                push(out, ' ');
            }
            break;
        } else if (j == 0) {
            while (i > 0 || k > 0 || l > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                push(s2_out, '-');
                push(out, ' ');
            }
            break;
        } else if (k == 0) {
            while (i > 0 || j > 0 || l > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                push(s3_out, '-');
                push(out, ' ');
            }
            break;
        } else if (l == 0) {
            while (i > 0 || j > 0 || k > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                push(s4_out, '-');
                push(out, ' ');
            }
            break;
        }

        // move
        if (s1[i - 1] == s2[j - 1] && s1[i - 1] == s3[k - 1] && s1[i - 1] == s4[l - 1] && s2[j - 1] == s3[k - 1] && s2[j - 1] == s4[l - 1] && s3[k - 1] == s4[l - 1]) {
            // 대각선 방향 이동
            push(s1_out, s1[--i]);
            push(s2_out, s2[--j]);
            push(s3_out, s3[--k]);
            push(s4_out, s4[--l]);
            push(out, '*');
        } else {
            // max 찾기
            int maximum = max(c[i - 1][j][k][l], max(c[i][j - 1][k][l], max(c[i][j][k - 1][l], c[i][j][k][l - 1])));
            if (c[i - 1][j][k][l] == maximum) {
                push(s1_out, s1[--i]);
            } else {
                push(s1_out, '-');
            }
            if (c[i][j - 1][k][l] == maximum) {
                push(s2_out, s2[--j]);
            } else {
                push(s2_out, '-');
            }
            if (c[i][j][k - 1][l] == maximum) {
                push(s3_out, s3[--k]);
            } else {
                push(s3_out, '-');
            }
            if (c[i][j][k][l - 1] == maximum) {
                push(s4_out, s4[--l]);
            } else {
                push(s4_out, '-');
            }
            push(out, ' ');
        }
    }
}
void backtrack_lcs_length_5(unsigned char *****c, int i, int j, int k, int l, int m, Stack *s1_out, Stack *s2_out, Stack *s3_out, Stack *s4_out, Stack *s5_out, Stack *out, char *s1, char *s2, char *s3, char *s4, char *s5) {
    while (i > 0 || j > 0 || k > 0 || l > 0 || m > 0) {
        // break point
        if (i == 0) {
            while (j > 0 || k > 0 || l > 0 || m > 0) {
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                if (m == 0) {
                    push(s5_out, '-');
                } else {
                    push(s5_out, s5[--m]);
                }
                push(s1_out, '-');
                push(out, ' ');
            }
            break;
        } else if (j == 0) {
            while (i > 0 || k > 0 || l > 0 || m > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                if (m == 0) {
                    push(s5_out, '-');
                } else {
                    push(s5_out, s5[--m]);
                }
                push(s2_out, '-');
                push(out, ' ');
            }
            break;
        } else if (k == 0) {
            while (i > 0 || j > 0 || l > 0 || m > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                if (m == 0) {
                    push(s5_out, '-');
                } else {
                    push(s5_out, s5[--m]);
                }
                push(s3_out, '-');
                push(out, ' ');
            }
            break;
        } else if (l == 0) {
            while (i > 0 || j > 0 || k > 0 || m > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (m == 0) {
                    push(s5_out, '-');
                } else {
                    push(s5_out, s5[--m]);
                }
                push(s4_out, '-');
                push(out, ' ');
            }
            break;
        } else if (m == 0) {
            while (i > 0 || j > 0 || k > 0 || l > 0) {
                if (i == 0) {
                    push(s1_out, '-');
                } else {
                    push(s1_out, s1[--i]);
                }
                if (j == 0) {
                    push(s2_out, '-');
                } else {
                    push(s2_out, s2[--j]);
                }
                if (k == 0) {
                    push(s3_out, '-');
                } else {
                    push(s3_out, s3[--k]);
                }
                if (l == 0) {
                    push(s4_out, '-');
                } else {
                    push(s4_out, s4[--l]);
                }
                push(s5_out, '-');
                push(out, ' ');
            }
            break;
        }

        // move
        if (s1[i - 1] == s2[j - 1] && s1[i - 1] == s3[k - 1] && s1[i - 1] == s4[l - 1] && s1[i - 1] == s5[m - 1] && s2[j - 1] == s3[k - 1] && s2[j - 1] == s4[l - 1] && s2[j - 1] == s5[m - 1] && s3[k - 1] == s4[l - 1] && s3[k - 1] == s5[m - 1] && s4[l - 1] == s5[m - 1]) {
            // 대각선 방향 이동
            push(s1_out, s1[--i]);
            push(s2_out, s2[--j]);
            push(s3_out, s3[--k]);
            push(s4_out, s4[--l]);
            push(s5_out, s5[--m]);
            push(out, '*');
        } else {
            // max 찾기
            unsigned char maximum = max(c[i - 1][j][k][l][m], c[i][j - 1][k][l][m]);
            maximum = max(maximum, c[i][j][k - 1][l][m]);
            maximum = max(maximum, c[i][j][k][l - 1][m]);
            maximum = max(maximum, c[i][j][k][l][m - 1]);
            if (c[i - 1][j][k][l][m] == maximum) {
                push(s1_out, s1[--i]);
            } else {
                push(s1_out, '-');
            }
            if (c[i][j - 1][k][l][m] == maximum) {
                push(s2_out, s2[--j]);
            } else {
                push(s2_out, '-');
            }
            if (c[i][j][k - 1][l][m] == maximum) {
                push(s3_out, s3[--k]);
            } else {
                push(s3_out, '-');
            }
            if (c[i][j][k][l - 1][m] == maximum) {
                push(s4_out, s4[--l]);
            } else {
                push(s4_out, '-');
            }
            if (c[i][j][k][l][m - 1] == maximum) {
                push(s5_out, s5[--m]);
            } else {
                push(s5_out, '-');
            }
            push(out, ' ');
        }
    }
}

void init_2d_array(int ***c, int m, int n) {
    // Allocate memory for the array of pointers
    *c = (int **)calloc(m, sizeof(int *));

    // Allocate memory for each row
    for (int i = 0; i < m; i++) {
        (*c)[i] = (int *)calloc(n, sizeof(int));
    }
}
void init_3d_array(int ****c, int l1, int l2, int l3) {
    *c = (int ***)calloc(l1, sizeof(int **));
    for (int i = 0; i < l1; i++) {
        (*c)[i] = calloc(l2, sizeof(int *));
        for (int j = 0; j < l2; j++) {
            (*c)[i][j] = calloc(l3, sizeof(int));
        }
    }
}
void init_4d_array(int *****c, int l1, int l2, int l3, int l4) {
    *c = (int ****)calloc(l1, sizeof(int ***));
    for (int i = 0; i < l1; i++) {
        (*c)[i] = calloc(l2, sizeof(int **));
        for (int j = 0; j < l2; j++) {
            (*c)[i][j] = calloc(l3, sizeof(int *));
            for (int k = 0; k < l3; k++) {
                (*c)[i][j][k] = calloc(l4, sizeof(int));
            }
        }
    }
}
void init_5d_array(unsigned char ******c, int l1, int l2, int l3, int l4, int l5) {
    *c = (unsigned char *****)calloc(l1, sizeof(unsigned char ****));
    for (unsigned char i = 0; i < l1; i++) {
        (*c)[i] = calloc(l2, sizeof(unsigned char ***));
        for (unsigned char j = 0; j < l2; j++) {
            (*c)[i][j] = calloc(l3, sizeof(unsigned char **));
            for (unsigned char k = 0; k < l3; k++) {
                (*c)[i][j][k] = calloc(l4, sizeof(unsigned char *));
                for (unsigned char l = 0; l < l4; l++) {
                    (*c)[i][j][k][l] = calloc(l5, sizeof(unsigned char));
                }
            }
        }
    }
}

void free_2d_array(int ***c, int m) {
    // Free each row
    for (int i = 0; i < m; i++) {
        free((*c)[i]);
    }
    // Free the array of pointers
    free(*c);
}
void free_3d_array(int ****c, int l1, int l2) {
    for (int i = 0; i < l1; i++) {
        for (int j = 0; j < l2; j++) {
            free((*c)[i][j]);
        }
        free((*c)[i]);
    }
    free(*c);
}
void free_4d_array(int ****c, int l1, int l2, int l3) {
    for (int i = 0; i < l1; i++) {
        for (int j = 0; j < l2; j++) {
            for (int k = 0; k < l3; k++) {
                free(c[i][j][k]);
            }
            free(c[i][j]);
        }
        free(c[i]);
    }
}
void free_5d_array(unsigned char *****c, int l1, int l2, int l3, int l4) {
    for (int i = 0; i < l1; i++) {
        for (int j = 0; j < l2; j++) {
            for (int k = 0; k < l3; k++) {
                for (int l = 0; l < l4; l++) {
                    free(c[i][j][k][l]);
                }
                free(c[i][j][k]);
            }
            free(c[i][j]);
        }
        free(c[i]);
    }
}

void file_in(int *k, char dna_sequences[5][130]) {
    // open file
    FILE *input_file;
    char buffer[16];

    if ((input_file = fopen(IN_FILE_NAME, "r")) == NULL) {
        fprintf(stderr, "file open error\n");
        exit(1);
    }

    // read input file
    fgets(buffer, sizeof(buffer), input_file);
    *k = atoi(buffer);
    fgets(buffer, sizeof(buffer), input_file);

    for (int i = 0; i < *k; i++) {
        fgets(dna_sequences[i], sizeof(dna_sequences[i]), input_file);
    }

    fclose(input_file);
}
void file_out(Stack *stack_array, int len) {
    // open file
    FILE *output_file;

    if ((output_file = fopen(OUT_FILE_NAME, "w")) == NULL) {
        fprintf(stderr, "file open error\n");
        exit(1);
    }

    // print stack to output file
    for (int i = 0; i < len; i++) {
        print_stack(&(stack_array[i]), output_file);
        free_stack(&(stack_array[i]));
    }

    fclose(output_file);
}
int max(int a, int b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}