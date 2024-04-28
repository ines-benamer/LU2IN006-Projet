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

    fprintf(f, "Nombre de chaînes : %d\n", nb_chaines);
    fprintf(f, "Nombre total de points : %d\n\n", comptePointsTotal(C));


    temps = clock();
    reconstitueReseauListe(C);
    temps = clock() - temps;
    fprintf(f, "Temps mis pour reconstituer le réseau avec la liste chaînée : %lf secondes\n\n", (double)temps/CLOCKS_PER_SEC);

    temps = clock();
    ReconstitueReseauArbre(C);
    temps = clock() - temps;
    fprintf(f, "Temps mis pour reconstituer le réseau avec l'arbre : %lf secondes\n\n", (double)temps/CLOCKS_PER_SEC);

    for(int m=1; m<100*nb_points_chaines; m+=15) {
        temps = clock();
        reconstitueReseauHachage(C, m);
        temps = clock() - temps;
        fprintf(f, "Temps mis pour reconstituer le réseau avec une table de hachage de taille %d : %lf secondes\n", m, (double)temps/CLOCKS_PER_SEC);
    }
}

int main(){
    srand(time(NULL));

    FILE* f = fopen("mon_test.txt", "w");
    if (!f) {
        printf("Erreur d'ouverture du fichier d'écriture\n");
        return 0;
    }

    fprintf(f, "COMPARAISON DES TEMPS DE RECONSTITUTION DE RESEAU - 100 points par chaîne\n\n\n");

    int nb_points_chaine = 100;
    int xmax = 5000;
    int ymax = 5000;

    for(int nb_chaines = 500; nb_chaines<2000; nb_chaines+=500){

        fprintf(f, "%d CHAINES\n\n", nb_chaines);

        ecrire_temps_fichier(f, nb_chaines, nb_points_chaine, xmax, ymax);
        fprintf(f, "\n\n");

    }

    fclose(f);
    return 0;
}