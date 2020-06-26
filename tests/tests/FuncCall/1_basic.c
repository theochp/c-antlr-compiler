int toto(int a, int b) {
    int c = a + b;
    return c;
}

int main() {
    int a = toto(1,2) + toto(1, 3);
    return a;
}