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
    if (!arbre) {
        printf("Erreur lors de l'allocation de la mémoire\n");
        return NULL;
    }

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


int position(double x, double y, ArbreQuat* parent){
    // 1 : so, 2 : ne, 3 : se, 4 : no
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
            return 1;
        }
        if (s==0 && o==0){
            return 2;
        }
        if (s==1 && o==0){
            return 3;
        }
        else {
            return 4;
        } 
}


//-----------------------------------------------------------------//


void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent, int* i) {
    *i = *i+1;
    printf("%d je commence a inserer un noeud\n", *i);

    if (n==NULL || parent==NULL) {
        return;
    }


    if (*a==NULL){ // Si on fait une insertion dans un arbre vide, on crée l'arbre et on ajoute le noeud dedans
        printf("%d insertion dans un arbre nul\n", *i);

        double coteX = ((double)parent->coteX)/2.0;
        double coteY = ((double)parent->coteY)/2.0;

        double xc, yc;
        int pos = position(n->x, n->y, parent); // On détermine les coordonnées du centre du sous-cadre que l'on veut créer
        if (pos==1) { // au sud-ouest
            xc = (parent->xc) - ((double)parent->coteX)/4.0;
            yc = (parent->yc) - ((double)parent->coteY)/4.0;
        } 
        
        else if (pos==2) { // au nord-est
            xc = (parent->xc) + ((double)parent->coteX)/4.0;
            yc = (parent->yc) + ((double)parent->coteY)/4.0;
        } 
            
        else if (pos==3) { // au sud-est
            xc = (parent->xc) - ((double)parent->coteX)/4.0;
            yc = (parent->yc) + ((double)parent->coteY)/4.0;
        } 
            
        else { // au nord-ouest
            xc = (parent->xc) + ((double)parent->coteX)/4.0;
            yc = (parent->yc) - ((double)parent->coteY)/4.0;
        }
        

        *a = creerArbreQuat(xc, yc, coteX, coteY);
        if (!(*a)) {
            printf("Erreur de création de l'arbre\n");
            return;
        }
        (*a)->noeud = n;

        printf("%d fin d'insertion dans un arbre vide\n", *i);
        *i = *i-1;
        return;

    }

    else {
        if ( (*a)->noeud !=NULL ) { // Si on veut faire une insertion dans une feuille
            printf("%d insertion dans une feuille\n", *i);


            // On garde le noeud contenu dans l'arbre de côté et on transforme l'arbre en un noeud interne
            Noeud* n2 = (*a)->noeud;
            (*a)->noeud = NULL;
            
            // On ajoute le noeud déjà présent dans un sous-arbre de l'arbre actuel
            int pos = position(n2->x, n2->y, *a);
            printf("%d début d'insertion du noeud déjà existant: x=%f, y=%f\n", *i, n->x, n->y);

            if (pos==1) { // au sud-ouest
                insererNoeudArbre(n2, &((*a)->so), *a, i);
            } 
            
            else {
                if (pos==2) { // au nord-est
                    insererNoeudArbre(n2, &((*a)->ne), *a, i);
                } 
                
                else {
                    if (pos==3) { // au sud-est
                        insererNoeudArbre(n2, &((*a)->se), *a, i);
                    } 
                    
                    else { // au nord-ouest
                        insererNoeudArbre(n2, &((*a)->no), *a, i);
                    }
                }
            }
            printf("%d fin d'insertion du noeud déjà existant: x=%f, y=%f\n", *i, n->x, n->y);


            // On ajoute le noeud que l'on voulait ajouter dans un sous-arbre de l'arbre actuel
            pos = position(n->x, n->y, (*a));

            printf("%d début d'insertion du nouveau noeud : x=%f, y=%f\n", *i, n->x, n->y);
            if (pos==1) { // au sud-ouest
                printf("%d so\n", *i);
                insererNoeudArbre(n, &((*a)->so), (*a), i);
            } 

            else {
                if (pos==2) { // au nord-est
                printf("%d ne\n", *i);
                    insererNoeudArbre(n, &((*a)->ne), (*a), i);
                } 
                
                else {
                    if (pos==3) { // au sud-est
                    printf("%d se\n", *i);
                        insererNoeudArbre(n, &((*a)->se), (*a), i);
                    } 
                    
                    else { // au nord-ouest
                    printf("%d no\n", *i);
                        insererNoeudArbre(n, &((*a)->no), (*a), i);
                    }
                }
            }
            printf("%d fin d'insertion du nouveau noeud : x=%f, y=%f\n", *i, n->x, n->y);
            printf("%d fin d'insertion dans une feuille\n", *i);

            (*a)->noeud = NULL;
            return;

        }


        else  if ( (*a != NULL) && ((*a)->noeud == NULL) ) { // Dans le cas d'une insertion dans une cellule interne
            printf("%d insertion dans un noeud interne\n", *i);

            int pos = position(n->x, n->y, parent);

            if (pos==1) {
                insererNoeudArbre(n, &((*a)->so), *a, i);
                return;
            }

            if (pos==2) {
                insererNoeudArbre(n, &((*a)->ne), *a, i);
                return;
            }

            if (pos==3){
                insererNoeudArbre(n, &((*a)->se), *a, i);
                return;
            }

            else {
                insererNoeudArbre(n, &((*a)->no), *a, i);
                return;
            }
        
        }

    }
    
    printf("%d fin d'insertion dans un noeud interne\n", *i);
    *i = *i - 1;
}



//-----------------------------------------------------------------//


Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {

    if (!a) {
        printf("Pointeur vers un arbre non valide\n");
    }

    if (*a==NULL) { // Dans les cas où on est dans un arbre vide, on crée le noeud et on le renvoie
        Noeud* n = creeNoeud(R->nbNoeuds+1, x, y);
        R->nbNoeuds++;
        int i=0;
        insererNoeudArbre(n, a, parent, &i);

    }



    ArbreQuat* arbre_cour = *a;
    
    while (arbre_cour) { // On parcourt les arbres niveau par niveau jusqu'à atteindre les feuilles

        if (arbre_cour==NULL) {
            break;
        }

        if (arbre_cour->noeud==NULL){ // On est dans un noeud interne, on continue la recherche à un niveau inférieur
            int pos = position(x, y, *a);

            if (pos==1) {
                arbre_cour = arbre_cour->so;
            }
            if (pos==2) {
                arbre_cour = arbre_cour->ne;
            }
            if (pos==3) {
                arbre_cour = arbre_cour->se;
            }
            else {
                arbre_cour = arbre_cour->no;
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
    
    // Si on passe par la, cela signifie qu'on n'a pas trouvé le noeud, donc on le crée et l'ajoute au réseau et à l'arbre
    Noeud* n = creeNoeud(R->nbNoeuds+1, x, y);

    //Ajout du noeud à l'arbre
        int truc = 0;
        insererNoeudArbre(n, a, parent, &truc);

    //Ajout du noeud au réseau
    CellNoeud* cn = creeCellNoeud(n);
    cn->suiv = R->noeuds;
    R->noeuds = cn;

    return n;
    
}


//-----------------------------------------------------------------//


Reseau* ReconstitueReseauArbre(Chaines* C) {

    if (!C) {
        printf("Pointeur Chaines* invalide\n");
        return NULL;
    }

    Reseau* reseau = creeReseau(C->gamma);
    if (!reseau) {
        printf("Erreur de création du réseau\n");
        return NULL;
    }

    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double xc = ((double)(xmax + xmin))/2.0;
    double yc = ((double)(ymax + ymin))/2.0;
    double coteX = (double) (xmax - xmin);
    double coteY = (double) (ymax - ymin);

    ArbreQuat* arbre = creerArbreQuat(xc, yc, coteX, coteY);
    if (!arbre) {
        printf("Erreur de création de l'arbre\n");
        return NULL;
    }

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



            // La position du point que l'on veut créer par rapport au centre de l'arbre, ("so", "se", "no" ou "ne")           
            int pos = position(x, y, arbre);

            if (!extrA) { // Si on n'a pas encore rencontré de noeud dans la chaîne, on définit la première extrémité
                // Puis on l'insère 
                if (pos==1) {
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->so), arbre, x, y);
                }
                if (pos==2) {
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->ne), arbre, x, y);
                }
                if (pos==3) {
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->se), arbre, x, y);
                }
                else {
                    extrA = rechercheCreeNoeudArbre(reseau, &(arbre->no), arbre, x, y);
                }
            }

            // On met ensuite à jour la deuxième extremité de la commodité
            // En ce faisant, on crée et ajoute le noeud à l'arbre et au réseau
            // De plus, s'ils ne sont pas déjà voisins, on rend le noeud courant et précédent voisins
            if (pos==1) {
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->so), arbre, x, y);
                rendreVoisins(V, extrB);
                V = extrB;
            }
            if (pos==2) {
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->ne), arbre, x, y);
                rendreVoisins(V, extrB);
                V = extrB;
            }
            if (pos==3) {
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->se), arbre, x, y);
                rendreVoisins(V, extrB);
                V = extrB;
            }
            else {
                extrB = rechercheCreeNoeudArbre(reseau, &(arbre->no), arbre, x, y);
                rendreVoisins(V, extrB);
                V = extrB;
            }

            // On ajoute la commodité au réseau
            if ( extrA!=NULL && extrB!=NULL && extrA != extrB) { // Si la commodité à une longueur strictement supérieure à 1
                rechercheCreeCellCommodite(reseau, extrA, extrB);
            }
            current_point = current_point->suiv;

        }
        current_chaine = current_chaine->suiv;

    }

    libereArbreQuat(arbre);
    return reseau;

}

void libereArbreQuat(ArbreQuat * a) {

    if (a == NULL) {
        return ;
    }

    if (a -> noeud != NULL) {
        free(a);
        return ;
    }

    libereArbreQuat(a -> so);
    libereArbreQuat(a -> se);
    libereArbreQuat(a -> no);
    libereArbreQuat(a -> ne);
    free(a);
}