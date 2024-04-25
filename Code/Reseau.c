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

    ajouteVoisin(n1, n2);
    ajouteVoisin(n2, n1);

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
    Reseau *reseau = creeReseau(C->gamma);
    for(CellChaine *chaine = C->chaines; chaine;chaine = chaine->suiv){// on parcourt les chaines 
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

                CellNoeud *noeud_voisin = creeCellNoeud(second); //c'est le noeud correspondant au voisin, pour l'ajouter dans les voisins
                if(! noeud_voisin){
                    liberer_reseau(reseau);
                    return NULL;
                }

                CellNoeud *noeud_courant = creeCellNoeud(noeud); //c'est le noeud courant 
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

