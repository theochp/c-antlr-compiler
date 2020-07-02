int main(){
    int tab[] = {18,68,16,65,24,872,6};
    int n=7;
    int non_ordonne = 1;
    while (non_ordonne){
        // Supposons le tableau ordonné
        non_ordonne = 0;
        int i=0;
        while (i<n-1){
            /* Si les 2 éléments sont mal triés */
            if(tab[i] > tab[i+1]){
                /* Inversion des 2 éléments */
                int tmp = tab[i+1];
                tab[i+1] = tab[i];
                tab[i] = tmp;

                /* Le tableau n'est toujours pas trié */
                non_ordonne = 1;
            }
            i++;
        }
    }
    // est-ce que le tri a fonctionne
    int j=0;
    int tri_ok = 1;
    while (j<n-1){
        if (tab[j]>tab[j+1]){
            tri_ok = 0;
        }
        j++;
    }
    return tri_ok;
}