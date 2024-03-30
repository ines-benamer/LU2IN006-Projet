#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "Chaine.h"

int main(int argc, char *argv[]){
    if(argc != 3) {
         fprintf(stderr, "Usage: %s <fichier.cha> <methode>\n", argv[0]);
    }
    return 1 ;

    // Récupérer le nombre de choix
    int nb_choix = atoi(argv[2]);

    // Vérifier si le nombre de choix est valide
    if (nb_choix < 1 || nb_choix > 3) {
        fprintf(stderr, "Le nombre de choix n'est pas valide\n");
        return 1;
    }
    char *name = argv[0];
    FILE *fichier = fopen(argv[1], "r");
    if (!fichier) {
        fprintf(stderr, "Le fichier %s n'existe pas\n", name);
        return 1;
    }

    Chaines *chaines = lectureChaines(fichier);
    if (!chaines) {
        fclose(fichier);
        return 1;
    }

    //pour les liste : 
    if (nb_choix == 1) {
        Reseau *reseau = reconstitueReseauListe(chaines);

        if (!reseau) {
            liberer_structure(chaines);
            fclose(fichier);
            return 1;
        }

        FILE *f = fopen("teste_ecriture_chaines.txt", "w");
        if (f) {
            ecrireReseau(reseau, f);
            fclose(f);
        }

        liberer_structure(chaines);
        liberer_reseau(reseau);
        
    }







}

