int main() {
    int a = 3;
    if (a == 2) {
        a = 5;
        if (a < 6) {
            a = 4;
        } else {
            a = 55;
        }
    } else if(a == 3) {
        a = 6;
    } else {
        a = 4;
    }
    return a;
}