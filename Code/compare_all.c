#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main() {

    srand(time(NULL));
    clock_t temps;

    printf("Veuillez entrer le nom du fichier à lire\n");
    char nom_fic[70];
    scanf("%s", nom_fic);
    FILE* fic_lecture = fopen(nom_fic, "r");
    if (!fic_lecture) {
        printf("Erreur d'ouverture du fichier donné\n");
        return 0;
    }

    Chaines* C = lectureChaines(fic_lecture);
    int nb_points = comptePointsTotal(C);

    FILE* compare_temps = fopen("comparaison_temps.txt", "w");
    if (!compare_temps) {
        printf("Erreur d'ouverture du fichier décriture\n");
        return 0;
    }
    
    fprintf(compare_temps, "---------------- COMPARAISON DES TEMPS DE RECONSTITUTION DE RESEAU ----------------\n\n\n");
    
    fprintf(compare_temps, "Nombre total de points : %d\n\n", nb_points);

    temps = clock();
    reconstitueReseauListe(C);
    temps = clock() - temps;
    fprintf(compare_temps, "Temps mis pour reconstituer le réseau avec la liste : %lf\n", (double)temps/CLOCKS_PER_SEC);

    temps = clock();
    ReconstitueReseauArbre(C);
    temps = clock() - temps;
    fprintf(compare_temps, "Temps mis pour reconstituer le réseau avec l'arbre : %lf\n", (double)temps/CLOCKS_PER_SEC);

    for (int i=0; i<10*nb_points; i++){

        temps = clock();
        reconstitueReseauHachage(C, i);
        temps = clock() - temps;

        fprintf(compare_temps, "Temps mis pour reconstituer le réseau avec une table de hachage de taille %d : %lf\n", i, (double)temps/CLOCKS_PER_SEC);

    }

    fclose(compare_temps);

    return 0;


}