#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    MEMORY_ERROR = -2
} error;

error Redecomposition(double *array_answ, double eps, int degree, double point, ...);

double Calc_equation(const double *coefs, int degree, double point);

double Calc_df(double *coefs, int degree, double point);

int main(){
    int degree = 4, i, x;
    double eps = 1e-6, a = 3.0, g_x;
    double *answers = (double *) malloc((degree + 1) * sizeof(double));
    error check;

    if(answers == NULL){
        printf("Problems with malloc.\n");
        free(answers);
        return MEMORY_ERROR;
    }

    check = Redecomposition(answers, eps, degree, a, -2.0, 1.0, -3.0, 0.0, 1.0);

    if (check == MEMORY_ERROR){
        printf("Problems with malloc.\n");
        free(answers);
        return MEMORY_ERROR;
    }
    else if(check == INPUT_ERROR){
        printf("Incorrect input.\n");
        free(answers);
        return INPUT_ERROR;
    }
    else{
        printf("Answer f(x - a):\ngi = f^(i)(a) / i!\n");
        for(i = 0; i <= degree; ++i)
            printf("g%d = %lf\n", i, answers[i]);
    }

    printf("\n");
    for(x = 0; x < degree + 1; x++){
        double f_x = -2.0 + 1.0 * x - 3.0 * x * x + 0.0 * x * x * x + 1.0 * x * x * x * x;
        printf("f(%d) = %lf\n", x, f_x);
    }
    printf("\n");

    for(x = 0; x < degree + 1; x++){
        g_x = 0.0;
        for(i = 0; i < degree + 1; i++)
            g_x += answers[i] * pow(x - 3.0, i);
        printf("g(%d) = %lf\n", x, g_x);
    }

    free(answers);
    return OK;
}

error Redecomposition(double *array_answ, double eps, int degree, double point, ...){
    double elem, factorial = 1.0;
    double *coefs;
    int i, k;

    if(degree < 0 || eps < 1e-10)
        return INPUT_ERROR;

    va_list args;
    va_start(args, point);

    coefs = (double *) malloc((degree + 1) * sizeof(double)); // для хранения коэффициентов многочлена

    if(coefs == NULL) {
        free(coefs);
        return MEMORY_ERROR;
    }
    for(k = 0; k < degree + 1; ++k){ // коэффициенты многочлена из переменных аргументов записываются в массив coefs
        elem = va_arg(args, double);
        coefs[k] = elem;
    }

    array_answ[0] = Calc_equation(coefs, degree, point); // считается первый коэффициент при (x - a)^0

    for(i = 1; i < degree + 1; ++i){
        array_answ[i] = Calc_df(coefs, degree + 1 - i, point) / factorial;
        factorial *= (i + 1);
    }

    free(coefs);
    va_end(args);

    return OK;
}

double Calc_equation(const double *coefs, int degree, double point){
    double answer = 0.0, multi = 1.0;
    int i;
    for(i = 0; i < degree + 1; ++i){
        answer += coefs[i] * multi; // коэффициент * (point ^ текущая степень)
        multi *= point; // увеличиваем степень point
    }
    return answer;
}

double Calc_df(double *coefs, int degree, double point){ // производная и ее значение в точке point
    int i;
    for(i = 0; i < degree; ++i)
        coefs[i] = coefs[i + 1] * (i + 1);
    return Calc_equation(coefs, degree - 1, point);
}