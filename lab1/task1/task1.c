#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

void Multiples_nums(long long int x);

int Is_simple(long long int x);

void Hex_system(long long int x);

void Degree_table(long long int x);

long long int Sum(long long int x);

long long int Factorial(long long int x);

long long int Str_to_int(const char *str, long long int *x, int base);

double Pow(double num, int degree);


int main(int argc, char *argv[]){

    long long int x, fact_x, sum_x;
    if (argc != 3) {
        printf("Incorrect input. Try again\n");
        return -1;
    }

    if (Str_to_int(argv[1], &x, 10) == -1){
        printf("Incorrect input. Try again\n");
        return -1;
    }

    if(x <= 0){
        printf("Incorrect input. The number must be positive\n");
        return -1;
    }

    if ((argv[2][0] == '-' || argv[2][0] == '/') == 0) {
        printf("Incorrect flag input. Try again\n");
        return -2;
    }

    if (argv[2][2] != '\0'){
        printf("Incorrect flag input. Try again\n");
        return -2;
    }

    switch(argv[2][1]){
        case 'h':
            Multiples_nums(x);
            break;
        case 'p':
            if (x == 1)
                printf("The number is neither simple nor composite\n");
            else if (Is_simple(x) > 0)
                printf("The number is simple\n");
            else
                printf("The number is composite\n");
            break;
        case 's':
            printf("Number in hex system: ");
            Hex_system(x);
            printf("\n");
            break;
        case 'e':
            if(x <= 10){
                printf("Degree table:\n");
                Degree_table(x);
            }
            else
                printf("The number must be less than or equal to 10\n");
            break;
        case 'a':
            sum_x = Sum(x);
            if (sum_x > 0)
                printf("The sum is %lld\n", sum_x);
            else
                printf("Overflow error\n");
            break;
        case 'f':
            fact_x = Factorial(x);
            if (fact_x > 0)
                printf("The factorial is %lld\n", fact_x);
            else
                printf("Overflow error\n");
            break;
        default:
            printf("There is no such flag\n");
            break;
    }

}




int Is_simple(long long int x){ // является ли число простым
    for (long long int i = 2; i <= (long long int)(sqrtl(x)); ++i){
        if(x % i == 0)
            return 0;
    }
    return 1;
}

long long int Factorial(long long int x){ // факториал числа
    if (x == 1)
        return 1;
    long long int fac = 1;
    for (long long int i = 2; i <= x; ++i){
        if ((unsigned long long int)(fac * i) > LLONG_MAX){
            return -3;
        }
        fac *= i;
    }
    return fac;
}

long long int Sum(long long int x){ // сумма чисел от 1 до x включительно
    long long int sum;
    sum = (1 + x) / 2 * x;
    if((unsigned long long int)(sum) > LLONG_MAX) {
        return -3;
    }
    return sum;
}

void Multiples_nums(long long int x){ // вывести числа, кратные x
    if (x > 100)
        printf("There are no multiples of x\n");

    else {
        printf("Numbers that are multiples of x: ");
        for (long long int i = x; i <= 100; i += x) {
            printf("%lld ", i);
        }
        printf("\n");
    }
}

void Degree_table(long long int x){ // вывести таблицу степеней
    for (int i = 1; i <= 10; ++i) {
        for (int j = 1; j <= x; ++j) {
            printf("%12lld  ", (long long int)Pow(i, j));
        }
        printf("\n");
    }
}


double Pow(double num, int degree){
    if (degree < 0){
        return Pow(1 / num, -degree);
    }
    if (degree == 0){
        return 1;
    }
    if(degree % 2 == 0){
        return Pow(num * num, degree / 2);
    }
    else {
        return num * Pow(num * num, (degree - 1) / 2);
    }
}

void Hex_system(long long int x){ // разделить число на отдельные цифры СС 16 и вывести их

    char str[100];
    sprintf(str, "%llx", x);
    for(int i = 0; i < strlen(str); ++i){
        printf("%c ", str[i]);
    }
}

long long int Str_to_int(const char *str, long long int *x, int base){
    char *endptr;
    long long int number = strtoll(str, &endptr, base); // endptr - указатель на символ, где остановилось преобразование

    if(number >= LLONG_MAX || number <= LLONG_MIN){
        return -1;
    }
    if(*endptr != '\0'){
        return -1;
    }
    *x = number;
    return 1;
}