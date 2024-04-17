#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {
    CellChaine* chaine_cour = C->chaines;
    int debut = 1;

    while(chaine_cour){
        CellPoint* point_cour = chaine_cour->points;

        while(point_cour) {

            if (debut==1){
                debut = 0;
                *xmin = point_cour->x;
                *ymin = point_cour->y;
                *xmax = point_cour->x;
                *ymax = point_cour->y;
            }
            else { // On teste les coordonnées du point courant afin de mettre à jour nos coordonnées extrêmes

                if (point_cour->x < *xmin) {
                    *xmin = point_cour->x;
                }

                if (point_cour->y < *ymin) {
                    *ymin = point_cour->y;
                }

                if (point_cour->x > *xmax) {
                    *xmax = point_cour->x;
                }

                if (point_cour->y > *ymax) {
                    *ymax = point_cour->y;
                }

            }

            point_cour = point_cour-> suiv;
        }

        chaine_cour = chaine_cour->suiv;
    }
}


ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat* arbre = (ArbreQuat*) malloc(sizeof(ArbreQuat));
    arbre->xc = xc;
    arbre->yc = yc;
    arbre->coteX = coteX;
    arbre->coteY = coteY;

    arbre->noeud = NULL;
    arbre->so = NULL;
    arbre->se = NULL;
    arbre->no = NULL;
    arbre->ne = NULL;

    return arbre;
}

char* position(double x, double y, ArbreQuat* parent){
    int s, o;

        if (y < parent->yc) { 
            s=1;
        }
        else {
            s=0;
        }

        if (x < parent->xc) {
            o=1;
        }
        else {
            o=0;
        }

        if (s==1 && o==1){
            return "so";
        }
        if (s==0 && o==0){
            return "ne";
        }
        if (s==1 && o==0){
            return "se";
        }
        else {
            return "no";
        }
}


void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {

    if (*a==NULL){ // Si on fait une insertion dans un arbre vide
        *a = creerArbreQuat(n->x, n->y, parent->coteX, parent->coteY);
        (*a)->noeud = n;

        //On détermine dans quelle branche de l'arbre parent on insère le nouvel arbre

        char* pos = position(n->x, n->y, parent);

        if (strcmp(pos, "so")==0){
            parent->so = *a;
            return;
        }
        if (strcmp(pos, "ne")==0){
            parent->ne = *a;
            return;
        }
        if (strcmp(pos, "se")==0){
            parent->se = *a;
            return;
        }
        if (strcmp(pos, "no")==0){
            parent->no = *a;
            return;
        }

    }

    if ( (*a)->noeud !=NULL ) { // Si on veut faire une insertion dans une feuille
        Noeud* n2 = (*a)->noeud;
        *a = creerArbreQuat((n->x + n2->x)/2, (n->y + n2->y)/2, parent->coteX, parent->coteY);

        ArbreQuat** a1 = (ArbreQuat**) malloc(sizeof(ArbreQuat*));
        a1 = NULL;
        ArbreQuat** a2 = (ArbreQuat**) malloc(sizeof(ArbreQuat*));
        a2 = NULL;

        insererNoeudArbre(n, a1, *a);
        insererNoeudArbre(n2, a2, *a);

        return;
    }

    if ( (*a != NULL) && ((*a)->noeud == NULL) ) { // Dans le cas d'une insertion dans une cellule interne

        char* pos = position(n->x, n->y, parent);

        if (strcmp(pos, "so")==0) {
            insererNoeudArbre(n, &(*a)->so, *a);
        }

        if (strcmp(pos, "ne")==0) {
            insererNoeudArbre(n, &(*a)->ne, *a);
            return;
        }

        if (strcmp(pos, "se")==0){
            insererNoeudArbre(n, &(*a)->se, *a);
            return;
        }

        if (strcmp(pos, "no")==0) {
            insererNoeudArbre(n, &(*a)->no, *a);
            return;
        }
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    ArbreQuat* arbre_cour = *a;
    
    while (arbre_cour) { // On parcourt les arbres niveau par niveau jusqu'à atteindre les feuilles

        if (arbre_cour->noeud==NULL){ // On est dans un noeud interne, on continue la recherche à un niveau inférieur
            char* pos = position(x, y, *a);

            if (strcmp(pos, "so")==0) {
                arbre_cour = arbre_cour->so;
            }
            if (strcmp(pos, "se")==0) {
                arbre_cour = arbre_cour->se;
            }
            if (strcmp(pos, "no")==0) {
                arbre_cour = arbre_cour->no;
            }
            if (strcmp(pos, "ne")==0) {
                arbre_cour = arbre_cour->ne;
            }

        }

        else { //On se trouve au niveau d'une feuille, on regarde si le noeud cherché existe ou pas
            if (arbre_cour->noeud->x == x  &&  arbre_cour->noeud->y == y) {
                return arbre_cour->noeud;
            }
            else {
                break;
            }
        } 

    }  
    
    //Lorsqu'on est sorti de la boucle while sans avoir fait de return ou qu'on n'est pas rentré dans le while, on n'a pas trouvé le noeud, donc on le crée et l'ajoute
    Noeud* n = creeNoeud(R->nbNoeuds+1, x, y);

    //Ajout du noeud au réseau
    CellNoeud* cn = creeCellNoeud(n);
    cn->suiv = R->noeuds;
    R->noeuds = cn;

    //Ajout du noeud à l'arbre
    insererNoeudArbre(n, a, parent);

    return n;
    
}

ArbreQuat* chainesToArbreQuat(Chaines* C) {
    // Fonction qui va nous permettre de construite un ArbreQuat à partir d'une liste chaînée Chaines*

    if (C->nbChaines==0) { return NULL; } // Si il n'y a rien dans les chaînes, on renvoie un arbre null

    // Calcul des coteX et coteY
    double xmin, xmax, ymin, ymax;
    CellChaine* chaine_cour = C->chaines;
    CellPoint* point_cour = chaine_cour->points;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;

    // Création de l'arbre
    ArbreQuat* arbre = creerArbreQuat(point_cour->x, point_cour->y, coteX, coteY);
    int num = 1;
    // Création et ajour des noeuds dans l'arbre
    while (chaine_cour) { // On parcourt les chaines 

        while (point_cour) { // Pour chaque chaiîne, on parcourt tous les points qui la composent
            Noeud* n = creeNoeud(num, point_cour->x, point_cour->y);
            num ++;
            point_cour = point_cour->suiv;

            char* pos = position(n->x, n->y, arbre); // On cherche dans quel cadran il faut insérer le noeud

            // Puis on insère le noeud dans le cadran correspondant
            if (strcmp(pos, "so")==0) {
                insererNoeudArbre(n, arbre->so, arbre);
            }
            if (strcmp(pos, "se")==0) {
                insererNoeudArbre(n, arbre->se, arbre);
            }
            if (strcmp(pos, "no")==0) {
                insererNoeudArbre(n, arbre->no, arbre);
            }
            if (strcmp(pos, "ne")==0) {
                insererNoeudArbre(n, arbre->ne, arbre);
            }

        }

        chaine_cour = chaine_cour->suiv; // On passe à la chaîne suivante
        if (chaine_cour) {
            point_cour = chaine_cour->points; // On actualise aussi le point qu'on regarde
        }
    }
}

void ArbreQuatToReseau(ArbreQuat* A, Reseau* R) {
    if (A->noeud != NULL) {
        ajouterNoeud(R, A->noeud);
        R->nbNoeuds;
    }
    else {
        ArbreQuatToReseau(A->se, R);
        ArbreQuatToReseau(A->so, R);
        ArbreQuatToReseau(A->no, R);
        ArbreQuatToReseau(A->ne, R);
    }
}

Reseau* ReconstitueReseauArbre(Chaines* C) {
    // Création du réseau
    Reseau* reseau = creeReseau(C->gamma);
    reseau->nbNoeuds = longueurTotale(C);
    reseau->commodites = NULL;

    //Création de l'arbre à partir des chaînes
    ArbreQuat* arbre = chainesToArbreQuat(C);

    // Ajout des noeuds dans le réseau à partir de l'arbre
    ArbreQuatToReseau(arbre, reseau);

    return reseau;
}