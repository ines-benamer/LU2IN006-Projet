#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include <stdlib.h>
#include <stdio.h>

#include "Reseau.h"
#include "SVGwriter.h"
#include "Chaine.h"

typedef struct{
    int u,v; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete{
    Arete *a; /* pointeur sur l’arete */
    struct cellule_arete* suiv;
} Cellule_arete;


typedef struct {
    int num; /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete* L_voisin; /* Liste chainee des voisins */
} Sommet;


typedef struct{
    int e1,e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct{
    int nbsom; /* Nombre de sommets */
    Sommet ** T_som; /* Tableau de pointeurs sur sommets */ 
    int gamma;
    int nbcommod; /* Nombre de commodites */
    Commod* T_commod; /* Tableau des commodites */
} Graphe;

Sommet *cree_sommet(int num, int x, int y);
Arete *cree_arete(int u, int v);
Cellule_arete *cree_cellule_arete(Arete *arete);
void liberer_graphe(Graphe *graphe);
void liberer_sommet(Sommet *sommet);
void liberer_arete(Cellule_arete *arete);
Graphe* creerGraphe(Reseau* r);
int plus_petit_nb_aretes(Graphe *graphe, int u, int v);
#endif
