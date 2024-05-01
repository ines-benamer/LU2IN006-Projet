// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#ifndef __CHAINE_H__
#define __CHAINE_H__
#include <stdio.h>
#include <math.h>



/* Liste chainee de points */
typedef struct cellPoint
{
  double x, y;            /* Coordonnees du point */
  struct cellPoint *suiv; /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine
{
  int numero;              /* Numero de la chaine */
  CellPoint *points;       /* Liste des points de la chaine */
  struct cellChaine *suiv; /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct
{
  int gamma;           /* Nombre maximal de fibres par cable */
  int nbChaines;       /* Nombre de chaines */
  CellChaine *chaines; /* La liste chainée des chaines */
} Chaines;

Chaines *lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char *nomInstance);
double longueurChaine(CellChaine *c);
double longueurTotale(Chaines *C);
int comptePointsChaine(CellChaine *C);
int comptePointsTotal(Chaines *C);
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax);
void libere_point(CellPoint *point);
void liberer_liste_points(CellPoint *liste);
void librer_chaine(CellChaine *chaine);
void liberer_liste_chaines(CellChaine *liste);
void liberer_structure(Chaines *structt);
#endif