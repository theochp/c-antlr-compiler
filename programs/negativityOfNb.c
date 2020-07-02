#include <stdio.h>
int main() {
    int message[] = {69, 110, 116, 114, 101, 114, 32, 117, 110, 32, 110, 111, 109, 98, 114, 101, 32, 58, 32};
    int i = 0;
    for (i = 0; i < 19; i++) {
        putchar(message[i]);
    }
    int first_num = getchar();
    int second_num = getchar();

    if (second_num >= 48) {
        if  (second_num <= 57) {
            if (first_num == 45) {
                int neg_mes[] = {78, 111, 109, 98, 114, 101, 32, first_num, second_num, 32, 101, 115, 116, 32, 110, 101, 103, 97, 116, 105, 118, 101, 10};
                for (i = 0; i < 23; i++) {
                    putchar(neg_mes[i]);
                }
            }
            else if (first_num >= 48) {
                if (first_num < 57) {
                    int pos_mes[] = {78, 111, 109, 98, 114, 101, 32, first_num, second_num, 32, 101, 115, 116, 32, 112, 111, 115, 105, 116, 105, 118, 101, 10};

                    for (i = 0; i < 23; i++) {
                        putchar(pos_mes[i]);
                    }
                }
            }
        }
    } else if (second_num == 10){
        if (first_num >= 48) {
            if (first_num < 57) {
                int pos_mes_space[] = {78, 111, 109, 98, 114, 101, 32, first_num, 32, 101, 115, 116, 32, 112, 111, 115, 105, 116, 105, 118, 101, 10};
                for (i = 0; i < 22; i++) {
                    putchar(pos_mes_space[i]);
                }
            }
        }
    } else {
        int invalid_mes[] = {69, 110, 116, 114, 101, 101, 32, 105, 110, 118, 97, 108, 105, 100};
        for (i = 0; i < 14; i++) {
            putchar(invalid_mes[i]);
        }
    }
    return 0;
}
