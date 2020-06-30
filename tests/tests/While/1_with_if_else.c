int main() {
    int a = 1;
    int b = 0;
    int c = 0;

    while ( c < 10) {
        if ( a > b) {
            c = c +a;
            b = a +1
        } else {
            c = c + b;
            a = c;
        }

    }

    return a - c + b;
}