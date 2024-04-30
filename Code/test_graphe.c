#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Graphe.h"
#include "Struct_File.h"

//c'est un brouillon pour tester les fonctions 
int main() {
    // Création d'un réseau de test (à remplacer avec un vrai réseau)
    Reseau* reseau = creeReseau(5); // Exemple de création de réseau, à remplacer avec l'appel réel

    // Création du graphe à partir du réseau
    Graphe* graphe = creerGraphe(reseau);

    // Affichage des informations sur le graphe
    if (graphe != NULL) {
        printf("Graphe cree avec succes :\n");
        printf("Nombre de sommets : %d\n", graphe->nbsom);
        printf("Gamma : %d\n", graphe->gamma);
        printf("Nombre de commodites : %d\n", graphe->nbcommod);

        // Affichage des sommets
        printf("Sommets :\n");
        for (int i = 0; i < graphe->nbsom; i++) {
            Sommet* sommet = graphe->T_som[i];
            printf("Sommet %d : x=%f, y=%f\n", sommet->num, sommet->x, sommet->y);
        }

        // Affichage des commodites
        printf("Commodites :\n");
        for (int i = 0; i < graphe->nbcommod; i++) {
            Commod commodite = graphe->T_commod[i];
            printf("Commodite %d : e1=%d, e2=%d\n", i+1, commodite.e1, commodite.e2);
        }
    } else {
        printf("Erreur lors de la creation du graphe.\n");
    }

    return 0;
}