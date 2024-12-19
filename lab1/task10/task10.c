#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2,
} error;

error Str_to_int(const char *str, long long int *x, int base);

void Conversion_from_ten(long long num, char *res, int base);

error Num_to_let(long long int n, char *let);


int main() {

    long long int number = 0, max_number = 0;
    int number_system, check1, check2, check_count = 0;
    char str[65], ans[65];

    printf("Enter the number system, then numbers\n");
    check1 = scanf("%d", &number_system);
    if(check1 != 1 || number_system < 2 || number_system > 36){
        printf("Wrong number system.\n");
        return INPUT_ERROR;
    }

    scanf("%s", str);

    while(strcmp(str, "Stop") != 0) {
        check2 = Str_to_int(str, &number, number_system);
        if (check2 == INPUT_ERROR) {
            printf("Incorrect number input.\n");
            return INPUT_ERROR;
        }
        else if (check2 == OVERFLOW_ERROR) {
            printf("Overflow error.\n");
            return OVERFLOW_ERROR;
        }
        else {
            if (llabs(number) > llabs(max_number)) {
                max_number = number;
            }
            scanf("%s", str);
        }
        ++check_count; // колво введенных чисел
    }

    if(check_count == 0){
        printf("You haven't entered any numbers.\n");
        return INPUT_ERROR;
    }

    Conversion_from_ten(max_number, ans, number_system);
    printf("Maximum number(in %d CC): %s\n", number_system, ans);

    for(int i = 9; i <= 36; i = i + 9){
        Conversion_from_ten(max_number, ans, i);
        printf("Number in %d CC: %s\n", i, ans);
    }

    return OK;
}


error Str_to_int(const char *str, long long int *x, int base){
    char *endptr;
    int len = strlen(str);
    long long int number;
    for(int i = 0; i < len; ++i){
        if(str[i] >= 'a' && str[i] <= 'z')
            return INPUT_ERROR;
    }
    number = strtoll(str, &endptr, base);

    if(number >= LLONG_MAX || number <= LLONG_MIN){
        return OVERFLOW_ERROR;
    }
    if(*endptr != '\0'){
        return INPUT_ERROR;
    }

    *x = number;
    return OK;
}

void Conversion_from_ten(long long num, char *res, int base){
    int ind = 0, sign = 1;
    char temp[65], letter;
    if(num < 0){
        sign = -1;
        num = llabs(num);
    }
    if(num == 0){ // если число это 0
        res[0] = '0';
        res[1] = '\0';
    }
    else {
        while (num > 0) {
            Num_to_let(num % base, &letter);
            temp[ind] = letter;
            num /= base;
            ++ind;
        }
        int len = ind - 1;
        int start = 0;
        if(sign == -1){
            res[0] = '-';
            start += 1;
            ind += 1;
        }
        for (int i = start; i < ind; ++i) {
            res[i] = temp[len];
            --len;
        }
        res[ind] = '\0';
    }
}

error Num_to_let(long long int n, char *let){ // из числа в букву
    if (n >= 0 && n <= 9) {
        *let = n + '0';
    }
    else{
        *let = n - 10 + 'A';
    }
    return OK;
}
