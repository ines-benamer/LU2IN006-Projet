// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include "ArbreQuat.h"
#include "Hachage.h"


int main(int argc, char *argv[]) {
    // On test les arguments
    if (argc != 3) {
        printf("Usage: %s <filename>"
                " <number : Le choix de la structure> \n\t- 1 pour les listes \n\t"
                "- 2 pour la table de hachage \n\t- 3 pour les arbres\n",
                argv[0]);
        return 1;
    }

    // On supprime le nom du fichier main des arguments
   
    int number = atoi(argv[2]);

    // On test le nombre si il est valide
    if (number < 0 || number > 3) {
        fprintf(stderr, "Le nombre n'est pas valide\n");
        return 1;
    }

    // On ouvre le fichier
    char *filename = argv[0];
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Le fichier %s n'existe pas\n", filename);
        return 1;
    }

    // On lit les chaines depuis le fichier
    Chaines *chaines = lectureChaines(file);
    if (!chaines) {
        fclose(file);
        return 1;
    }
    Reseau *reseau;
    // Premiere partie : Avec les liste
    if (number == 1) {
        // On reconstruit le reseau
        reseau = reconstitueReseauListe(chaines);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
        FILE *f = fopen("teste_ecriture_chaines.txt", "w");
        if (f) {
            ecrireReseau(reseau, f);
            fclose(f);
        }
    }

    // Deuxieme partie : Avec la table de hachage
    if (number == 2) {
        // On crée la table de hachage
        TableHachage *table = creeTableHachage(50);
        if (!table) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On reconstruit le reseau
        reseau = reconstitueReseauHachage(chaines, table->length);

       liberer_table_hachage(table);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
        FILE *f = fopen("Reseaureconstituer.txt", "w");
        if (f) {
            ecrireReseau(reseau, f);
            fclose(f);
        }
    }
    // Troisiéme partie : Avec les arbres
    if (number == 3) {
        double xmax, xmin, ymax, ymin;
        int cote_x, cote_y;

        // On crée l'arbre
        chaineCoordMinMax(chaines, &xmin, &ymin, &xmax, &ymax);

        cote_x = xmax - xmin;
        cote_y = ymax - ymin;

        ArbreQuat *arbre = creerArbreQuat((xmax + xmin) / 2, (ymax + ymin) / 2, cote_x, cote_y);

        if (!arbre) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On reconstruit le reseau
        reseau = ReconstitueReseauArbre(chaines);

        libereArbreQuat(arbre);

        // Si on a eu une erreur de reconstitution
        if (!reseau) {
            liberer_structure(chaines);
            fclose(file);
            return 1;
        }

        // On crée le fichier de sortie : Ou on va ecrire le reseau reconstruit
        FILE *f = fopen("ReconstitutionArbre.txt", "w");
        if (f) {
            ecrireReseau(reseau, f);
            fclose(f);
        }
    }

    afficheReseauSVG(reseau, "SVG_reseau");
    liberer_structure(chaines);
    libererReseau(reseau);

    fclose(file);
    return 0;
}






 