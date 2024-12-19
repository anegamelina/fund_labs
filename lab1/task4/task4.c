#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

typedef enum errors{
    OK = 1,
    MALLOC_ERROR = 0,
    INPUT_ERROR = -1,
    FLAG_ERROR = -2,
    FILE_ERROR = -3
} error;

int main(int argc, char *argv[]) {
    char flag, x;
    int elem;
    int counter = 0;
    char input_path[PATH_MAX], output_path[PATH_MAX];
    const char *in_path, *out_path;

    if (argc > 4 || argc < 3){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }
    if ((argv[1][0] == '-' || argv[1][0] == '/') == 0){
        printf("Incorrect flag input. Try again.\n");
        return FLAG_ERROR;
    }

    FILE *input_file = fopen(argv[2], "r");
    if(input_file == NULL){ // ошибка при открытии входного файла
        printf("Problem with opening file. Try again.\n");
        return FILE_ERROR;
    }

    FILE *output_file;
    if(argv[1][1] == 'n' && argv[1][3] == '\0' && argc == 4){ // если известен выходной файл
        flag = argv[1][2];

        in_path = (const char *) realpath(argv[2], input_path);
        out_path = (const char *) realpath(argv[3], output_path);

        if(!in_path || !out_path){
            printf("1 Problem with files.\n");
            fclose(input_file);
            return FILE_ERROR;
        }

        if(strcmp(in_path, out_path) == 0){
            printf("Input and output files can't be the same.\n"); // проверка если входной и выходной файлы совпадают
            fclose(input_file);
            return FILE_ERROR;
        }

        output_file = fopen(argv[3], "w");
        if (output_file == NULL){
            printf("Problem with opening file. Try again.\n");
            fclose(input_file);
            return FILE_ERROR;
        }
    }

    else if((argv[1][1] == 'd' || argv[1][1] == 'i' || argv[1][1] == 's' || argv[1][1] == 'a') && (argv[1][2] == '\0')){ // если неизвестен выходной файл
        flag = argv[1][1];
        in_path = (const char *) realpath(argv[2], input_path);
        if(!in_path){
            printf("2 Problem with files.\n");
            fclose(input_file);
            return FILE_ERROR;
        }

        char *output_name = (char *) malloc((strlen(in_path) + 5) * sizeof(char));
        if(output_name == NULL){
            printf("Problem with memory.\n");
            fclose(input_file);
            return MALLOC_ERROR;
        }

        char *last_slash = strrchr(in_path, '/');
        if(last_slash)
            ++last_slash;
        else
            last_slash = (char *) in_path; // устанавливаем указатель на начало in_path

        int len_path = strlen(in_path) - strlen(last_slash);
        strncpy(output_name, in_path, len_path);
        strcat(output_name, "out_");
        strcat(output_name, last_slash);
        output_name[strlen(in_path) + 4] = '\0';

        if(strcmp(in_path, output_name) == 0){
            printf("Input and output files can't be the same.\n"); // проверка если входной и выходной файлы совпадают
            fclose(input_file);
            free(output_name);
            return FILE_ERROR;
        }

        output_file = fopen(output_name, "w");
        if (output_file == NULL){
            printf("Problem with opening file. Try again.\n");
            fclose(input_file);
            free(output_name);
            return FILE_ERROR;
        }
        free(output_name);
    }

    else{
        printf("Incorrect input. Try again.\n");
        fclose(input_file);
        return FLAG_ERROR;
    }


    switch(flag){
        case 'd': // исключить арабские цифры
            while((elem = fgetc(input_file)) != EOF){
                x = (char) elem;
                if (isdigit(x) == 0)
                    fputc(x, output_file);
            }
            printf("Done.\n");
            break;

        case 'i': // сколько раз в строке встречаются латинские буквы
            while((elem = fgetc(input_file)) != EOF){
                x = (char) elem;
                if (isalpha(x))
                    ++counter;
                else if(x == '\n'){
                    fprintf(output_file, "%d\n", counter);
                    counter = 0;
                }
            }
            fprintf(output_file, "%d\n", counter);
            printf("Done.\n");
            break;

        case 's': // сколько раз в строке встречаются символы, отличные от англ букв, цифр и пробелов
            while((elem = fgetc(input_file)) != EOF){
                x = (char) elem;

                if(x == '\n'){
                    ++counter;
                    fprintf(output_file, "%d\n", counter);
                    counter = 0;
                }

                else if((isalpha(x) || isdigit(x) || x == ' ') == 0) {
                    ++counter;
                }
            }
            fprintf(output_file, "%d\n", counter);
            printf("Done.\n");
            break;

        case 'a': // заменить все символы кроме цифр на ASCII коды в 16 СС
            while((elem = fgetc(input_file)) != EOF){
                x = (char) elem;
                if(isdigit(x) || x == '\n')
                    fprintf(output_file, "%c", x);
                else
                    fprintf(output_file, "%X", x);
            }
            printf("Done.\n");
            break;
        default:
            printf("Incorrect flag input.\n");
            fclose(input_file);
            fclose(output_file);
            return FLAG_ERROR;
    }

    fclose(input_file);
    fclose(output_file);
    return OK;
}
