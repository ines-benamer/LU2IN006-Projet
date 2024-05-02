// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

void ecrire_temps_fichier(FILE* f, int nb_chaines, int nb_points_chaines, int xmax, int ymax) {
    Chaines* C = generationAleatoire(nb_chaines, nb_points_chaines, xmax, ymax);
    clock_t temps;
    Reseau* R;

    fprintf(f, "Nombre de chaînes : %d\n", nb_chaines);
    fprintf(f, "Nombre total de points : %d\n\n", comptePointsTotal(C));


    temps = clock();
    R = reconstitueReseauListe(C);
    temps = clock() - temps;
    fprintf(f, "Temps mis pour reconstituer le réseau avec la liste chaînée : %lf secondes\n\n", (double)temps/CLOCKS_PER_SEC);
    libererReseau(R);
    


    // temps = clock();
    // R = ReconstitueReseauArbre(C);
    // temps = clock() - temps;
    // fprintf(f, "Temps mis pour reconstituer le réseau avec l'arbre : %lf secondes\n\n", (double)temps/CLOCKS_PER_SEC);


    int nb_pts_tot = nb_chaines*nb_points_chaines;
    int pas = (int) (nb_pts_tot/20);

    for(int m=500; m<nb_pts_tot; m+=pas) {
        temps = clock();
        R = reconstitueReseauHachage(C, m);
        temps = clock() - temps;
        fprintf(f, "Temps mis pour reconstituer le réseau avec une table de hachage de taille %d : %lf secondes\n", m, (double)temps/CLOCKS_PER_SEC);
        libererReseau(R);
    }

    liberer_structure(C);


}

int main(){
    srand(time(NULL));

    FILE* f = fopen("compare_temps_alea.txt", "w");
    if (!f) {
        printf("Erreur d'ouverture du fichier d'écriture\n");
        return 0;
    }

    fprintf(f, "COMPARAISON DES TEMPS DE RECONSTITUTION DE RESEAU - 100 points par chaîne\n\n\n");

    int nb_points_chaine = 100;
    int xmax = 5000;
    int ymax = 5000;

    for(int nb_chaines = 500; nb_chaines<=5000; nb_chaines+=500){

        fprintf(f, "%d CHAINES\n\n", nb_chaines);
        printf("nb chaines : %d\n", nb_chaines);

        ecrire_temps_fichier(f, nb_chaines, nb_points_chaine, xmax, ymax);
        fprintf(f, "\n\n");

    }

    fclose(f);
    return 0;
}