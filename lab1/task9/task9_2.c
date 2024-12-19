#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum errors{
    OK = 1,
    MALLOC_ERROR = -2,
}func;

void Create_random_array(int array[], int size);
int Compare(const void *num1, const void *num2);
void Print_array(int array[], int size);
void Closest_elem(int a, int size, int array[], int *res);

int main() {
    int a_size, b_size, res;
    srand(time(NULL));

    // a_size = 10 + rand() % (10000 - 10 + 1); от 10 до 10000
    // b_size = 10 + rand() % (10000 - 10 + 1);

    a_size = 10 + rand() % 10; // от 10 до 19 значения генерятся для наглядности
    b_size = 10 + rand() % 10;

    int *A = (int *) malloc(a_size * sizeof(int));
    int *B = (int *) malloc(b_size * sizeof(int));

    if(A == NULL || B == NULL){
        if(A != NULL)
            free(A);
        if(B != NULL)
            free(B);
        printf("Problem with memory. Try again.\n");
        return MALLOC_ERROR;
    }

    Create_random_array(A, a_size);
    Create_random_array(B, b_size);

    qsort(B, b_size, sizeof(int), Compare); // быстрая сортировка

    int *C = (int *) malloc(a_size * sizeof(int));
    if(C == NULL){
        free(A);
        free(B);
        printf("Problem with memory. Try again.\n");
        return MALLOC_ERROR;
    }

    printf("A: ");
    Print_array(A, a_size);
    printf("B: ");
    Print_array(B, b_size);

    for(int i = 0; i < a_size; ++i) {
        Closest_elem(A[i], b_size, B, &res);
        C[i] = A[i] + res;
    }

    printf("C: ");
    Print_array(C, a_size);

    free(A);
    free(B);
    free(C);

    return OK;
}

void Create_random_array(int array[], int size){
    for (int i = 0; i < size; ++i)
        array[i] = -1000 + rand() % (1000 - (-1000) - 1);
}

int Compare(const void *num1, const void *num2){ // отриц - num1 < num2, ноль - num1 == num2, положит - num1 > num2
    return (*(int *)num1 - *(int *)num2);
}

void Print_array(int array[], int size){
    for(int i = 0; i < size; ++i)
        printf("%d ", array[i]);
    printf("\n");
}

void Closest_elem(int a, int size, int array[], int *res){ // бин поиск
    int left_border = 0, right_border = size - 1, middle, difference;

    while(left_border < right_border){
        middle = left_border + (right_border - left_border) / 2;
        difference = array[middle] - a; // а - искомое число

        if(difference == 0) {
            *res = array[middle];
            return;
        }
        else if(difference < 0)
            left_border = middle + 1;
        else
            right_border = middle - 1;
    }

    if(left_border == 0)
        *res = array[0];

    else{
        if(abs(array[left_border] - a) < abs(array[left_border - 1] - a))
            *res = array[left_border];
        else
            *res = array[left_border - 1];
    }
}