#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"

int menu(){
    printf("\nVeuillez choisir une option : \n");
    printf("0 - Quitter le programme\n");
    printf("1 - Lire un fichier\n");
    printf("2 - Ecrire le réseau dans un fichier txt\n");
    printf("3 - Créer une représentation graphique du réseau\n");
    int res;
    scanf("%d", &res);
    return res;
}

int main(){

    Reseau* reseau = NULL;
    int tourne = 1;

    while (tourne) {
        int choix = menu();
        switch(choix){

            case 0 : 
                tourne = 0;
                break;

            case 1 : 
                printf("Entrer le nom du fichier à lire\n");
                char nom_fic[70];
                scanf("%s", nom_fic);

                FILE* fichier_lu = fopen(nom_fic, "r");
                if (!fichier_lu){
                    printf("Erreur d'ouverture du fichier");
                }
                else {
                    Chaines* chaines = lectureChaines(fichier_lu);
                    reseau = reconstitueReseauListe(chaines);
                    if (reseau){
                        printf("Le fichier a bien été lu et enregistré\n");

                        FILE* ecriture_reseau = fopen("ecriture_reseau.txt", "w");
                        printf("AU SECOURS J'EN PEUX PLUS\n");
                        if (ecriture_reseau){
                            ecrireReseau(reseau, ecriture_reseau);
                            printf("Le réseau a bien été écrit dans le fichier 'ecriture_reseau.txt' \n");
                        }
                        else{
                            printf("Erreur de l'ouverture du fichier d'écriture\n");
                        }
                        fclose(ecriture_reseau);
                    }
                    else {
                        printf("Le fichier a bien été lu mais n'a pas pu être enregistré");
                    }
                    liberer_structure(chaines);
                }
                fclose(fichier_lu);
                break;

            // case 2 : 
            //     if (!reseau){
            //         printf("Aucun réseau n'existe, donc on ne peut pas en écrire. Veuillez d'abord lire un fichier.\n");
            //         break;
            //     }
            //     else {
            //         printf("rentré la oeoe");
            //         FILE* ecriture_reseau = fopen("ecriture_reseau.txt", "w");
            //         if (!ecriture_reseau){
            //             printf("Erreur de l'ouverture du fichier d'écriture\n");
            //         }
            //         else{
            //             ecrireReseau(reseau, ecriture_reseau);
            //             printf("Le réseau a bien été écrit dans le fichier 'ecriture_reseau.txt' \n");
            //         }
            //         fclose(ecriture_reseau);
            //         break;
            //     }

            case 3 :
                if (!reseau){
                    printf("Aucun reseau n'existe, on ne peut pas en créer de représentation graphique. Veuiller d'abord lire un fichier.\n");
                }

                else {
                    afficheReseauSVG(reseau, "AffichageReseau");
                    printf("La représentation graphique du réseau a été créée dans le fichier 'AffichageReseau.html' \n");
                }

                break;


            default :
                printf("Le choix entré n'est pas valide\n");
                break;

        }
    }

    liberer_reseau(reseau);
    return 0;

}

