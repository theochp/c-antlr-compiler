int toto(int a, int b) {
    int c = a + b;
    if (b) {
        c = c = toto(c,c);
    }
    return c;
}

int main() {
    int a = toto(1,0) + toto(1, 3);
    return a;
}