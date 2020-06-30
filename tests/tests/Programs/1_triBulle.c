int main(){
    int tab[] = {18,68,16,65,24,872,6};
    int n=7;
    tri_a_bulle(tab, n);
    // est-ce que le tri a fonctionne
    int i=0;
    while (i<n-1){
        if (tab[i]>tab[i+1]){
            return 0;
        }
        i++;
    }
    return 1;
}

void tri_a_bulle(int t[], int n){
    int non_ordonne = 1;
    while (non_ordonne){
        // Supposons le tableau ordonné
        non_ordonne = 0;
        int i=0;
        while (i<n-1){
            /* Si les 2 éléments sont mal triés */
            if(t[i] > t[i+1]){
                /* Inversion des 2 éléments */
                int tmp = t[i+1];
                t[i+1] = t[i];
                t[i] = tmp;

                /* Le tableau n'est toujours pas trié */
                non_ordonne = 1;
            }
            i++;
        }
    }
}