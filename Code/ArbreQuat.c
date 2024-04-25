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


//-----------------------------------------------------------------//


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

//-----------------------------------------------------------------//


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


//-----------------------------------------------------------------//


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


//-----------------------------------------------------------------//


Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    ArbreQuat* arbre_cour = *a;
    printf("hey\n");
    
    while (arbre_cour) { // On parcourt les arbres niveau par niveau jusqu'à atteindre les feuilles

        if (arbre_cour==NULL) {
            break;
        }

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

        if (arbre_cour==NULL) {
            break;
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
    
    // Si on passe par la, cela signifie qu'on n'a pas trouvé le noeud, donc on le crée et l'ajouter au réseau et à l'arbre
    Noeud* n = creeNoeud(R->nbNoeuds+1, x, y);

    //Ajout du noeud au réseau
    CellNoeud* cn = creeCellNoeud(n);
    cn->suiv = R->noeuds;
    R->noeuds = cn;

    //Ajout du noeud à l'arbre
    insererNoeudArbre(n, a, parent);

    return n;
    
}


//-----------------------------------------------------------------//


Reseau* ReconstitueReseauArbre(Chaines* C) {
    Reseau* reseau = creeReseau(C->gamma);

    ArbreQuat* arbre = NULL;

    Noeud* extrA;
    Noeud* extrB;
    Noeud* V;

    CellChaine* current_chaine = C->chaines;

    while (current_chaine) {

        if (!current_chaine->points) {
            continue;
        }

        CellPoint* current_point = current_chaine->points;
        extrA = NULL;
        extrB = NULL;
        V = NULL;

        while (current_point) { // On parcourt tous les points de la chaîne

            double x = current_point->x;
            double y = current_point->y;

            if (arbre==NULL){ // Si l'arbre n'existe pas encore, on le crée
                // Les coordonnées du centre de l'arbre sont celles du premier point rencontré (ce dernier sera donc placé au ne)
                double xmin, xmax, ymin, ymax;  
                chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
                double coteX = xmax - xmin;
                double coteY = ymax - ymin;
                arbre = creerArbreQuat(x, y, coteX, coteY);
            }

            // La position du point que l'on veut créer par rapport au centre de l'arbre, ("so", "se", "no" ou "ne")           
            char* pos = position(x, y, arbre);

            if (!extrA) { // Si on n'a pas encore rencontré de noeud dans la chaîne, on définit la première extrémité
                // Puis on l'insère 
                if (strcmp(pos, "so")==0) {
                    printf("bro1.1\n");
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->so), arbre, x, y);
                    printf("bro1.2\n");
                }
                if (strcmp(pos, "se")==0) {
                    printf("bro2.1\n");
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->se), arbre, x, y);
                    printf("bro2.2\n");
                }
                if (strcmp(pos, "no")==0) {
                    printf("bro3.1\n");
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->no), arbre, x, y);
                    printf("bro3.2\n");
                }
                else {
                    printf("bro4.1\n");
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->ne), arbre, x, y);
                    printf("bro4.2\n");
                }
            }

            // On met ensuite à jour la deuxième extremité de la commodité
            // En ce faisant, on crée et ajoute le noeud à l'arbre et au réseau
            // De plus, s'ils ne sont pas déjà voisins, on rend le noeud courant et précédent voisins
            if (strcmp(pos, "so")==0) {
                printf("on est la\n");
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->so), arbre, x, y);
                printf("on est sorti de la\n");
                rendreVoisins(V, extrB);
                V = extrB;
            }
            if (strcmp(pos, "se")==0) {
                printf("on est la2\n");
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->se), arbre, x, y);
                printf("on est sorti de la2\n");
                rendreVoisins(V, extrB);
                V = extrB;
            }
            if (strcmp(pos, "no")==0) {
                printf("on est la3\n");
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->no), arbre, x, y);
                printf("on est sorti de la3\n");
                rendreVoisins(V, extrB);
                V = extrB;
            }
            else {
                printf("on est la4\n");
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->ne), arbre, x, y);
                printf("on est sorti de la4\n");
                rendreVoisins(V, extrB);
                V = extrB;
            }

            // On ajoute la commodité au réseau
            if (extrA != extrB) { // Si la commodité à une longueur strictement supérieure à 1
                rechercheCreeCellCommodite(reseau, extrA, extrB);
            }

            current_point = current_point->suiv;

        }

        current_chaine = current_chaine->suiv;

    }

    return reseau;

}