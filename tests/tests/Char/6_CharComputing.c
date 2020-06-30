int maFonction(int a, int b){
    return a*b;
}

int main(){
    char a = 10;
    char b = 20;
    char res = maFonction(a,b)+maFonction(a-1,b-1)+maFonction(a-2,b-2)+maFonction(maFonction(a,b),b);
    return res;
}
