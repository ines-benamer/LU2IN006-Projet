#include "Hachage.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//question 4.2 :

double cle(double x,double y){
    return y + (x + y) * (x + y + 1) / 2;
}
//pour répondre a cette question (Est-ce que la fonction clef vous semble appropriee ?) il faut d'abord vérifier  la fonction de hachage et les collisions

//---------------------------------------------
//question 4.3 :

int hachage(double cle, int lenght) {
    float a = (sqrt(5) - 1) / 2.0;
    double cleH = cle * a;
    long cle_int = (long)cleH;

    return (int)(lenght * (cleH - cle_int));
}
// on peux proposer d'autre fonction comme par exemple creer une table de hachage (si ines veux bien sur ;) )