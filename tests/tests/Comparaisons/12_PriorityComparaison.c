int main(){
    /* avec priorite 1 != 0 --> 1
     * sans priorite 1 != 1 > 1 --> 0 > 1 --> 0
    */
    return 2 <= 3 != 1 > 1;
}