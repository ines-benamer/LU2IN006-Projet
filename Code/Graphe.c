#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Graphe.h"

// question 7.1 : 

//------------------------------------------CREE_SOMMET-----------------------------------
Sommet *cree_sommet(int num, int x, int y) {
    // On cree un sommet
    Sommet *new = (Sommet *)malloc(sizeof(Sommet));
    if (!new) return NULL;
    new->L_voisin = NULL;
    new->num = num;
    new->x = x;
    new->y = y;

    return new;
}

//------------------------------------------CREE_ARETE-----------------------------------

Arete *cree_arete(int u, int v) {
    // On cree une arete
    Arete *arete = (Arete *)malloc(sizeof(Arete));
    if (!arete) return NULL;
    arete->u = u;
    arete->v = v;

    return arete;
}

//------------------------------------------CREE_CELLULE_ARETE-----------------------------------

Cellule_arete *cree_cellule_arete(Arete *arete) {
    // On cree une cellule d'arete
    Cellule_arete *cellule = (Cellule_arete *)malloc(sizeof(Cellule_arete));
    if (!cellule) return NULL;
    cellule->a = arete;
    cellule->suiv = NULL;

    return cellule;
}

//------------------------------------------LIBERER_GRAPHE-----------------------------------

void liberer_graphe(Graphe *graphe) {
    if (!graphe) return;
    free(graphe->T_commod);
    for (int i = 0; i < graphe->nbsom; i++)
        liberer_sommet(graphe->T_som[i]);
    free(graphe->T_som);
    free(graphe);
}

//------------------------------------------LIBERER_SOMMET-----------------------------------

void liberer_sommet(Sommet *sommet) {
    if (!sommet) return;
    Cellule_arete *tmp = NULL;
    while (sommet->L_voisin) {
        tmp = sommet->L_voisin->suiv;
        liberer_arete(sommet->L_voisin);
        sommet->L_voisin = tmp;
    }
    free(sommet);
}

//------------------------------------------LIBERER_ARETE----------------------------------

void liberer_arete(Cellule_arete *arete) {
    if (!arete) return;
    if (arete->a->u == -1 && arete->a->v == -1)
        free(arete->a);
    else {
        arete->a->u = -1;
        arete->a->v = -1;
    }
    free(arete);
}

//------------------------------------------CREE_GRAPHE-----------------------------------

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

    // on alloue la memoire necessaire pour le tableau des sommets 
    graphe->T_som = (Sommet **)malloc(sizeof(Sommet *)*graphe->nbsom);
    if (! graphe->T_som){
        free(graphe->T_commod);
        free(graphe);
        return NULL;
    }

    //initialisation des cases du tableau des sommets a NULL
    for(int i = 0; i<graphe->nbsom;i++){
        graphe->T_som[i] = NULL;
    }
    // initialisation des structures du tableau commodites suivant les commodites du reseau
    int i = 0;
    for(CellCommodite * comod = r->commodites;comod;comod = comod->suiv,i++){
        graphe->T_commod[i].e1 = comod->extrA->num -1;
        graphe->T_commod[i].e2 = comod->extrB->num -1;

    }

    // boucle pour creer les sommets du graphe a partir des noeuds du reseau 
    for(CellNoeud *noeud = r->noeuds;noeud;noeud = noeud->suiv){
        graphe->T_som[noeud->nd->num -1] = 0;
    }

}// a finir c'est trop long 