#include <stdio.h>
#include <limits.h>

typedef enum errors{
    OK = 1,
    INPUT_ERROR = -1,
} error;

error Conversion_to_system(int number, int r, char *answer, int *sign);

int Plus(int a, int b);

int main(){
    char answer[65];
    int sign = 0;
    error check = Conversion_to_system(255, 1, answer, &sign);
    if(check == INPUT_ERROR){
        printf("Incorrect input. Try again.\n");
        return INPUT_ERROR;
    }
    else{
        if(sign == 1)
            printf("-");
        printf("%s\n", answer);
    }
    return OK;

}

error Conversion_to_system(int number, int r, char *answer, int *sign){
    char base[] = "01234567890123456789ABCDEFGHIJKLMNOPQRSTUV"; // символы для представления чисел в сс до 32-ричной
    int count = 0, remain, len, res, i; // remain - остаток
    char symbol;
    if(r < 1 || r > 5)
        return INPUT_ERROR;

    if(number < 0){ // для отриц чисел
        number = Plus(~number, 1); // доп код (инвертировать все разряды и +1)
        *sign = 1;
    }

    while(number != 0){
        remain = ((number >> r) << r) ^ number; // получаем остаток от деления числа на 2^r
        answer[count] = base[remain]; // записываем остаток в строку
        count = Plus(count, 1); // увеличиваем счетчик
        number = number >> r; // делим нацело на 2^r
    }
    answer[count] = '\0';

    len = count >> 1; // половина длины строки
    res = Plus(count, -1); // индекс последнего символа
    for(i = 0; i < len; i = Plus(i, 1)){ // обмен символов между началом и концом строки
        symbol = answer[i]; // сохраняем текущий символ
        answer[i] = answer[Plus(res, -i)]; // заменяем символ в начале строки из конца строки
        answer[Plus(res, -i)] = symbol; // заменяем символ в конце строки на сохраненный символ из начала
    }

    return OK;
}

int Plus(int a, int b){ // сложение в столбик
    int sum_ones, shift;
    while(b != 0){ // цикл повторяется до тех пор, пока не останется переносов
        sum_ones = a ^ b; // сложение битов без учёта переноса.
        shift = (a & b) << 1; // вычисление переноса и его перемещение на следующий разряд

        a = sum_ones;
        b = shift;
    }
    return a;
}