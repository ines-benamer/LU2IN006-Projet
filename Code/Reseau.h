#ifndef __RESEAU_H__
#define __RESEAU_H__
#include "Chaine.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;

Noeud* creeNoeud(int num, int x, int y);
CellCommodite* creeCommodite(Noeud* a, Noeud* b);
Reseau* creeReseau(int gamma);
CellNoeud *creeCellNoeud(Noeud *noeud);
void rendreVoisins(Noeud* n1, Noeud* n2);
void ajouterNoeud(Reseau* R, Noeud* n);
void ajouteVoisins(Noeud* n1, Noeud* n2);
void libererReseau(Reseau *reseau);
void libererCellNoeuds(CellNoeud *cells, int rm);
void libereNoeud(Noeud *noeud);
void liberercommodites(CellCommodite *commodites);
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);
void rechercheCreeCellCommodite(Reseau* R, Noeud* extrA, Noeud* extrB);
Reseau* reconstitueReseauListe(Chaines *C);
void ecrireReseau(Reseau *R, FILE *f);
int nb_voisins(CellNoeud* liste_noeud);
int nbLiaisons(Reseau *R);
int nbCommodites(Reseau *R);
void afficheReseauSVG(Reseau *R, char* nomInstance);
#endif

