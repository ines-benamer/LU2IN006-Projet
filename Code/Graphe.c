#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Graphe.h"

// question 7.1 : 

Graphe* creerGraphe(Reseau* r){
    if(!r){
        return NULL;
    }
    Graphe *graphe = (Graphe*)malloc(sizeof(Graphe));
    if(!graphe){
        return NULL;
    }

    graphe->gamma = r->gamma;
    graphe->nbsom = r->nbNoeuds;
    graphe->nbcommod = nbCommodites(r);

    // on alloue de la memoire necessaire pour le tableau des commodites.
    graphe->T_commod = (Commod*)malloc(sizeof(Commod)* graphe -> nbcommod);
    if (! graphe->T_commod){
        free(graphe);
        return NULL;
    }


}