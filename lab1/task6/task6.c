#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define EPS 1e-7

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2
} error;

error Str_to_double(const char *str, double *x);

double Integral_a(double x);

double Integral_b(double x);

double Integral_c(double x);

double Integral_d(double x);

double Integration(double eps, double (*integration_func)(double));

double Rectangle_method(int num_of_parts, double eps, double (*integration_func)(double));

int main(int argc, char *argv[]){
    double eps, a, b, c, d;

    if(argc != 2){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if(Str_to_double(argv[1], &eps) != OK){
        printf("Incorrect epsilon input.\n");
        return INPUT_ERROR;
    }

    if(eps <= EPS || eps >= 1){
        printf("Incorrect epsilon input.\n");
        return INPUT_ERROR;
    }

    a = Integration(eps, Integral_a);
    b = Integration(eps, Integral_b);
    c = Integration(eps, Integral_c);
    d = Integration(eps, Integral_d);

    printf("Answer for integral a: %lf\n", a);
    printf("Answer for integral b: %lf\n", b);
    printf("Answer for integral c: %lf\n", c);
    printf("Answer for integral d: %lf\n", d);

    return OK;

}

error Str_to_double(const char *str, double *x){
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

double Integral_a(double x){
    return log(1 + x);
}

double Integral_b(double x){
    return exp(-x * x / 2);
}

double Integral_c(double x){
    return log(1 / (1 - x));
}

double Integral_d(double x){
    return pow(x, x);
}

double Integration(double eps, double (*integration_func)(double)){
    double previous_res = 1.0, res = 0.0;
    int num_of_parts = 1; // колво разбиений интервала интегрирования

    while(fabs(res - previous_res) >= eps){
        previous_res = res;
        res = Rectangle_method(num_of_parts, eps, integration_func);
        num_of_parts *= 2; // если не добились достаточно точного результата
    }
    return res;
}

double Rectangle_method(int num_of_parts, double eps, double (*integration_func)(double)){
    double h, sum = 0.0, x, i;
    h = 1.0 / num_of_parts; // ширина 1 прямоугольника (интервала на которые разбиваем)

    for(i = eps; i < num_of_parts - eps; ++i){
        x = h * i; // вычисляем текущую точку интервала
        sum += integration_func(x);
    }
    return h * sum;
}