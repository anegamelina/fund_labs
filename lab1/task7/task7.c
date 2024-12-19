#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    FLAG_ERROR = -2,
    FILE_ERROR = -3,
    MALLOC_ERROR = -4
} error;

error Conversion_to_cc(int num, int base, FILE *file);

int main(int argc, char *argv[]) {
    FILE *file1, *file2, *output_file;
    char file1_path[PATH_MAX], file2_path[PATH_MAX], output_path[PATH_MAX];
    const char *f1_path, *f2_path, *out_path;
    int elem1, elem2;
    int check = 0, position = 1;

    if(!((argv[1][0] == '/' || argv[1][0] == '-') && argv[1][2] == '\0')){
        printf("Incorrect flag input.\n");
        return FLAG_ERROR;
    }

    switch(argv[1][1]) {
        case 'r':
            if (argc != 5) { // проверяем соответствие колва арг флагу
                printf("Incorrect number of arguments. Try again.\n");
                return INPUT_ERROR;
            }

            f1_path = (const char *) realpath(argv[2], file1_path);
            f2_path = (const char *) realpath(argv[3], file2_path);
            out_path = (const char *) realpath(argv[4], output_path);

            if(!f1_path || !f2_path || !out_path){
                printf("Problem with files.\n");
                return FILE_ERROR;
            }

            if(strcmp(f1_path, out_path) == 0 || strcmp(f2_path, out_path) == 0){
                printf("Input and output files can't be the same.\n");
                return FILE_ERROR;
            }

            file1 = fopen(argv[2], "r");
            file2 = fopen(argv[3], "r");
            output_file = fopen(argv[4], "w");
            if (file1 == NULL || file2 == NULL || output_file == NULL) { // если возникла ошибка в файлах
                if (file1 != NULL)
                    fclose(file1);
                if (file2 != NULL)
                    fclose(file2);
                if (output_file != NULL)
                    fclose(output_file);
                printf("Problem with files.\n");
                return FILE_ERROR;
            }

            while(!feof(file1) || !feof(file2)) {

                // если нечетная позиция
                if (position % 2 != 0) {
                    elem1 = fgetc(file1);

                    if (elem1 == EOF) {
                        if (check == 1) { // проверка что слово закончилось
                            fprintf(output_file, " ");
                        }
                        ++position;
                    }
                    else if (isspace(elem1)) { // проверка на разделитель
                        if (check == 1) {
                            fprintf(output_file, " ");
                            check = 0; // пометили что прошли лексему
                            ++position;
                        }
                    }
                    else {
                        fprintf(output_file, "%c", (char)elem1); // выводим слово в выходной файл
                        check = 1; // пометили что мы находимся в лексеме
                    }
                }

                    // если четная позиция
                else {
                    elem2 = fgetc(file2);
                    if (elem2 == EOF) {
                        if (check == 1) { // проверка что слово закончилось
                            fprintf(output_file, " ");
                        }
                        ++position;
                    }
                    else if (isspace(elem2)) { // проверка на разделитель
                        if (check == 1) {
                            fprintf(output_file, " ");
                            check = 0; // пометили что прошли лексему
                            ++position;
                        }
                    }
                    else {
                        fprintf(output_file, "%c", (char) elem2); // выводим слово в выходной файл
                        check = 1; // пометили что мы находимся в лексеме
                    }
                }
            }
            fclose(file1);
            fclose(file2);
            fclose(output_file);
            break;

        case 'a':
            if (argc != 4) {
                printf("Incorrect number of arguments. Try again.\n");
                return INPUT_ERROR;
            }
            f1_path = (const char *) realpath(argv[2], file1_path);
            out_path = (const char *) realpath(argv[3], output_path);

            if(!f1_path || !out_path){
                printf("Problem with files.\n");
                return FILE_ERROR;
            }

            if(strcmp(f1_path, out_path) == 0){
                printf("Input and output files can't be the same.\n");
                return FILE_ERROR;
            }

            file1 = fopen(argv[2], "r");
            output_file = fopen(argv[3], "w");
            if (file1 == NULL ||output_file == NULL) { // если возникла ошибка в файлах
                if (file1 != NULL)
                    fclose(file1);
                if (output_file != NULL)
                    fclose(output_file);
                printf("Problem with files.\n");
                return FILE_ERROR;
            }

            while(!feof(file1)){
                elem1 = fgetc(file1);
                if(elem1 == EOF)
                    continue;
                if(isspace(elem1)){
                    if(check == 1){
                        fprintf(output_file, " ");
                        check = 0;
                        ++position;
                    }
                }
                else {
                    if (position % 10 == 0) {
                        if(isalpha(elem1) && elem1 >= 'A' && elem1 <= 'Z')
                            elem1 += 32; // преобразуем в строчную
                        if(Conversion_to_cc(elem1, 4, output_file) != OK){ // преобр в 4 СС
                            printf("Problems with memory or base.\n");
                            return MALLOC_ERROR;
                        }
                        check = 1; // пометка что мы сейчас находимся внутри лексемы
                    }
                    else if (position % 2 == 0) {
                        if(isalpha(elem1) && elem1 >= 'A' && elem1 <= 'Z')
                            elem1 += 32;
                        fprintf(output_file, "%c", (char) elem1);
                        check = 1;
                    }
                    else if (position % 5 == 0) {
                        if(Conversion_to_cc(elem1, 8, output_file) != OK){
                            printf("Problems with memory or base.\n");
                            return MALLOC_ERROR;
                        }
                        check = 1;
                    }
                    else{
                        fprintf(output_file, "%c", (char) elem1);
                        check = 1;
                    }
                }
            }
            fclose(file1);
            fclose(output_file);
            break;

        default:
            printf("There is no such flag.\n");
            return FLAG_ERROR;
    }
    return OK;
}

error Conversion_to_cc(int num, int base, FILE *file){

    if(base < 2 || base > 36)
        return INPUT_ERROR;

    if(num == 0){
        fprintf(file, "0");
        return OK;
    }

    char *array = malloc(1 * sizeof(char));
    int ind = 0;

    if (array == NULL) {
        return MALLOC_ERROR;
    }

    while (num > 0) {
        if (num % base <= 9)
            array[ind] = (char) (num % base + '0');
        else
            array[ind] = (char) (num % base - 10 + 'a'); // для СС > 10

        num /= base;
        ++ind;

        char *check = realloc(array, (sizeof(array) + 1) * sizeof(char));
        if (check == NULL) {
            free(array);
            return MALLOC_ERROR;
        }
        array = check;
    }
    for (int i = ind - 1; i >= 0; --i)
        fprintf(file, "%c", array[i]);

    free(array);
    return OK;
}