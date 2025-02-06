#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    FILE_ERROR = -2
}error;

error Print_tree(FILE *input_file, FILE *output_file);

int main(int argc, char *argv[]){
    char input_path[PATH_MAX], output_path[PATH_MAX];
    const char *in_path, *out_path;
    FILE *input_file, *output_file;
    int count = 1;

    if(argc != 3){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    in_path = (const char *) realpath(argv[1], input_path);
    out_path = (const char *) realpath(argv[2], output_path);

    if(!in_path || !out_path){
        printf("Problem with files.\n");
        return FILE_ERROR;
    }

    if(strcmp(input_path, output_path) == 0){
        printf("Input and output files can't be the same.\n");
        return FILE_ERROR;
    }

    input_file = fopen(argv[1], "r");
    output_file = fopen(argv[2], "w");

    if(input_file == NULL || output_file == NULL){
        fclose(input_file);
        fclose(output_file);
        printf("Problem with files.\n");
        return FILE_ERROR;
    }

    while(!feof(input_file)){
        fprintf(output_file, "Tree № %d:\n", count);
        if(Print_tree(input_file, output_file) != OK){
            printf("Incorrect input.\n");
            fclose(input_file);
            fclose(output_file);
            return INPUT_ERROR;
        }
        fprintf(output_file, "\n");
        ++count;
    }
    fclose(input_file);
    fclose(output_file);

    return OK;
}

error Print_tree(FILE *input_file, FILE *output_file){
    char symb;
    int i, level = 0;
    symb = fgetc(input_file);

    while(symb != '\n' && symb != EOF){
        if(symb == '(')
            ++level;
        else if(symb == ')'){
            if(level != 0)
                --level;
            else // если ==0 то это лишняя скобка - ошибка
                return INPUT_ERROR;
        }
        else if(symb == ',' || symb == ' '){ // если запятая или пробел то игнорируем и читаем след символ
            symb = fgetc(input_file);
            continue;
        }
        else{ // для всех остальных символов
            for(i = 0; i < level; ++i)
                fprintf(output_file, "\t");

            if(!isspace(symb))
                fprintf(output_file, "->%c\n", symb);
        }
        symb = fgetc(input_file);
    }
    return OK;
}