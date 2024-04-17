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
    cellnoeud* current = H->table[indice];

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

TableHachage* chainesToHachage(Chaines*C, int M) {
    TableHachage* tableH = creeTableHachage(M);

    CellChaine* chaine_cour = C->chaines;
    CellPoint* point_cour = chaine_cour-> points;
    int num = 0;

    while (chaine_cour) {

        while (point_cour) {

            Noeud* n = creeNoeud(num, point_cour->x, point_cour->y);    
            num ++;
            CellNoeud* cn = creeCellNoeud(n);

            double cle = cle(point_cour->x, point_cour->y);
            int hachage = hachage(cle, M);

            cn->suiv = tableH[hachage];
            tableH[hachage] = cn;

            point_cour = point_cour->suiv;
        }

        chaine_cour = chaine_cour->suiv;
        if (chaine_cour) {
            point_cour = chaine_cour->points;
        }
    }

    return tableH;
}

void hachageToReseau(TableHachage* tableH, Reseau* R) {
    for (int i=0; i<tableH->length; i++) { // On parcourt toutes les cases de la table
        CellNoeud* cn = tableH->table[i];

        while (cn) { // Pour chaque case, on duplique le cellnoeud et on l'ajoute au réseau
            CellNoeud* cn_res = dupliqueCellNoeud(cn);
            cn_res->suiv = R->noeuds;
            R->noeuds = cn_res;
            cn = cn->suiv;
        }
    }
}

Reseau* reconstitueReseauHachage(Chaines* C, int M){
    Reseau* reseau = creeReseau(C->gamma);

    // On crée la table de hachage à partir de la liste chaînée de chaînes de points
    TableHachage* tableH = chainesToHachage(C, M);
    // Puis on crée le réseau à partir de la table de hachage
    hachageToReseau(tableH, reseau);

    return reseau;
}