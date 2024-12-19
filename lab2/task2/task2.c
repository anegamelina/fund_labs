#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
#define EPS 1e-7

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2
} func;

func Geometric_mean(double *mean, int n, ...);

func Fast_pow(double num, int degree, double *answer);

func Overflow_check(const double *num);

func Denormalize_check(const double *num);


int main() {

    double mean = 1.0;

    if (Geometric_mean(&mean, 3, 1.0, 2.0, 3.0) == OK)
        printf("Geometric mean: %f\n", mean);
    else{
        printf("Overflow error.\n");
        return OVERFLOW_ERROR;
    }
    mean = 1.0;

    double number = 0.04;
    int degree = 2;
    double fastpow_answ;
    if(Fast_pow(number, degree, &fastpow_answ) == OK)
        printf("Fast Pow answer is: %f\n", fastpow_answ);
    else{
        printf("Overflow error.\n");
        return OVERFLOW_ERROR;
    }

    return OK;
}


func Geometric_mean(double *mean, int n, ...){

    if (n == 0)
        return INPUT_ERROR;

    va_list numbers;
    va_start(numbers, n);

    for(int i = 0; i < n; ++i){
        double num = va_arg(numbers, double);
        if(num <= EPS){
            va_end(numbers);
            return INPUT_ERROR;
        }
        if(Overflow_check(mean) != OK || Denormalize_check(mean) != OK){
            va_end(numbers);
            return OVERFLOW_ERROR;
        }

        *mean *= num;
    }

    va_end(numbers);

    *mean = pow(*mean, (double) (1.0 / n));
    return OK;

}


func Fast_pow(double num, int degree, double *answer){
    func check = OK;

    if(degree < 0)
        return Fast_pow(1 / num, -degree, answer);

    if(degree == 0) {
        *answer = 1.0;
        return OK;
    }

    if((degree & 1) == 0) {
        check = Fast_pow(num, degree / 2, answer);
        *answer *= *answer;
        if(Overflow_check(answer) != OK || Denormalize_check(answer) != OK)
            return OVERFLOW_ERROR;
    }
    else{
        check = Fast_pow(num, degree - 1, answer);
        *answer *= num;
        if(Overflow_check(answer) != OK || Denormalize_check(answer) != OK)
            return OVERFLOW_ERROR;
    }
    return check;
}

func Overflow_check(const double *num){
    if(isnan(*num) || isinf(*num) || *num == 0.0 || Denormalize_check(num) != OK) // проверка на переполнение вниз
        return OVERFLOW_ERROR;
    return OK;
}

func Denormalize_check(const double *num){ // проверка на переполнение вниз
    if(fabs(*num) < EPS && fabs(*num) < DBL_MIN)
        return OVERFLOW_ERROR;
    else
        return OK;
}