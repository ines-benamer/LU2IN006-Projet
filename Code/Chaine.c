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

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}
