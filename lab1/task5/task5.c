#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2,
    EPS_ERROR = -3
} error;

error Str_to_double(char *str, double *x);

double Sum_A(double x, double epsilon);

double Sum_B(double x, double epsilon);

double Sum_C(double x, double epsilon);

double Sum_D(double x, double epsilon);

error Check_overflow(double x);

int main(int argc, char *argv[]){
    double x, epsilon;

    if(argc != 3){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if(Str_to_double(argv[1], &x) == INPUT_ERROR){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if(Str_to_double(argv[1], &x) == OVERFLOW_ERROR){
        printf("Overflow error. Try again.\n");
        return OVERFLOW_ERROR;
    }

    if(Str_to_double(argv[2], &epsilon) != OK || epsilon <= 0.0 || epsilon > 1.0){
        printf("Incorrect epsilon input.\n");
        return EPS_ERROR;
    }
    if (Sum_A(x, epsilon) != epsilon)
        printf("Answer for A: %lf\n", Sum_A(x, epsilon));
    else
        printf("Incorrect input for function A\n");
    if (Sum_B(x, epsilon) != epsilon)
        printf("Answer for B: %lf\n", Sum_B(x, epsilon));
    else
        printf("Incorrect input for function B\n");
    if (Sum_C(x, epsilon) != epsilon && Sum_C(x, epsilon) != INFINITY)
        printf("Answer for C: %lf\n", Sum_C(x, epsilon));
    else
        printf("Incorrect input for function C\n");
    if (Sum_D(x, epsilon) != epsilon && Sum_D(x, epsilon) != INFINITY)
        printf("Answer for D: %lf\n", Sum_D(x, epsilon));
    else
        printf("Incorrect input for function D\n");

    return OK;
}

error Str_to_double(char *str, double *x){
    char *endptr;
    int count_point = 0, i;
    double number;

    for(i = 0; i < strlen(str); ++i){
        if((str[i] < '0' || str[i] > '9') && str[i] != '.'){
            return INPUT_ERROR;
        }
        else if(str[i] == '.' && count_point == 0)
            ++count_point;
    }

    if(count_point != 1)
        return INPUT_ERROR;

    number = strtod(str, &endptr);

    if(number >= HUGE_VAL || number <= -HUGE_VAL)
        return OVERFLOW_ERROR;

    if(*endptr != '\0')
        return INPUT_ERROR;

    *x = number;
    return OK;

}

double Sum_A(double x, double epsilon){ // эквивалентно ряду тейлора e^x
    int n = 0; // номер текущего члена ряда
    double iter_answer = 1.0, res = 0.0; // iter_answer - текущий член ряда
    while(fabs(iter_answer) >= epsilon){ // проверяем что текущее значение >= epsilon
        if(n == INT_MAX) // чтобы цикл в бесконечность не ушел
            return epsilon;
        res += iter_answer; // прибавляем текущий член ряда к общей сумме
        ++n; // следующий член ряда
        iter_answer *= x / n; // след член ряда по формуле вычисляем
    }
    return res;
}

double Sum_B(double x, double epsilon){
    int n = 0;
    double iter_answer = 1.0, res = 0.0;
    while(fabs(iter_answer) >= epsilon){
        if(n == INT_MAX)
            return epsilon;
        res += iter_answer;
        ++n;
        iter_answer *= -1.0 * x * x / ((2 * n) * (2 * n - 1.0));
    }
    return res;
}

double Sum_C(double x, double epsilon){
    int n = 0;
    double iter_answer = 1.0, res = 0.0;
    while(fabs(iter_answer) >= epsilon){
        if(n == INT_MAX)
            return epsilon;
        res += iter_answer;
        ++n;
        iter_answer *= 9.0 * x * x * n * n / (3.0 * n * (3.0 * n - 3.0) + 2.0);
        if (epsilon + iter_answer >= 1.0) // если будет >=1 то сумма уйдет в бесконечность
            return INFINITY;
    }
    return res;
}


double Sum_D(double x, double epsilon){
    int n = 0;
    if(fabs(x) >= 1.0) // значение суммы зависит от х
        return INFINITY;
    double iter_answer = -1.0 * x * x / 2.0, res = 0.0; // начинается с n = 1 поэтому первый член такой

    while(fabs(iter_answer) >= epsilon){
        if(Check_overflow(res) == OVERFLOW_ERROR || Check_overflow(iter_answer) == OVERFLOW_ERROR){ // проверка на переполнение
            return INFINITY;
        }
        if(n == INT_MAX)
            return epsilon;
        res += iter_answer;
        ++n;
        iter_answer *= (-1.0 * x * x * (2.0 * n - 1.0)) / (2.0 * n);
    }
    return res;
}

error Check_overflow(double x){
    if(isinf(x) || fabs(x) > DBL_MAX)
        return OVERFLOW_ERROR;
    return OK;
}