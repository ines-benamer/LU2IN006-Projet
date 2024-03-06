#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Chaine.h"
#include "SVGwriter.h"

int main(int argc, char* argv[])
{
    printf("here\n");
    printf("%d\n", argc);

    if (argc != 2)
    {
        printf("Usage : <nom du fichier à lire>\n");
    }

    printf("there\n");
    printf("%s\n", argv[1]);
    FILE *f = fopen(argv[1], "r");

    if (f==NULL){
        printf("Erreur lors de l'ouverture du fichier\n");
        return 0;
    }

    

    Chaines *fic_lu = lectureChaines(f);
    Chaines *C = (Chaines *)malloc(sizeof(Chaines));
    FILE *f2 = fopen("test_ecriture.txt", "r");
    ecrireChaines(C, f2);

    afficheChainesSVG(fic_lu, "TestAffichage");

    fclose(f);

    return 0;
}
