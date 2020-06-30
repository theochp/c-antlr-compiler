int main(){

    int a = 5;
    int b = 4;
    int c = 3;

    int t[] = {9,8,7,6,5,4,3,2,1,0};

    return t[5] - t[t[0]-t[a-b]] * c;
}