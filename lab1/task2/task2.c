#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2,
    EPS_ERROR = -3
} error;

error Str_to_double(char *str, double *num);

double Limit_e(double epsilon);

double Limit_pi(double epsilon);

double Limit_ln2(double epsilon);

double Limit_sqrt2(double epsilon);

double Limit_gamma(double epsilon);

double Row_e(double epsilon);

double Row_pi(double epsilon);

double Row_ln2(double epsilon);

double Row_sqrt2(double epsilon);

double Row_gamma(double epsilon);

double Equation_e(double epsilon);

double Equation_pi(double epsilon);

double Equation_ln2(double epsilon);

double Equation_sqrt2(double epsilon);

double Equation_gamma(double epsilon);

int Is_simple (int number);


int main(int argc, char* argv[]){

    double eps;
    if (argc != 2){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

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

double Limit_e(double epsilon){
    int n = 1;
    double cur_iter = 0.0, prev_iter;
    do{
        prev_iter = cur_iter;
        n++;
        cur_iter = pow((1.0 + 1.0 / n), n);
    }while(fabs(prev_iter - cur_iter) >= epsilon);

    return cur_iter;
}

double Limit_pi(double epsilon){
    int n = 2;
    double cur_iter = 4.0, prev_iter; // типо сразу посчитали значение для n = 1
    do{
        prev_iter = cur_iter;
        cur_iter *= (4.0 * n * (n - 1.0)) / pow(2.0 * n - 1.0, 2);
        n++;
    }while(fabs(prev_iter - cur_iter) >= epsilon);

    return cur_iter;
}

double Limit_ln2(double epsilon){
    int n = 1;
    double cur_iter = 0.0, prev_iter;
    do{
        prev_iter = cur_iter;
        n *= 2; //для ускорения сходимости предела
        cur_iter = n * (pow(2.0, 1.0 / (double) n) - 1.0);
    }while(fabs(prev_iter - cur_iter) >= epsilon);

    return cur_iter;
}

double Limit_sqrt2(double epsilon){
    int n = 1;
    double cur_iter = -0.5, prev_iter;
    do{
        prev_iter = cur_iter;
        n++;
        cur_iter = prev_iter - prev_iter * prev_iter * 0.5 + 1.0;
    }while(fabs(prev_iter - cur_iter) >= epsilon);

    return cur_iter;
}

double Limit_gamma(double epsilon){
    int m = 1, k;
    double cur_iter = 1.0, prev_iter, sum = 1.0;
    do{
        prev_iter = cur_iter;
        m *= 2;
        for (k = 2; k <= m; ++k){
            sum += 1.0 / k;
        }
        cur_iter = sum - log(m);
        sum = 1.0;
    }while(fabs(prev_iter - cur_iter) >= epsilon);

    return cur_iter;
}

double Row_e(double epsilon){
    int n = 1;
    double sum = 1.0, prev_iter = 1.0;
    do{
        prev_iter /= (double) n;
        sum += prev_iter;
        n++;
    }while(prev_iter >= epsilon);

    return sum;
}


double Row_pi(double epsilon){
    int n = 1, sign = 1;
    double sum = 0.0, prev_iter;
    do{
        prev_iter = 4.0 * (double) sign / (2.0 * n - 1.0);
        sum += prev_iter;
        n++;
        sign *= -1;
    }while(fabs(prev_iter) >= epsilon);

    return sum;
}

double Row_ln2(double epsilon){
    int n = 1, sign = 1;
    double sum = 0.0, prev_iter;
    do{
        prev_iter = (double) sign / n;
        sum += prev_iter;
        n++;
        sign *= -1;
    }while(fabs(prev_iter) >= epsilon);

    return sum;
}

double Row_sqrt2(double epsilon){
    int k = 2;
    double sum = pow(2.0, 0.25), prev_iter;
    do{
        prev_iter = sum;
        ++k;
        sum *= pow(2, 1.0 / pow(2, k));
    }while(fabs(sum - prev_iter) >= epsilon);

    return sum;
}

double Row_gamma(double epsilon){
    int k = 2;
    double cur_iter = 0.5, prev_iter = 0.0, sqrt_num;
    do{
        prev_iter = cur_iter;
        ++k;
        sqrt_num = sqrt(k);
        if(fmod(sqrt_num, 1.0) == 0){ // проверка если k - полный квадрат
            k++;
            sqrt_num = sqrt(k);
        }
        cur_iter += 1.0 / pow((int) sqrt_num, 2.0) - 1.0 / k;
    }while(fabs(prev_iter - cur_iter) >= epsilon);
    return (cur_iter - pow(M_PI, 2) / 6);
}

double Equation_e(double epsilon){
    double x = 1.0, f = 1.0, df; // берем начальное приближение 1 (тк близко к е)
    while(fabs(f) > epsilon){
        f = log(x) - 1.0;
        df = 1.0 / x;
        x = x - f / df;
    }
    return x;
}

double Equation_pi(double epsilon){
    double x = 1.0, f = 1.0, df;
    while(fabs(f) > epsilon){
        f = cos(x) + 1.0;
        df = -sin(x);
        x = x - f / df;
    }
    return x;
}

double Equation_ln2(double epsilon){
    double x = 1.0, f = 1.0, df;
    while(fabs(f) > epsilon){
        f = exp(x) - 2.0;
        df = exp(x);
        x = x - f / df;
    }
    return x;
}

double Equation_sqrt2(double epsilon){
    double x = 1.0, f = 1.0, df;
    while(fabs(f) > epsilon){
        f = x * x - 2.0;
        df = 2.0 * x;
        x = x - f / df;
    }
    return x;
}

double Equation_gamma(double epsilon){
    double current_iteration = log(2) * 0.5, last_iteration, number = 0.5;
    int p = 2;

    do {
        last_iteration = current_iteration;
        do {
            ++p;
        } while (Is_simple(p)); // пока составное увеличиваем p
        number *= (p - 1.0) / p;
        current_iteration = log(p) * number;

    } while (fabs(last_iteration - current_iteration) >= epsilon);

    return (-log(current_iteration));
}

int Is_simple(int number) { // 1 если составное, 0 если простое
    if(number < 0)
        number *= -1;

    if (number == 0 || number == 1 || number == 2)
        return 0;

    if(number % 2 == 0)
        return 1;

    for (int i = 3; i <= floor(sqrt(number)); i += 2) {
        if (number % i == 0)
            return 1;
    }
    return 0;
}