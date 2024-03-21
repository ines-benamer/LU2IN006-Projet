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
}

//-----------------------------------------------------------------//

Reseau* creer_reseau(int gamma){
    Reseau *reseau = malloc(sizeof(Reseau));
    reseau->gamma = gamma;
    reseau->commodites = NULL;
    reseau->nbNoeuds = 0;
    reseau->noeuds = NULL;
    return reseau;
}

//-----------------------------------------------------------------//

CellNoeud *cree_cell_noeud(Noeud *noeud) {
    CellNoeud *cell = (CellNoeud *)malloc(sizeof(CellNoeud));
    cell->nd = noeud;
    cell->suiv = NULL;
    return cell;
}

//-----------------------------------------------------------------//

void liberer_reseau(Reseau *reseau) {
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

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {

}

//-----------------------------------------------------------------//

Reseau* reconstitueReseauListe(Chaines *C){
    Reseau *reseau = creer_reseau(C->gamma);
    for(CellChaine *chaine = C->chaines; chaine;chaine = chaine->suiv){// on parcours les chaines 
    Noeud *premier = NULL;
    Noeud *second = NULL;
        
        for(CellPoint *point = chaine->points;point;point = point->suiv){
            Noeud *noeud = rechercheCreeNoeudListe(reseau,point->x,point->y);
            if(! premier){
                premier = noeud;
            }

            if(second){ // si second  existe déja donc on a un premier (precedent)
                int existe = 0;
                for(CellNoeud *voisins = noeud->voisins;voisins;voisins = voisins->suiv){
                    if(voisins->nd->num == second->num){ // on verifier si y'a deja la liaison on l'ajoute pas 
                        existe = 1 ;
                        break;
                    }
                }

                if(existe){// on verifier si la liaison existe on passe au prochain point 
                    second = noeud;
                    continue;
                }

                CellNoeud *noeud_voisin = cree_cell_noeud(second); //c'est le noeud correspondant au voisin, pour l'ajouter dans les voisins
                if(! noeud_voisin){
                    liberer_reseau(reseau);
                    return NULL;
                }

                CellNoeud *noeud_courant = cree_cell_noeud(noeud); //c'est le noeud courant 
                if(! noeud_voisin){
                    liberer_reseau(reseau);
                    free(noeud_voisin);
                    return NULL;
                }
                // on ajoute le voisin dans la liste du noeud courant 
                noeud_voisin ->suiv = noeud->voisins;
                noeud->voisins = noeud_voisin;

                //on ajoute le noeud courant dans la liste du voisin courant 
                noeud_courant ->suiv = second->voisins;
                second->voisins = noeud_courant;
            }
            // on garde le dernier noeud 
            second = noeud;
        }

        if(premier){// on crée la commodites entre premier et second si premier n'est pas NULL
            CellCommodite *cmd = creeCommodite(premier,second);
            if(!cmd){
                liberer_reseau(reseau);
                return NULL;
            }
            // on ajoute la commodites en tete de la liste des commodites
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}


//exercice 3 :

void ecrireReseau(Reseau *R, FILE *f){}

//-----------------------------------------------------------------//

static int nb_voisins(CellNoeud* liste_noeud){
    int nb =0 ;
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

//-----------------------------------------------------------------//
