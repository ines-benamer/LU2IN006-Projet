#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include <stdio.h>
#include "Reseau.h"

typedef struct table {
    cellnoeud** table;
    int lenght;
} TableHachage;

double cle(double x,double y);
int hachage(double cle, int lenght);

#endif