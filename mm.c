#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define n 1000
#define n_loop 3
#define n_perm 6
#define PRINT_MAT 0 // Flag to print the result of the matrix multiplication at every iteration

double A[n][n];
double B[n][n];
double C[n][n];
int P[n_perm][n_loop]; // Matrix to store the possible permutations of i,j,k

double time_spent[n_perm];
int perm_idx;

// Function to print a matrix
void print_mat(double arr[n][n])
{
    printf("------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f \t", arr[i][j]);
            arr[i][j] = 0;
        }
        printf("\n");
    }
    printf("------------------------------------------\n");
}

// Function to print permutation
void print_perm(int arr[][n_loop], int idx)
{
    printf("#%d\t", idx);
    for (int i = 0; i < n_loop; i++)
    {
        printf("%d\t", arr[idx][i]);
    }
}

// Function to swap 2 variables
void swap(int arr[], int a, int b)
{
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

// Function to compute permutations
void permutation(int arr[], int start, int end)
{
    if (start == end)
    {
        // Store the permutation
        for (int i = 0; i < n_loop; i++)
        {
            P[perm_idx][i] = arr[i];
        }

        // Run experiment (matrix multiplication)
        struct timespec start, end;
        int loopArr[3] = {0, 0, 0};
        int i, j, k;
        clock_gettime(CLOCK_REALTIME, &start);
        for (loopArr[0] = 0; loopArr[0] < n; loopArr[0]++)
        {
            for (loopArr[1] = 0; loopArr[1] < n; loopArr[1]++)
            {
                for (loopArr[2] = 0; loopArr[2] < n; loopArr[2]++)
                {
                    i = loopArr[P[perm_idx][0]];
                    j = loopArr[P[perm_idx][1]];
                    k = loopArr[P[perm_idx][2]];
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        clock_gettime(CLOCK_REALTIME, &end);
        time_spent[perm_idx] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        print_perm(P, perm_idx);
        printf("%f\n", time_spent[perm_idx++]);
        if (PRINT_MAT == 1)
        {
            print_mat(C);
            printf("\n");
        }

        return;
    }
    for (int i = start; i < end; i++)
    {
        // Swap the first and ith digits
        swap(arr, i, start);
        // Fix the new first digit and call permutation on the rest of the digits
        permutation(arr, start + 1, end);
        // Swap the first and ith digits back to their original positions
        swap(arr, i, start);
    }
}

int main()
{
    perm_idx = 0;
    // Populate the matrices with random values between 0.0 and 1.0
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = (double)rand() / (double)RAND_MAX;
            B[i][j] = (double)rand() / (double)RAND_MAX;
            C[i][j] = 0;
        }
    }

    int permArr[3] = {0, 1, 2};

    printf("Perm\ti\tj\tk\tTime(s)\n------------------------------------------\n");
    permutation(permArr, 0, n_loop);

    return 0;
}