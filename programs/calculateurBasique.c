#include <stdio.h>
int main() {
    int message[] = {69, 110, 116, 114, 101, 114, 32, 117, 110, 32, 111, 112, 101, 114, 97, 116, 101, 117, 114, 32, 40, 43, 44, 32, 45, 44, 32, 42, 44, 32, 47, 44, 32, 41, 32, 58, 10};
    int i = 0;
    for (i = 0; i < 37; i++) {
        putchar(message[i]);
    }
    int operator = getchar();

    int mes[] = {83, 97, 105, 115, 105, 114, 32, 50, 32, 111, 112, 101, 114, 97, 110, 100, 101, 115, 32, 58, 10};
    for (i = 0; i < 21; i++) {
        putchar(mes[i]);
    }
    int space = getchar();

    int first_op = getchar();
    space = getchar();
    int second_op = getchar();
    int result[] = {first_op, 32, operator, 32, second_op, 32, 61, 32};

    if (operator == 43)
        result[8] = first_op + second_op - 48;
    else if (operator == 45)
        result[8] = first_op - second_op + 48;
    else if (operator == 42)
        result[8] = (first_op-48) * (second_op-48) + 48;
    else if (operator == 47)
        result[8] = (first_op-48)/(second_op-48) + 48;

    for(i = 0; i < 9; i++){
        putchar(result[i]);
    }
    putchar(10);

    return 0;
}