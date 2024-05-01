// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Chaine.h"
#include "SVGwriter.h"

void menu(){

    printf("0 - Quitter\n");
    printf("1 - Lire des chaînes dans un fichier\n");
    printf("2 - Ecrire des chaînes dans un fichier");

}

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        printf("Usage : <nom du fichier à lire>\n");
    }

    FILE *f = fopen(argv[1], "r");

    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }  

    Chaines *fic_lu = lectureChaines(f);


    FILE *f2 = fopen("test_ecriture_chaines.txt", "w");
    if (f2==NULL){
        printf("Erreur d'ouverture du fichier\n");
    }

    ecrireChaines(fic_lu, f2);

    afficheChainesSVG(fic_lu, "AffichageChaines");

    liberer_structure(fic_lu);

    fclose(f);

    return 0;
}
