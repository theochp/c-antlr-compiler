int fac(int n) {
    int r;
    if (n>=1)
        r=n*fac(n-1);
    else
        r= 1;
    return r;
}

int main() {
    return fac(4);
}