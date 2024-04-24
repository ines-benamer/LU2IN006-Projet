#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Hachage.h"
#include "Reseau.h"

//question 4.2 :

double cle(double x,double y){
    return y + (x + y) * (x + y + 1) / 2;
}
//pour répondre a cette question (Est-ce que la fonction clef vous semble appropriee ?) il faut d'abord vérifier  la fonction de hachage et les collisions
// En testant cette fonction avec différentes valeurs de x et y
// On constate que les valeurs retournées par la fonction clé semblent assez bien réparties de 0 à 100

//---------------------------------------------
//question 4.3 :

int hachage(double cle, int lenght) {
    float a = (sqrt(5) - 1) / 2.0;
    double cleH = cle * a;
    long cle_int = (long)cleH;

    return (int)(lenght * (cleH - cle_int));
}


// on peux proposer d'autre fonction comme par exemple creer une table de hachage (si ines veux bien sur ;) )
TableHachage* creeTableHachage(int taille){
    TableHachage* th = (TableHachage*) malloc(sizeof(TableHachage));
    th->table = (cellnoeud**) malloc(taille * sizeof(cellnoeud*));
    th->length = taille;
    return th;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    double cle = cle(x, y);
    int indice = hachage(cle, H->length);
    int trouve = 0;
    CellNoeud* current = H->table[indice];

    while (current){ // On parcourt la liste des noeuds pour voir si le noeud recherché existe 
        if (current->nd->x==x && current->nd->y==y) {
            trouve = 1;
            return current->nd;
        }
        current = current->suiv;
    }
    if (trouve==0){ // On est ici dans le cas où le noeud n'existait pas dans le réseau

        Noeud* new = creeNoeud(R->nbNoeuds+1, x, y); // On crée le noeud

        // On ajoute le nouveau noeud à la table de hachage
        new->suiv = H->table[indice];
        h->table[indice] = new;

        // On ajoute le nouveau noeud au réseau
        R->nbNoeuds++;
        CellNoeud* new_c = creeCellNoeud(new);
        new_c->suiv = R->noeuds;
        R->noeuds = new_c
    }
}


Reseau* reconstitueReseauHachage(Chaines* C, int M){
    Reseau* reseau = creeReseau(C->gamma);
    TableHachage* table = creeTableHachage(M);

    CellChaine* current_chaine = C->chaines;

    Noeud* extrA;
    Noeud* extrB;
    Noeud* V;

    while (current_chaine) {

        if (!(current_chaine->points)) {
            continue;
        }

        CellPoint* current_point = current_chaine->points;
        extrA = NULL;
        extrB = NULL;
        V = NULL;

        while (current_point) {
            if (!extrA) {
                extrA = rechercheCreeNoeudHachage(reseau, table, current_point->x, current_point->y);
            }

            // On ajoute le point courant au réseau et à la table de hachage s'il n'existe pas
            // Et on indique le dernier point de la chaîne comme étant ce point
            extrB = rechercheCreeNoeudHachage(reseau, table, current_point->x, current_point->y); 

            // On ajoute le point courant aux voisins du point précédent et inversement si ils ne sont pas déjà voisins
            rendreVoisins(V, extrB);
            // On modifie le point 
            V = extrB;

            current_point = current_point->suiv;
        }   

        if (extrA!=extrB) { // Si les deux extremités ne sont pas les mêmes (donc si la commodité n'est pas vide ou à un seul point)
            rechercheCreeCellCommodite(reseau, extrA, extrB);
        }

        current_chaine = current_chaine->suiv;

    }

    return reseau;

}