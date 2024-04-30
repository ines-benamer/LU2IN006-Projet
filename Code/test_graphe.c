#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Graphe.h"
#include "Struct_File.h"

//c'est un brouillon pour tester les fonctions 
int main() {
   
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
    if (!reseau) {
        printf("Erreur d'allocation mémoire pour le réseau.\n");
        return 1;
    }

    
    reseau->gamma = 2;
    reseau->nbNoeuds = 3;
    reseau->noeuds = NULL;


    Graphe* graphe = creerGraphe(reseau);


    if (!graphe) {
        printf("Erreur lors de la création du graphe.\n");
        free(reseau);
        return 1;
    }


    int u = 0; 
    int v = 20; 
    int nb_aretes = plus_petit_nb_aretes(graphe, u, v);
    printf ("le nb d'aretes = %d\n", nb_aretes);
    if (nb_aretes >= 0) {
        printf("Le plus petit nombre d'arêtes entre les sommets %d et %d est : %d\n", u+1, v+1, nb_aretes);
    } else {
        printf("Erreur lors du calcul du nombre d'arêtes entre les sommets %d et %d.\n", u+1, v+1);
    }


    liberer_graphe(graphe);
    free(reseau);

   

    return 0;
}
