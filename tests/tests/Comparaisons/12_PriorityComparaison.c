int main(){
    // avec priorité : 1 != 0 --> 1
    // sans priorité : 1 != 1 > 1 --> 0 > 1 --> 0
    return 2 <= 3 != 1 > 1;
}