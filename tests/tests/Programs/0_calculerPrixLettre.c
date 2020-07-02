#include <stdio.h>

int calculerPrixLettre(int poidsLettre){
    // ceci calcule le prix d'une lettre selon son poids
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

int main(){
    int poidsLettres[6] = {368,1200,22,12,741,42};
    int i=0;
    int prixTotal=0;
    while (i < 6){
        int prix = calculerPrixLettre(poidsLettres[i]);
        if (prix == -1){
            return -1 ;
        }
        prixTotal = prixTotal + prix;
        i++;
    }
    return prixTotal;

}