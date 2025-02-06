#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#define eps 1e-7

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
    FILE_ERROR = -2,
    MEMORY_ERROR = -3
}error;

typedef struct Employee{
    unsigned int id;
    char name[100];
    char surname[100];
    double salary;
}Employee;

error Read_info(int *size, Employee **employers, FILE *input_file, int *count); // Функция для чтения данных о сотрудниках из файла
error Check_info(Employee info);  // Функция для проверки корректности данных сотрудника
int Comparator(const void *a, const void *b);

int main(int argc, char* argv[]) {
    char in_path[PATH_MAX], out_path[PATH_MAX];
    FILE *input_file, *output_file;
    Employee *employers;
    int size = 2, count = 0, i;

    if (argc != 4 || (argv[2][0] != '-' && argv[2][0] != '/') || argv[2][2] != '\0') {
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }

    if (!realpath(argv[1], in_path) || !realpath(argv[3], out_path)) {
        printf("Problem with files.\n");
        return FILE_ERROR;
    }

    if (strcmp(in_path, out_path) == 0) {
        printf("Input and output files can't be the same.\n");
        return FILE_ERROR;
    }

    employers = (Employee *) malloc(size * sizeof(Employee));
    if (employers == NULL) {
        printf("Problems with malloc.\n");
        return MEMORY_ERROR;
    }

    input_file = fopen(in_path, "r");
    output_file = fopen(out_path, "w");
    if (input_file == NULL || output_file == NULL) {
        printf("Problem with files.\n");
        fclose(input_file);
        fclose(output_file);
        return FILE_ERROR;
    }

    switch (argv[2][1]) {
        case 'a':
            if(Read_info(&size, &employers, input_file, &count) == OK){
                qsort(employers, count, sizeof(Employee), Comparator);
                for(i = 0; i < count; ++i){
                    fprintf(output_file, "%u %s %s %lf\n", employers[i].id, employers[i].name, employers[i].surname, employers[i].salary);
                }
                printf("Done.\n");
            }
            else{
                printf("Incorrect input.\n");
                fclose(input_file);
                fclose(output_file);
                free(employers);
                return INPUT_ERROR;
            }
            break;

        case 'd':
            if(Read_info(&size, &employers, input_file, &count) == OK){
                qsort(employers, count, sizeof(Employee), Comparator);
                for(i = count - 1; i >= 0; --i){
                    fprintf(output_file, "%u %s %s %lf\n", employers[i].id, employers[i].name, employers[i].surname, employers[i].salary);
                }
                printf("Done.\n");
            }
            else{
                printf("Incorrect input.\n");
                fclose(input_file);
                fclose(output_file);
                free(employers);
                return INPUT_ERROR;
            }
            break;

        default:
            printf("Incorrect input.\n");
            break;
    }
    fclose(input_file);
    fclose(output_file);
    free(employers);
    return OK;
}

error Read_info(int *size, Employee **employers, FILE *input_file, int *count) { //  читает данные о сотрудниках из файла и сохраняет их в динамически выделенный массив структур
    int check_read_file;
    Employee *new_struct; // для проверки реаллока

    while((check_read_file = fscanf(input_file, "%u%s%s%lf\n", &(*employers)[*count].id, (*employers)[*count].name,
                                      (*employers)[*count].surname, &(*employers)[*count].salary)) == 4) {
        if (Check_info((*employers)[*count]) != OK)
            return INPUT_ERROR;

        ++(*count);
        if (*count == *size) {
            *size *= 2;
            new_struct = (Employee *) realloc(*employers, *size * sizeof(Employee));
            if (new_struct == NULL) {
                free(*employers);
                return MEMORY_ERROR;
            }
            *employers = new_struct;
        }
    }
    if(check_read_file < 4 && check_read_file > 0)
        return INPUT_ERROR;

    return OK;
}

error Check_info(Employee info){
    int size, i;
    if(info.salary < 0)
        return INPUT_ERROR;
    size = strlen(info.name);
    for(i = 0; i < size; ++i){
        if(info.name[i] < 'A' || info.name[i] > 'z' || (info.name[i] > 'Z' && info.name[i] < 'a'))
            return INPUT_ERROR;
    }
    size = strlen(info.surname);
    for(i = 0; i < size; ++i){
        if(info.surname[i] < 'A' || info.surname[i] > 'z' || (info.surname[i] > 'Z' && info.surname[i] < 'a'))
            return INPUT_ERROR;
    }
    return OK;
}

int Comparator(const void *a, const void *b){
    const Employee *first = (const Employee *) a;
    const Employee *second = (const Employee *) b;
    int check;

    if(first->salary - second->salary > eps)
        return 1;
    else if(second->salary - first->salary > eps)
        return -1;

    check = strcmp(first->surname, second->surname);
    if(check > 0)
        return 1;
    else if(check < 0)
        return -1;

    check = strcmp(first->name, second->name);
    if(check > 0)
        return 1;
    else if(check < 0)
        return -1;

    return first->id - second->id;
}