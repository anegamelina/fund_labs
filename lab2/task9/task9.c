#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#define EPS 1e-7

typedef enum errors{
    OK = 1,
    OVERFLOW_ERROR = -1,
    INPUT_ERROR = -2
} error;

error Function(int *answ, int *len, int base, int count, ...);

error Overflow_check(double num);

error Turn_into_fraction(double num, long long int *numerator, long long int *denominator);

void NOD(long long int n1, long long int n2, long long int *answer);

int If_can_be_represented(int base, long long int denominator);

int main() {
    int answ[100];
    int len = 0;
    error check;

    check = Function( answ, &len, 10, 1, 0.123);
    if(check == OVERFLOW_ERROR){
        printf("Overflow error. Try again.\n");
    }
    else if(check == INPUT_ERROR){
        printf("Incorrect input. Try again.\n");
    }
    else{
        for(int i = 0; i < len; ++i){
            if(answ[i] == 1)
                printf("%d. Number can be finally represented\n", i + 1);
            else
                printf("%d. Number can't be finally represented\n", i + 1);
        }
    }

    return OK;
}

error Function(int *answ, int *len, int base, int count, ...){
    long long int numerator, denominator;

    if (base < 2 || base > 36)
        return INPUT_ERROR;
    if(count == 0)
        return INPUT_ERROR;

    va_list args;
    va_start(args, count);

    for(int i = 0; i < count; ++i){
        double num = va_arg(args, double);

        if(Overflow_check(num) == OVERFLOW_ERROR){
            va_end(args);
            return OVERFLOW_ERROR;
        }

        if(num <= 0 || num >= 1){
            va_end(args);
            return INPUT_ERROR;
        }
        denominator = 1;

        if(Turn_into_fraction(num, &numerator, &denominator) == OVERFLOW_ERROR) { // преообразование в дробь
            va_end(args);
            return OVERFLOW_ERROR;
        }
        if(If_can_be_represented(base, denominator) == 1)
            answ[i] = 1;
        else
            answ[i] = 0;
        ++(*len);
    }
    va_end(args);
    return OK;
}

error Overflow_check(double num){
    if(isnan(num) || isinf(num))
        return OVERFLOW_ERROR;
    return OK;
}

error Turn_into_fraction(double num, long long int *numerator, long long int *denominator){
    long long int x;

    while(fabs(num - floor(num)) >= EPS){
        if(*numerator > LLONG_MAX || *denominator > LLONG_MAX)
            return OVERFLOW_ERROR;
        num *= 10;
        *denominator *= 10;
    }

    *numerator =(long long int) num;

    NOD(*numerator, *denominator, &x); // на что надо поделить чтобы сократить дробь

    *numerator /= x; // сокращение
    *denominator /= x;

    return OK;
}

void NOD(long long int n1, long long int n2, long long int *answer){
    while(n1 > 0 && n2 > 0){
        if(n1 > n2)
            n1 = n1 % n2;
        else
            n2 = n2 % n1;
    }
    if(n1 != 0)
        *answer = n1;
    else
        *answer = n2;
}

int If_can_be_represented(int base, long long int denominator){
    for(int i = 2; i <= base; ++i){
        if(i % 2 == 0 && i != 2) // пропускает четные числа, кроме 2, так как они не являются простыми
            continue;

        while (base % i == 0 && denominator % i == 0) {
            while (base % i == 0)
                base /= i;
            while(denominator % i == 0)
                denominator /= i;

            if(denominator == 1)
                return 1;
        }
    }
    if(base == 1 && denominator == 1)
        return 1;
    else
        return 0;
}