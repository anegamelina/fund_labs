#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdlib.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    OVERFLOW_ERROR = -2,
    MEMORY_ERROR = -3
} error;

int Len_of_str(const char *str);

void Reverse_str(const char *str, char *reversed_str);

void Upper_odd_str(const char *str, char *upper_str);

void Num_letter_other(const char *str, char *ordered_str);

error Str_to_int(const char *str, unsigned int *num, int base);

error Concatenation(const char *str, char *random_str, int *len, size_t *buf);

error Random_concatenation(int num, char *argv[], size_t buf, char *random_str);

int main(int argc, char *argv[]) {
    int len;
    char *str, *str_last;
    unsigned int num;
    size_t bufsize = BUFSIZ;

    if(argv[1][0] != '-' || argv[1][2] != '\0'){
        printf("Incorrect flag input.\n");
        return INPUT_ERROR;
    }
    if(argv[1][1] == 'l' || argv[1][1] == 'r' || argv[1][1] == 'u' || argv[1][1] == 'n'){
        if(argc != 3){
            printf("The number of arguments is incorrect for this flag.\n");
            return INPUT_ERROR;
        }
    }
    else if(argv[1][1] == 'c'){
        if(argc < 4) {
            printf("The number of arguments is incorrect for this flag.\n");
            return INPUT_ERROR;
        }
    }
    else{
        printf("There is no such flag.\n");
        return INPUT_ERROR;
    }
    str = (char *) malloc((Len_of_str(argv[2]) + 1) * sizeof(char));
    str_last = (char *) malloc(bufsize * sizeof(char));

    if(str == NULL || str_last == NULL){
        printf("Problems with malloc.\n");
        if(str != NULL)
            free(str);
        if(str_last != NULL)
            free(str_last);
        return MEMORY_ERROR;
    }

    switch(argv[1][1]){
        case 'l':
            len = Len_of_str(argv[2]);
            printf("The length of the line is: %d\n", len);
            break;

        case 'r':
            Reverse_str(argv[2], str);
            printf("Reversed version of line is: %s\n", str);
            break;

        case 'u':
            Upper_odd_str(argv[2], str);
            printf("Line with upper symbols: %s\n", str);
            break;

        case 'n':
            Num_letter_other(argv[2], str);
            printf("An ordered line(numbers, letters, other): %s\n", str);
            break;

        case 'c':
            if (Str_to_int(argv[2], &num, 10) == INPUT_ERROR){
                printf("Incorrect number input. Try again.\n");
                free(str);
                free(str_last);
                return INPUT_ERROR;
            }
            else if(Str_to_int(argv[2], &num, 10) == OVERFLOW_ERROR){
                printf("Number overflow. Try again.\n");
                free(str);
                free(str_last);
                return OVERFLOW_ERROR;
            }

            srand(num);

            if(Random_concatenation(argc - 3, argv, bufsize, str_last) == MEMORY_ERROR){
                printf("Problems with memory.\n");
                free(str);
                free(str_last);
                return OVERFLOW_ERROR;
            }
            else{
                printf("Random concatenated line: %s\n", str_last);
            }
            break;

        default:
            printf("Incorrect flag input");
            free(str);
            free(str_last);
            return INPUT_ERROR;
    }

    free(str);
    free(str_last);
    return OK;
}

int Len_of_str(const char *str){
    int i;
    for(i = 0; *(str + i) != '\0'; ++i);
    return i;
}

void Reverse_str(const char *str, char *reversed_str){
    int len = Len_of_str(str);

    for(int i = 0; i < len; ++i){
        reversed_str[len - 1 - i] = str[i];
    }
    reversed_str[len] = '\0';
}

void Upper_odd_str(const char *str, char *upper_str){
    int len = Len_of_str(str);

    for(int i = 0; i < len; ++i){
        if((i & 1) != 0){
            if(str[i] >= 'a' && str[i] <= 'z')
                upper_str[i] = str[i] - 32;
            else
                upper_str[i] = str[i];
        }
        else
            upper_str[i] = str[i];
    }

    upper_str[len] = '\0';
}

void Num_letter_other(const char *str, char *ordered_str){
    int len = Len_of_str(str);
    int ind = 0;

    for(int i = 0; i < len; ++i){
        if(str[i] >= '0' && str[i] <= '9'){
            ordered_str[ind] = str[i];
            ++ind;
        }
    }

    for(int i = 0; i < len; ++i){
        if((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
            ordered_str[ind] = str[i];
            ++ind;
        }
    }

    for(int i = 0; i < len; ++i){
        if(!((str[i] >= 'a' && str[i] <= 'z') ||
             (str[i] >= 'A' && str[i] <= 'Z') ||
             (str[i] >= '0' && str[i] <= '9'))) {
            ordered_str[ind] = str[i];
            ++ind;
        }
    }
    ordered_str[len] = '\0';
}

error Str_to_int(const char *str, unsigned int *num, int base){
    char *endptr;
    unsigned int check_num = strtoul(str, &endptr, base);

    if(check_num >= ULONG_MAX)
        return OVERFLOW_ERROR;

    if(*endptr != '\0')
        return INPUT_ERROR;

    *num = check_num;
    return OK;
}

error Concatenation(const char *str, char *random_str, int *len, size_t *buf){
    int size;
    char *new;
    // str - добавляемая строка, random_str - строка результат конкатенации, len - длина строки random_str

    if(random_str == NULL)
        return MEMORY_ERROR;

    size = Len_of_str(str);

    if(size + (*len + 1) > *buf){
        *buf *= 2;
        new = (char *) realloc(random_str, *buf);
        if(new == NULL)
            return MEMORY_ERROR;
        random_str = new;
    }

    for(int i = 0; i < size; ++i){ // str добавляется к random_str
        random_str[*len] = str[i];
        ++(*len);
    }
    random_str[*len] = '\0';
    return OK;

}

error Random_concatenation(int num, char *argv[], size_t buf, char *random_str) { // num - колво аргументов в argv, argv[] - массив строк которые подает пользователь, random_str - строка для результата конкатенации
    int num_of_str = num, len_random_str = 0, random, count = 0; // len_random_str - для отслеживания текуцей длины строки, count - счетчик перебираемых строк
    int strings[num]; // для отслеживания того, какие строки мы уже брали

    while (count != num) { // пока не перебрали все строки
        random = 3 + rand() % ((num_of_str + 2)- 3 + 1); // выбираем рандом значение из диапазона [3, num_of_str + 2]

        if (strings[random - 3] != 1) {
            if (Concatenation(argv[random], random_str, &len_random_str, &buf) == MEMORY_ERROR)
                return MEMORY_ERROR;
            else {
                ++count;
                strings[random - 3] = 1;
            }
        }
    }
    return OK;
}