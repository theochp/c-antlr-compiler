int main() {
    int a;
    int b = 0;
    for(a = 10; a > 0 ; a=a-1) {
        b = 2 * a;
        a = a - 1;
    }
    return b;
}