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


// on peux proposer d'autre fonction comme par exemple creer une table de hachage \(si ines veux bien sur ;) 
TableHachage* creeTableHachage(int taille){
    TableHachage* th = (TableHachage*) malloc(sizeof(TableHachage));
    th->table = (CellNoeud**) malloc(taille * sizeof(CellNoeud*));
    th->length = taille;
    return th;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    double clef = cle(x, y);
    int indice = hachage(clef, H->length);
    CellNoeud* current = H->table[indice];

    while (current){ // On parcourt la liste des noeuds pour voir si le noeud recherché existe 
        if (current->nd->x==x && current->nd->y==y) { // On trouve un noeud correspondant, on le retourne
            return current->nd;
        }
        current = current->suiv;
    }

    // En sortant du while, on n'a pas encore fait de return, donc on n'a pas trouvé de noeud correspondant
    // Alors on le crée et on l'ajoute

    Noeud* n = creeNoeud(R->nbNoeuds+1, x, y); // On crée le noeud

    // On ajoute le nouveau noeud à la table de hachage
    CellNoeud* cn_hach = creeCellNoeud(n);
    cn_hach->suiv = H->table[indice];
    H->table[indice] = cn_hach;

    // On ajoute le nouveau noeud au réseau
    R->nbNoeuds++;
    CellNoeud* cn_res = creeCellNoeud(n);
    cn_res->suiv = R->noeuds;
    R->noeuds = cn_res;

    return n;
    
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

void liberer_table_hachage(TableHachage *table ){
    if(!table->table) return;

    for(int i=0;i < table->length;i++){
        libererCellNoeuds(table->table[i],0);
    }
    free(table);
        

}