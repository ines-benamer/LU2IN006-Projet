// OULD BOUALI Louisa
// BENAMER BELKACEM Inès
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Reseau.h"
#include "SVGwriter.h"

Noeud* creeNoeud(int num, int x, int y) {
    Noeud* n = (Noeud*) malloc(sizeof(Noeud));
    n->num = num;
    n->x = x;
    n->y = y;
    n->voisins = NULL;
    return n;
}

//-----------------------------------------------------------------//

CellCommodite* creeCommodite(Noeud* a, Noeud* b){
    CellCommodite* com = (CellCommodite*) malloc(sizeof(CellCommodite));
    com->extrA = a;
    com->extrB = b;
    com->suiv = NULL;
    return com;
}

//-----------------------------------------------------------------//

Reseau* creeReseau(int gamma){
    Reseau *reseau = malloc(sizeof(Reseau));
    reseau->gamma = gamma;
    reseau->commodites = NULL;
    reseau->nbNoeuds = 0;
    reseau->noeuds = NULL;
    return reseau;
}

//-----------------------------------------------------------------//

CellNoeud *creeCellNoeud(Noeud *noeud) {
    CellNoeud *cell = (CellNoeud *)malloc(sizeof(CellNoeud));
    cell->nd = noeud;
    cell->suiv = NULL;
    return cell;
}


//-----------------------------------------------------------------//


CellNoeud* dupliqueCellNoeud(CellNoeud* cn) {
    CellNoeud* new = creeCellNoeud(cn->nd);
    return new;
}


//-----------------------------------------------------------------//


void ajouterNoeud(Reseau* R, Noeud* n) {
    CellNoeud* cn = creeCellNoeud(n);
    cn->suiv = R->noeuds;
    R->noeuds = cn;
}


//-----------------------------------------------------------------//


void ajouteVoisin(Noeud* n1, Noeud* n2) {
    // Vérifier si ils sont déjà voisins
    CellNoeud* voisin = n1->voisins;
    while (voisin) {
        if (voisin->nd==n2) {
            return;
        }
        voisin = voisin->suiv;
    }

    // On crée de CellNoeud à partir de n1 et n2 pour pouvoir les ajouter en tant que voisins.
    CellNoeud* cn1 = creeCellNoeud(n2);

    //Les voisins de n1 sont ses voisins actuels auquels on ajoute le CellNoeud issu de n2
    cn1->suiv = n1->voisins;
    n1->voisins = cn1;
}


//-----------------------------------------------------------------//


void rendreVoisins(Noeud* n1, Noeud* n2) {

    if (n1==NULL || n2==NULL) { return; }

    ajouteVoisin(n1, n2);
    ajouteVoisin(n2, n1);

}


//-----------------------------------------------------------------//


void libererReseau(Reseau *reseau) {
    if (!reseau) return;
    // Libération des commodités
    CellCommodite *commodite = reseau->commodites;
    while (commodite != NULL) {
        CellCommodite *suivant = commodite->suiv;
        free(commodite);
        commodite = suivant;
    }

    // Libération des noeuds
    CellNoeud *noeud = reseau->noeuds;
    while (noeud != NULL) {
        CellNoeud *suivant = noeud->suiv;
        free(noeud);
        noeud = suivant;
    }
    free(reseau);

}


//-----------------------------------------------------------------//

void libererCellNoeuds(CellNoeud *cells, int rm) {
    CellNoeud *tmp = NULL;

    // On parcours la liste
    while (cells) {
        // Si c'est une suppresion forte on supprime les noeud
        if (rm)
            libereNoeud(cells->nd);

        // On suppime le cell noeud
        tmp = cells->suiv;

        free(cells);
        cells = tmp;
    }
}

//-----------------------------------------------------------------//

void libereNoeud(Noeud *noeud){
    if(!noeud) return;
    libererCellNoeuds(noeud->voisins,0);
    free(noeud);
}
//-----------------------------------------------------------------//

void libererCommodites(CellCommodite *commodites){
    CellCommodite *ptr = NULL;
    while(commodites){
        ptr = commodites->suiv;
        free(commodites);
        commodites = ptr;
    }
}

//-----------------------------------------------------------------//


Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    int trouve = 0;
    CellNoeud* cn = R->noeuds;
    while (cn) {
        if ( (cn->nd->x==x) && (cn->nd->y==y)){
            trouve = 1;
            return cn->nd;
        }
        cn = cn->suiv;
    }
    if (!trouve) { // Alors on crée un nouveau noeud et on l'ajoute au réseau
        Noeud* n = creeNoeud(R->nbNoeuds+1, x, y);
        R->nbNoeuds ++;
        CellNoeud* new_c = creeCellNoeud(n);
        new_c->suiv = R->noeuds;
        R->noeuds = new_c;
        return n;
    }
    return NULL;
}


//-----------------------------------------------------------------//


void rechercheCreeCellCommodite(Reseau* R, Noeud* extrA, Noeud* extrB) {
    CellCommodite* current_commodite = R->commodites;

    while (current_commodite) { // On parcourt les commodités
        // S'il existe une commodité qui a pour extremités les deux noeuds en paramètre, on la renvoie
        if ( (current_commodite->extrA == extrA && current_commodite->extrB==extrB) || (current_commodite->extrA == extrB && current_commodite->extrB == extrA) ){
            return;
        }
        current_commodite = current_commodite->suiv;
    }

    // On n'a pas trouvé de commodité dont les deux extrémités sont les deux passés en paramètre
    // Alors on crée cette commodité et on l'ajoute à la liste des commodités du réseau
    CellCommodite* new = creeCommodite(extrA, extrB);
    new->suiv = R->commodites;
    R->commodites = new;
}


//-----------------------------------------------------------------//


Reseau* reconstitueReseauListe(Chaines *C){
    // Création du réseau à partir du paramètre gamma de la structure Chaines
    Reseau *reseau = creeReseau(C->gamma);
    
    // Parcours de chaque chaîne dans la liste de chaînes
    for(CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        // Initialisation des variables pour le premier et le deuxième nœud de la chaîne
        Noeud *premier = NULL;
        Noeud *second = NULL;
        
        // Parcours de chaque point dans la chaîne
        for(CellPoint *point = chaine->points; point; point = point->suiv) {
            // Recherche ou création du nœud correspondant aux coordonnées du point
            Noeud *noeud = rechercheCreeNoeudListe(reseau, point->x, point->y);
            
            // Si premier n'est pas encore défini, assigne le nœud actuel à premier
            if (!premier) {
                premier = noeud;
            }

            // Si le deuxième nœud existe déjà, vérifie si une liaison entre les deux nœuds existe déjà
            if (second) {
                int existe = 0;
                // Parcours des voisins du nœud actuel pour vérifier l'existence de la liaison
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == second->num) {
                        existe = 1;
                        break;
                    }
                }
                
                // Si la liaison existe déjà, passe au prochain point
                if (existe) {
                    second = noeud;
                    continue;
                }

                // Création des cellules de nœud pour les liens entre les nœuds actuel et précédent
                CellNoeud *noeud_voisin = creeCellNoeud(second);
                CellNoeud *noeud_courant = creeCellNoeud(noeud);
                
                // Vérification de l'allocation de mémoire pour les cellules de nœud
                if (!noeud_voisin || !noeud_courant) {
                    // Libération de la mémoire allouée pour le réseau en cas d'échec
                    libererReseau(reseau);
                    // Libération de la mémoire allouée pour la cellule de nœud non utilisée
                    free(noeud_voisin);
                    return NULL;
                }
                
                // Ajout du nœud voisin à la liste des voisins du nœud courant
                noeud_voisin->suiv = noeud->voisins;
                noeud->voisins = noeud_voisin;

                // Ajout du nœud courant à la liste des voisins du nœud précédent
                noeud_courant->suiv = second->voisins;
                second->voisins = noeud_courant;
            }
            
            // Conservation du dernier nœud
            second = noeud;
        }

        // Création d'une commodité entre le premier et le dernier nœud de la chaîne, si le premier nœud est défini
        if (premier) {
            CellCommodite *cmd = creeCommodite(premier, second);
            if (!cmd) {
                libererReseau(reseau);
                return NULL;
            }
            // Ajout de la commodité en tête de la liste des commodités du réseau
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    // Retour du réseau reconstitué
    return reseau;
}


//exercice 3 :

//-----------------------------------------------------------------//


int nb_voisins(CellNoeud* liste_noeud){
    int nb = 0 ;
    for(;liste_noeud;liste_noeud = liste_noeud->suiv,nb++){
        continue;
    }
    return nb;
}


//-----------------------------------------------------------------//


int nbLiaisons(Reseau *R){
    if(! R) return 0;
    int nb = 0;
    for(CellNoeud* noeud = R->noeuds;noeud; nb+= nb_voisins(noeud->nd->voisins),noeud = noeud->suiv){
        continue;
    }
    return nb /2 ; //pour eliminer les double liaison 
}


//-----------------------------------------------------------------//


int nbCommodites(Reseau *R){
    if(!R)return 0;
    int nb =0 ;

    for(CellCommodite* cmd = R->commodites;cmd;cmd = cmd->suiv,nb++){
        continue;
    }
    return nb;
}


//-----------------------------------------------------------------//


void ecrireReseau(Reseau *R, FILE *f){
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodités: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n", R->gamma);
    fprintf(f, "\n");

    CellNoeud* cour = R->noeuds;
    while (cour) {
        Noeud* nc = cour->nd;
        fprintf(f, "v %d %f %f\n", nc->num, nc->x, nc->y);
        cour = cour->suiv;
    }
    fprintf(f, "\n");

    Noeud* parcourus[R->nbNoeuds];
    int nb_parcourus = 0;

    cour = R->noeuds;
    while (cour) {
        CellNoeud* voisins = cour->nd->voisins;
        while(voisins) {
            int parcouru = 0;
            for (int i=0; i<nb_parcourus; i++){
                if (parcourus[i]==voisins->nd) {
                    parcouru = 1;
                }
            }
            if (parcouru==0){
                fprintf(f, "l %d %d\n", cour->nd->num, voisins->nd->num);
            }
            voisins = voisins->suiv;
        }
        cour = cour->suiv;
        parcourus[nb_parcourus] = cour->nd;
        nb_parcourus++;
    }
    fprintf(f, "\n");

    CellCommodite* commodites = R->commodites;
    while (commodites) {
        fprintf(f, "k %d %d\n", commodites->extrA->num, commodites->extrB->num);
        commodites = commodites->suiv;
    }
    
}


//-----------------------------------------------------------------//


void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

