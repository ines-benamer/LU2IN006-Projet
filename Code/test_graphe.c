#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Graphe.h"
#include "Struct_File.h"

//c'est un brouillon pour tester les fonctions 
int main() {
    // Création d'un réseau fictif pour tester
    int gamma = 2;
    Reseau* reseau = creeReseau(gamma);
    if (!reseau) {
        printf("Erreur lors de la création du réseau.\n");
        return 1;
    }

    // Appel de la fonction pour créer le graphe à partir du réseau
    Graphe* graphe = creerGraphe(reseau);
    if (!graphe) {
        printf("Erreur lors de la création du graphe.\n");
        return 1;
    }

    // Affichage de quelques informations du graphe pour vérification
    printf("Nombre de sommets dans le graphe : %d\n", graphe->nbsom);
    printf("Nombre de commodités dans le graphe : %d\n", graphe->nbcommod);
    printf("Gamma du graphe : %d\n", graphe->gamma);

    // Libération de la mémoire allouée pour le réseau et le graphe
    // (Vous devez définir vos propres fonctions pour libérer la mémoire)
    // libérer_reseau(reseau);
    // liberer_graphe(graphe);

    return 0;
}