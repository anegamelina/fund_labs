#include <stdio.h>
#include <math.h>
#define EPS 1e-7

typedef enum errors{
    OK = 1,
    EPS_ERROR = -1,
    DICHOTOMY_ERROR = -2
} error;

double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);
error Dichotomy(double a, double b, double epsilon, double (*function)(double), double *answ);

int main() {
    double answer, a, b, epsilon;
    error res;
    answer = 0.0, a = 1, b = 3, epsilon = 0.05;
    res = Dichotomy(a, b, epsilon, f1, &answer);

    if(res == OK)
        printf("Answer: %f\n", answer);
    else if(res == EPS_ERROR)
        printf("Incorrect epsilon input.\n");
    else
        printf("Function doesn't change sign on the interval.\n");

    answer = 0.0, a = 0, b = 1, epsilon = 0.004;
    res = Dichotomy(a, b, epsilon, f2, &answer);

    if(res == OK)
        printf("Answer: %f\n", answer);
    else if(res == EPS_ERROR)
        printf("Incorrect epsilon input.\n");
    else
        printf("Function doesn't change sign on the interval.\n");

    answer = 0.0, a = -1, b = 1, epsilon = 1e-6;
    res = Dichotomy(a, b, epsilon, f3, &answer);

    if(res == OK)
        printf("Answer: %f\n", answer);
    else if(res == EPS_ERROR)
        printf("Incorrect epsilon input.\n");
    else
        printf("Function doesn't change sign on the interval.\n");

    answer = 0.0, a = 1, b = 3, epsilon = -1;
    res = Dichotomy(a, b, epsilon, f1, &answer);

    if(res == OK)
        printf("Answer: %f\n", answer);
    else if(res == EPS_ERROR)
        printf("Incorrect epsilon input.\n");
    else
        printf("Function doesn't change sign on the interval.\n");

    answer = 0.0, a = 5, b = 8, epsilon = 0.0001;
    res = Dichotomy(a, b, epsilon, f4, &answer);

    if(res == OK)
        printf("Answer: %f\n", answer);
    else if(res == EPS_ERROR)
        printf("Incorrect epsilon input.\n");
    else
        printf("Function doesn't change sign on the interval.\n");

    return OK;
}

error Dichotomy(double a, double b, double epsilon, double (*function)(double), double *answ){
    double c; // середина интервала

    if(epsilon <= EPS)
        return EPS_ERROR;

    if(fabs(function(a)) <= epsilon){
        *answ = a;
        return OK;
    }
    if(fabs(function(b)) <= epsilon){
        *answ = b;
        return OK;
    }
    if(function(a) * function(b) > 0) // функция не меняет знак
        return DICHOTOMY_ERROR;

    c = (a + b) / 2;
    while(fabs(function(c)) >= epsilon){
        if(function(a) * function(b) > 0)
            return DICHOTOMY_ERROR;

        c = (a + b) / 2;
        if(function(a) * function(c) < 0) // корень находится в интервале [a, c]
            b = c;
        else // корень в интервале [c, b]
            a = c;

    }
    *answ = c;
    return OK;

}

double f1(double x){
    return x * x - 4;
}

double f2(double x){
    return x - 1;
}

double f3(double x){
    return x * x;
}

double f4(double x){
    return sin(x);
}
