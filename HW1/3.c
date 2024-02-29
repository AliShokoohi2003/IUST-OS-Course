#include <stdio.h>
#include <stdlib.h>

void rotateImage(int n, int **arr) {
    int** temp = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        temp[i] = (int*)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = arr[n - j - 1][i];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arr[i][j] = temp[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        free(temp[i]);
    }
    free(temp);
}

int main() {
    int n;
    scanf("%d", &n);

    int** arr = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        arr[i] = (int*)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &arr[i][j]);
        }
    }
    if (arr[0][n-1] == 0)
    {
        rotateImage(n, arr);
        rotateImage(n, arr);
        rotateImage(n, arr);
    }
    if (arr[n-1][n-1] == 0)
    {
        rotateImage(n, arr);
        rotateImage(n, arr);
    }
    if (arr[n-1][0] == 0)
    {
        rotateImage(n, arr);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d", arr[i][j]);
            if (j < n - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }
    free(arr);

    return 0;
}
