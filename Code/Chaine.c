#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"


// Question 2

void ecrireChaines(Chaines* C, FILE* f) {

    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);
    CellChaine* chaine_cour = C->chaines;

    char* tmp = "";

    while (chaine_cour) {
        fprintf(f, "%d ", chaine_cour->numero);
        int compteur = 0;
        char* str = ""; // Chaîne de caractères temporaire dans laquelle on va stocker les points de la ligne
        CellPoint* point_cour = chaine_cour->points;

        while (point_cour) { // On parcourt la liste chaînée de points en 
            sprintf(tmp, "%f", point_cour->x);
            strcat(str, tmp);
            strcat(str, " ");
            sprintf(tmp, "%f", point_cour->y);
            strcat(str, tmp);
            strcat(str, " ");
            compteur++;
            point_cour = point_cour->suiv;
        }
        fprintf(f, "%d %s\n", compteur, str);
        chaine_cour = chaine_cour->suiv;
    }
}
