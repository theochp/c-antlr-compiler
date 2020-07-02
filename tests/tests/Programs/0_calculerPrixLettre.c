#include <stdio.h>

int main(){
    int poidsLettres[6] = {368,1200,22,12,741,42};
    int i=0;
    int prixTotal=0;
    putchar(1);
    while (i < 6){
        int prix = calculerPrixLettre(poidsLettres[i]);
        if (prix == -1){
            putchar(-1);
            return -1 ;
        }
        prixTotal = prixTotal + prix;
        putchar(i);
        putchar(prix);
        i++;
    }
    return prixTotal;

}

int calculerPrixLettre(int poidsLettre){
    // ceci calcule le prix d'une lettre selon son poids
    putchar(42);
    if(poidsLettre <= 20)
        return 1;
    if (poidsLettre <= 100)
        return 2;
    if (poidsLettre <= 250)
        return 4;
    if (poidsLettre <= 300)
        return 6;
    if (poidsLettre <= 3000)
        return 9;
    return -1;
}