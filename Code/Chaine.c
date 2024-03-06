#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"
// Question 1 :
Chaines *lectureChaines(FILE *f)
{
    Chaines *chaine = (Chaines *)malloc(sizeof(Chaines));
    fscanf(f, "NbChain: %d\n", &(chaine->nbChaines));
    fscanf(f, "Gamma: %d\n", &(chaine->gamma));

    chaine->chaines = NULL;

    for (int i = 0; i < chaine->nbChaines; i++)
    {
        CellChaine *nouvelleChaine = (CellChaine *)malloc(sizeof(CellChaine));
        if (!nouvelleChaine)
        {
            fprintf(stderr, "Erreure d'allocation de mémoire\n");
            return NULL;
        }
        fscanf(f, "%d", &(nouvelleChaine->numero));
        nouvelleChaine->points = NULL;

        int nbpoints;
        fscanf(f, "%d", &nbpoints);
        for (int j = 0; j < nbpoints; j++)
        {
            CellPoint *nouveauPoint = (CellPoint *)malloc(sizeof(CellPoint));
            if (!nouveauPoint)
            {
                fprintf(stderr, "erreure d'allocation de mémoire");
                return NULL;
            }
            fscanf(f, "%lf %lf", &(nouveauPoint->x), &(nouveauPoint->y));
            nouveauPoint->suiv = nouvelleChaine->points;
            nouvelleChaine->points = nouveauPoint;
        }

        nouvelleChaine->suiv = chaine->chaines;
        chaine->chaines = nouvelleChaine;
    }

    return chaine;
}

// Question 2

void ecrireChaines(Chaines *C, FILE *f)
{

    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);
    CellChaine *chaine_cour = C->chaines;

    char tmp[20];

    while (chaine_cour)
    {
        fprintf(f, "%d ", chaine_cour->numero);
        int compteur = 0;
        char str[] = ""; // Chaîne de caractères temporaire dans laquelle on va stocker les points de la ligne
        CellPoint *point_cour = chaine_cour->points;

        while (point_cour)
        { // On parcourt la liste chaînée de points en
            snprintf(tmp, 10, "%f", point_cour->x);
            strcat(str, tmp);
            strcat(str, " ");
            snprintf(tmp, 10, "%f", point_cour->y);
            strcat(str, tmp);
            strcat(str, " ");
            compteur++;
            point_cour = point_cour->suiv;
        }
        fprintf(f, "%d %s\n", compteur, str);
        chaine_cour = chaine_cour->suiv;
    }
}

void afficheChainesSVG(Chaines *C, char *nomInstance)
{
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx, precy;
    SVGwriter svg;
    ccour = C->chaines;
    while (ccour != NULL)
    {
        pcour = ccour->points;
        while (pcour != NULL)
        {
            if (maxx < pcour->x)
                maxx = pcour->x;
            if (maxy < pcour->y)
                maxy = pcour->y;
            if (minx > pcour->x)
                minx = pcour->x;
            if (miny > pcour->y)
                miny = pcour->y;
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVGinit(&svg, nomInstance, 500, 500);
    ccour = C->chaines;
    while (ccour != NULL)
    {
        pcour = ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
        precx = pcour->x;
        precy = pcour->y;
        pcour = pcour->suiv;
        while (pcour != NULL)
        {
            SVGline(&svg, 500 * (precx - minx) / (maxx - minx), 500 * (precy - miny) / (maxy - miny), 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            SVGpoint(&svg, 500 * (pcour->x - minx) / (maxx - minx), 500 * (pcour->y - miny) / (maxy - miny));
            precx = pcour->x;
            precy = pcour->y;
            pcour = pcour->suiv;
        }
        ccour = ccour->suiv;
    }
    SVGfinalize(&svg);
}

// question 4 :

double longueurChaine(CellChaine *c)
{
    CellPoint *points_1 = NULL;
    double longueur = 0;
    CellPoint *points_2;
    for (points_2 = c->points; points_2; points_2 = points_2->suiv)
    {
        double distance = sqrt(pow((points_2->x - points_1->x), 2) + pow((points_2->y - points_1->y), 2));
        longueur += distance;
        points_1 = points_2;
    }
    return longueur;
}

double longueurTotale(Chaines *C)
{
    double total = 0;

    CellChaine *chaine = C->chaines;

    while (chaine)
    {
        total += longueurChaine(chaine);
        chaine = chaine->suiv;
    }
    return total;
}