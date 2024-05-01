// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"

// Question 1 :
Chaines *lectureChaines(FILE *f){
    Chaines *chaine = (Chaines *)malloc(sizeof(Chaines));
    fscanf(f, "NbChain: %d\n", &(chaine->nbChaines));
    fscanf(f, "Gamma: %d\n", &(chaine->gamma));

    chaine->chaines = NULL;

    for (int i = 0; i < chaine->nbChaines; i++){
        
        CellChaine *nouvelleChaine = (CellChaine *)malloc(sizeof(CellChaine));
        if (!nouvelleChaine){
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            return NULL;
        }

        fscanf(f, "%d", &(nouvelleChaine->numero));
        nouvelleChaine->points = NULL;

        int nbpoints;
        fscanf(f, "%d", &nbpoints);
        for (int j = 0; j < nbpoints; j++){
            CellPoint *nouveauPoint = (CellPoint *)malloc(sizeof(CellPoint));
            if (!nouveauPoint){
                fprintf(stderr, "erreur d'allocation de mémoire");
                return NULL;
            }
            fscanf(f, "%lf %lf", &(nouveauPoint->x), &(nouveauPoint->y));
            nouveauPoint->suiv = nouvelleChaine->points;
            nouvelleChaine->points = nouveauPoint;
        }

        nouvelleChaine->suiv = chaine->chaines;
        chaine->chaines = nouvelleChaine;
    }

    return chaine;
}

//-----------------------------------------------------------------------//
// Question 2

void ecrireChaines(Chaines *C, FILE *f){
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);
    CellChaine *chaine_cour = C->chaines;
    char tmp[20];
    while (chaine_cour){
        fprintf(f, "%d ", chaine_cour->numero);
        int compteur = 0;
        char str[100] = ""; // Chaîne de caractères temporaire dans laquelle on va stocker les points de la ligne
        CellPoint *point_cour = chaine_cour->points;

        while (point_cour){ // On parcourt la liste chaînée de points en points
            snprintf(tmp, 10, "%.2f", point_cour->x);
            strcat(str, tmp);
            strcat(str, " ");
            snprintf(tmp, 10, "%.2f", point_cour->y);
            strcat(str, tmp);
            strcat(str, " ");
            compteur++;
            point_cour = point_cour->suiv;
        }
        fprintf(f, "%d %s\n", compteur, str);
        chaine_cour = chaine_cour->suiv;
    }
}

//-----------------------------------------------------------------------//

void afficheChainesSVG(Chaines *C, char *nomInstance){
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx, precy;
    SVGwriter svg;
    ccour = C->chaines;
    while (ccour != NULL){
        pcour = ccour->points;
        while (pcour != NULL){
            if (maxx < pcour->x)
                maxx = pcour->x;
            if (maxy < pcour->y)
                maxy = pcour->y;
            if (minx > pcour->x)
                minx = pcour->x;
            if (miny > pcour->y)
                miny = pcour->y;
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVGinit(&svg, nomInstance, 500, 500);
    ccour = C->chaines;
    while (ccour != NULL){
        pcour = ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
        precx = pcour->x;
        precy = pcour->y;
        pcour = pcour->suiv;
        while (pcour != NULL){
            SVGline(&svg, 500 * (precx - minx) / (maxx - minx), 500 * (precy - miny) / (maxy - miny), 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            SVGpoint(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            precx = pcour->x;
            precy = pcour->y;
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVGfinalize(&svg);
}

//-----------------------------------------------------------------------//
// question 4 :

double longueurChaine(CellChaine *c){
    CellPoint *points_1;
    double longueur = 0;
    CellPoint *points_2;
    for (points_2 = c->points, points_1 = c->points; points_2; points_2 = points_2->suiv){
        double distance = sqrt(pow((points_2->x - points_1->x), 2) + pow((points_2->y - points_1->y), 2));
        longueur += distance;
        points_1 = points_2;
    }
    return longueur;
}

//-----------------------------------------------------------------------//

double longueurTotale(Chaines *C){
    double total = 0;
    CellChaine *chaine = C->chaines;
    while (chaine){
        total ++;
        chaine = chaine->suiv;
    }
    return total;
}

//-----------------------------------------------------------------------//
//question 5 :

int comptePointsChaine(CellChaine *C){
    CellPoint *point;
    int nbpoint=0;
    for(point = C->points; point ;point = point->suiv){
        nbpoint ++;
    }
    return nbpoint;
}
//-----------------------------------------------------------------------//
int comptePointsTotal(Chaines *C){

    CellChaine *chaine;
    int nbtotal = 0;
    for(chaine = C->chaines; chaine ; nbtotal += comptePointsChaine(chaine), chaine = chaine->suiv){
        continue;
    }
    return nbtotal;
}

//-----------------------------------------------------------------------//


Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {

    CellChaine* current_chaine;
    CellPoint* current_point;

    Chaines* C = (Chaines*) malloc(sizeof(Chaines));
    C->gamma = 5;
    C->nbChaines = nbChaines;
    C->chaines = NULL;

    for(int i=0; i<nbChaines; i++) {
        current_chaine = (CellChaine*) malloc(sizeof(CellChaine));
        current_chaine->numero = i+1;
        current_chaine->points = NULL;
        current_chaine->suiv = C->chaines;
        C->chaines = current_chaine;

        for(int i=0; i<nbPointsChaine; i++){
            current_point = (CellPoint*) malloc(sizeof(CellPoint));
            current_point->x = ( (double)rand() / (double) RAND_MAX ) * xmax;
            current_point->y = ( (double)rand() / (double) RAND_MAX ) * ymax;
            current_point->suiv = current_chaine->points;
            current_chaine->points = current_point;
        }
        
    }

    return C;
    
}


//-----------------------------------------------------------------------//


void libere_point(CellPoint *point){
    free(point);
}

//-----------------------------------------------------------------------//

void liberer_liste_points(CellPoint *liste){
    CellPoint *ptr = NULL;

    while (liste){
        ptr = liste->suiv;
        libere_point(liste);
        liste = ptr;
    }
    
}

//-----------------------------------------------------------------------//

void librer_chaine(CellChaine *chaine){
    if(! chaine){
        return ;
    }
    liberer_liste_points(chaine->points);
    free(chaine);
}

//-----------------------------------------------------------------------//

void liberer_liste_chaines(CellChaine *liste){
    CellChaine *chaine = NULL;
    while (liste){
        chaine = liste->suiv;
        librer_chaine(liste);
        liste = chaine;
    }
    
}

//-----------------------------------------------------------------------//

void liberer_structure(Chaines *structt){
    if(!structt){
        return ;
    }
    liberer_liste_chaines(structt->chaines);
    free(structt);

}