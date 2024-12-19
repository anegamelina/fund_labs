#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2
} func;

func Str_to_int(char* str, long long int* num, int base);

void Search_elements(long long int array[], int len);

int main(int argc, char *argv[]) {
    long long int a, b;
    if(argc != 3){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if(Str_to_int(argv[1], &a, 10) != OK ||
       Str_to_int(argv[2], &b, 10) != OK ){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if (a >= b){
        printf("a must be less than b.\n");
        return INPUT_ERROR;
    }

    long long int array[10];

    srand(time(NULL)); // инициализация генератора случайных значений

    for(int i = 0; i < 10; ++i){
        array[i] = a + rand() % (b - a + 1);
    }

    for(int i = 0; i < 10; ++i){ // изначальный массив
        printf("%lld ", array[i]);
    }
    printf("\n");

    Search_elements(array, 10);

    for(int i = 0; i < 10; ++i){ // полученный новый массив
        printf("%lld ", array[i]);
    }
    printf("\n");

    return OK;
}

func Str_to_int(char* str, long long int* num, int base){
    char* endptr;
    long long int answ;
    answ = strtoll(str, &endptr, base);

    if(answ >= LLONG_MAX || answ <= LLONG_MIN)
        return OVERFLOW_ERROR;
    if(*endptr != '\0')
        return INPUT_ERROR;

    *num = answ;
    return OK;
}

void Search_elements(long long int array[], int len){

    long long int max_elem = LLONG_MIN, min_elem = LLONG_MAX;
    int ind_max = -1, ind_min = -1;

    for(int i = 0; i < len; ++i){
        if(array[i] > max_elem){
            max_elem = array[i];
            ind_max = i;
        }
        if(array[i] < min_elem){
            min_elem = array[i];
            ind_min = i;
        }
    }
    if(ind_max != -1 && ind_min != -1){ // меняем местами мин и макс
        long long int n;
        n = array[ind_min];
        array[ind_min] = array[ind_max];
        array[ind_max] = n;
    }
}