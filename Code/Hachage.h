#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Reseau.h"

typedef struct table {
    Cellnoeud** table;
    int lenght;
} TableHachage;

double cle(double x,double y);
int hachage(double cle, int lenght);
TableHachage* creeTableHachage(int taille);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);


#endif