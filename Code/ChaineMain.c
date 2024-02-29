#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Chaine.h"
#include "SVGwriter.h"

int main(int argc, char *argv)
{

    if (argc != 2)
    {
        printf("Usage : <nom du fichier à lire>");
    }
    FILE *f = fopen(argv[1], "r");

    Chaines *fic_lu = lectureChaines(f);

    FILE *f2 = fopen("test_ecriture.txt", "r");
    ecrireChaines(f2);

    afficheChainesSVG(fic_lu, "TestAffichage");

    return 0;
}
