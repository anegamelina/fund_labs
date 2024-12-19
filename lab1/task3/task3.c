#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>


int Str_to_double(const char *str, long double *x);

int Str_to_int(const char *str, long long int *x, int base);

int Sides_of_triangle(long double side_1, long double side_2, long double side_3, long double eps);

int Is_quadratic_equation(long double a, long double b, long double c, long double eps, long double *x1, long double *x2);

void Answers_output(long double a, long double b, long double c, long double eps, long double x1, long double x2, int *counter);

void All_answers(long double a, long double b, long double c, long double eps, long double x1, long double x2);

int main(int argc, char *argv[]) {
    long long int num_1, num_2;
    long double eps, a, b, c;

    if((argv[1][0] == '-' || argv[1][0] == '/') == 0){
        printf("Incorrect flag input. Try again.\n");
        return -2;
    }
    if(argv[1][2] != '\0'){
        printf("Incorrect flag input. Try again.\n");
        return -2;
    }

    switch(argv[1][1]) {
        case 'q':

            if(argc != 6) {
                printf("Incorrect input. Try again.\n");
                return -1;
            }
            if(! ((Str_to_double(argv[2], &eps) == 1)
                  && (Str_to_double(argv[3], &a) == 1)
                  && (Str_to_double(argv[4], &b) == 1)
                  && (Str_to_double(argv[5], &c) == 1))){
                printf("Incorrect input. Try again.\n");
                return -1;
            }
            if(eps < 0){
                printf("Incorrect input. Epsilon must be positive\n");
                return -1;
            }

            long double x1 = 0, x2 = 0;
            int counter = 0;

            All_answers(a, b, c, eps, x1, x2);


            break;

        case 'm':

            if(argc != 4){
                printf("Incorrect input. Try again.\n");
                return -1;
            }
            if(! ((Str_to_int(argv[2], &num_1, 10) == 1)
                  && (Str_to_int(argv[3], &num_2, 10) == 1))){
                printf("Incorrect input. Try again.\n");
                return -1;
            }

            if(num_1 == 0 || num_2 == 0) {
                printf("Numbers must be non-zero\n");
                return -1;
            }

            if (num_1 % num_2 == 0)
                printf("The first number is a multiple of the second\n");
            else
                printf("The first number is not a multiple of the second\n");
            break;

        case 't':
            if(argc != 6){
                printf("Incorrect input. Try again.\n");
                return -1;
            }
            if(! ((Str_to_double(argv[2], &eps) == 1)
                  && (Str_to_double(argv[3], &a) == 1)
                  && (Str_to_double(argv[4], &b) == 1)
                  && (Str_to_double(argv[5], &c) == 1))){
                printf("Incorrect input. Try again.\n");
                return -1;
            }
            if(a <= 0 || b <= 0 || c <= 0){
                printf("Incorrect input. Numbers must be greater than 0\n");
                return -1;
            }
            if(eps < 0){
                printf("Incorrect input. Epsilon must be positive\n");
                return -1;
            }

            int res = Sides_of_triangle(a, b, c, eps);
            if(res == 1)
                printf("The numbers are the sides of a right triangle\n");
            else if(res == -3) {
                printf("Overflow error\n");
                return -3;
            }
            else
                printf("The numbers are not the sides of a right triangle\n");
            break;

        default:
            printf("There is no such flag\n");
            return -2;
    }

}



int Str_to_int(const char *str, long long int *x, int base){
    char *endptr;
    long long int number = strtoll(str, &endptr, base);

    if(number >= LLONG_MAX || number <= LLONG_MIN){
        return -3;
    }
    if(*endptr != '\0'){
        return -1;
    }
    *x = number;
    return 1;

}

int Str_to_double(const char *str, long double *x){
    char *endptr;

    int count_point = 0;
    for(int i = 0; i < strlen(str); ++i){
        if((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '-'){
            return -1;
        }
        else if(str[i] == '.' && count_point == 0)
            ++count_point;
    }

    if(count_point > 1)
        return -1;

    long double number = strtold(str, &endptr);

    if(number >= HUGE_VALL || number <= -HUGE_VALL)
        return -3;

    if(*endptr != '\0')
        return -1;

    *x = number;
    return 1;

}

int Sides_of_triangle(long double side_1, long double side_2, long double side_3, long double eps){
    if (side_1 * side_1 >= HUGE_VALL || side_2 * side_2 >= HUGE_VALL || side_3 * side_3 >= HUGE_VALL)
        return -3;
    if(fabsl(side_2 * side_2 + side_3 * side_3 - side_1 * side_1) <= eps){ // cторона 1
        return 1;
    }
    else if(fabsl(side_1 * side_1 + side_3 * side_3 - side_2 * side_2) <= eps){ // cторона 2
        return 1;
    }
    else if(fabsl(side_1 * side_1 + side_2 * side_2 - side_3 * side_3) <= eps){ // cторона 3
        return 1;
    }
    else
        return 0;
}

int Is_quadratic_equation(long double a, long double b, long double c, long double eps, long double *x1, long double *x2) {
    long double discriminant;
    discriminant = b * b - 4 * a * c;
    if (discriminant >= HUGE_VALL || discriminant <= -HUGE_VALL)
        return -3;

    if(a <= eps && b <= eps)
        return 3;

    if (discriminant < 0)
        return 0;

    else if (discriminant <= eps) {
        *x1 = (-b + sqrtl(discriminant)) / (2 * a);
        *x2 = *x1;
        return 1;
    } else {
        *x1 = (-b + sqrtl(discriminant)) / (2 * a);
        *x2 = (-b - sqrtl(discriminant)) / (2 * a);
        return 2;

    }
}

void Answers_output(long double a, long double b, long double c, long double eps, long double x1, long double x2, int *counter){
    int eq = Is_quadratic_equation(a, b, c, eps, &x1, &x2);

    if(eq == 2 && a > eps){
        printf("x1 = %Lf, x2 = %Lf\n", x1, x2);
        *counter = 1;
    }
    else if(eq == 1 && a > eps){
        printf("x = %Lf\n", x1);
        *counter = 1;
    }
    else if(eq == 3)
        *counter = -1;
}

void All_answers(long double a, long double b, long double c, long double eps, long double x1, long double x2){
    int counter = 0;
    if(fabsl(a - b) > eps && fabsl(a - c) > eps && fabsl(b - c) > eps){
        Answers_output(a, b, c, eps, x1, x2, &counter);
        Answers_output(a, c, b, eps, x1, x2, &counter);
        Answers_output(b, a, c, eps, x1, x2, &counter);
        Answers_output(b, c, a, eps, x1, x2, &counter);
        Answers_output(c, a, b, eps, x1, x2, &counter);
        Answers_output(c, b, a, eps, x1, x2, &counter);
    }
    else if(fabsl(a - b) <= eps && fabsl(a - c) <= eps && fabsl(b - c) <= eps){
        Answers_output(a, b, c, eps, x1, x2, &counter);
    }
    else if(fabsl(a - b) <= eps){
        Answers_output(a, b, c, eps, x1, x2, &counter);
        Answers_output(a, c, b, eps, x1, x2, &counter);
        Answers_output(c, a, b, eps, x1, x2, &counter);
    }
    else if(fabsl(b - c) <= eps){
        Answers_output(a, b, c, eps, x1, x2, &counter);
        Answers_output(b, c, a, eps, x1, x2, &counter);
        Answers_output(c, a, b, eps, x1, x2, &counter);
    }
    else if(fabsl(a - c) <= eps){
        Answers_output(a, b, c, eps, x1, x2, &counter);
        Answers_output(a, c, b, eps, x1, x2, &counter);
        Answers_output(b, a, c, eps, x1, x2, &counter);
    }

    if(counter == 0){
        printf("No solutions\n");
    }
    else if(counter == -1)
        printf("x = infinity\n");
}