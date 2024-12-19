#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    FILE_ERROR = -2,
    OVERFLOW_ERROR = -3
} error;

error Str_to_int(char* str, long long int* number, int base);

int main(int argc, char* argv[]) {
    int elem, ind = 0, max_base = -1;
    char input_path[PATH_MAX], output_path[PATH_MAX];
    const char *in_path, *out_path;
    char number[65];
    FILE *input_file, *output_file;

    if (argc != 3){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    in_path = (const char *) realpath(argv[1], input_path);
    out_path = (const char *) realpath(argv[2], output_path);

    if(!in_path || !out_path){
        printf("Problem with files.\n");
        return FILE_ERROR;
    }

    if(strcmp(in_path, out_path) == 0){
        printf("Input and output files can't be the same.\n");
        return FILE_ERROR;
    }

    input_file = fopen(argv[1], "r");
    output_file = fopen(argv[2], "w");

    if(input_file == NULL || output_file == NULL){
        printf("Problem with files.\n");
        if(input_file != NULL)
            fclose(input_file);
        if(output_file != NULL)
            fclose(output_file);
        return FILE_ERROR;
    }

    while(!feof(input_file)){
        elem = fgetc(input_file);

        if(!isspace(elem) && elem != EOF){
            if(elem == '0' && ind == 0) // проверка на ведущие нули
                continue; // если они есть то скипаем все что внизу и переходим к след итерации

            number[ind] = (char)(elem);
            ++ind;

            if(isdigit(elem) && elem - '0' + 1 > max_base){
                max_base = elem - '0' + 1;
            }
            else if ((isalpha(elem) && tolower(elem) - 'a' + 11 > max_base)
                     || (isupper(elem) && elem - 'A' + 11 > max_base)) {
                max_base = tolower(elem) - 'a' + 11;
            }
            else if((char)elem < '0' || (char)elem > 'z'){
                printf("Incorrect input.\n");
                fclose(input_file);
                fclose(output_file);
                return INPUT_ERROR;
            }
            number[ind] = '\0';

        }

        else{
            if(ind != 0 && max_base != -1){
                long long int answ = 0;

                if(Str_to_int(number, &answ, max_base) == OK){
                    fprintf(output_file, "%s %d %lli\n", number, max_base, answ);
                }

                else{
                    printf("Problems with input or overflow.\n");
                    fclose(input_file);
                    fclose(output_file);
                    return INPUT_ERROR;
                }
            }
            ind = 0;
            max_base = -1;

        }
    }
    printf("Done.\n");
    fclose(input_file);
    fclose(output_file);

    return OK;
}

error Str_to_int(char* str, long long int* number, int base){
    char *endptr;

    long long int num = strtoll(str, &endptr, base);
    if(num >= LLONG_MAX || num <= LLONG_MIN)
        return OVERFLOW_ERROR;

    else if(*endptr != '\0')
        return INPUT_ERROR;

    *number = num;

    return OK;

}