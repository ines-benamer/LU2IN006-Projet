// OULD BOUALI Louisa
// BENAMER BELKACEM Inès

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Reseau.h"

Noeud* creeNoeud(int num, int x, int y) {
    Noeud* n = (Noeud*) malloc(sizeof(Noeud));
    n->num = num;
    n->x = x;
    n->y = y;
    n->voisins = NULL;
    return n;
}

CellCommodite* creeCommodite(Noeud* a, Noeud* b){
    CellCommodite* com = (CellCommodite*) malloc(sizeof(CellCommodite));
    com->extrA = a;
    com->extrB = b;
    com->suiv = NULL;
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {

}