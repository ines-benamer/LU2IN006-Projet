// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "Graphe.h"
#include "Struct_File.h"

// question 7.1 : 

//------------------------------------------CREE_SOMMET-----------------------------------

Sommet *cree_sommet(int num, int x, int y) {
    // On cree un sommet
    Sommet *new = (Sommet *)malloc(sizeof(Sommet));
    if (!new) return NULL;
    new->L_voisin = NULL;
    new->num = num;
    new->x = x;
    new->y = y;

    return new;
}

//------------------------------------------CREE_ARETE-----------------------------------

Arete *cree_arete(int u, int v) {
    // On cree une arete
    Arete *arete = (Arete *)malloc(sizeof(Arete));
    if (!arete) return NULL;
    arete->u = u;
    arete->v = v;

    return arete;
}

//------------------------------------------CREE_CELLULE_ARETE-----------------------------------

Cellule_arete *cree_cellule_arete(Arete *arete) {
    // On cree une cellule d'arete
    Cellule_arete *cellule = (Cellule_arete *)malloc(sizeof(Cellule_arete));
    if (!cellule) return NULL;
    cellule->a = arete;
    cellule->suiv = NULL;

    return cellule;
}

//------------------------------------------LIBERER_GRAPHE-----------------------------------

void liberer_graphe(Graphe *graphe) {
    if (!graphe) return;
    free(graphe->T_commod);
    for (int i = 0; i < graphe->nbsom; i++)
        liberer_sommet(graphe->T_som[i]);
    free(graphe->T_som);
    free(graphe);
}

//------------------------------------------LIBERER_SOMMET-----------------------------------

void liberer_sommet(Sommet *sommet) {
    if (!sommet) return;
    Cellule_arete *tmp = NULL;
    while (sommet->L_voisin) {
        tmp = sommet->L_voisin->suiv;
        liberer_arete(sommet->L_voisin);
        sommet->L_voisin = tmp;
    }
    free(sommet);
}

//------------------------------------------LIBERER_ARETE----------------------------------

void liberer_arete(Cellule_arete *arete) {
    if (!arete) return;
    if (arete->a->u == -1 && arete->a->v == -1)
        free(arete->a);
    else {
        arete->a->u = -1;
        arete->a->v = -1;
    }
    free(arete);
}


//------------------------------------------CREE_GRAPHE-----------------------------------

Graphe* creerGraphe(Reseau* r){
    if(r == NULL){
        printf("Impossible de creer Graphe a partir d'un reseau qui n'existe pas\n");
        return NULL;
    }

    Graphe *g = (Graphe*)malloc(sizeof(Graphe));
    if(g==NULL){
        printf("Erreur malloc création Graphe\n");
        return NULL;
    }

    /*Initialisation des variables de g*/
    g->nbsom = r->nbNoeuds;
    g->gamma = r->gamma;
    g->nbcommod = nbCommodites(r);

    g->T_som = (Sommet**)malloc(sizeof(Sommet*) * g->nbsom);
    if(g->T_som == NULL){
        printf("Erreur malloc T_som création Graphe\n");
        free(g);
        return NULL;
    }
    for(int i=0; i < g->nbsom; i++){
        g->T_som[i] = NULL;
    }

    g->T_commod = (Commod*)malloc(sizeof(Commod) * g->nbcommod);
    if(g->T_commod == NULL){
        printf("Erreur malloc T_commod création Graphe\n");
        free(g->T_som);
        free(g);
        return NULL;
    }

    /*Boucle qui parcours tous les noeuds du Reseau*/
    CellNoeud *cr = r->noeuds;
    int num;
    for(int i=0; i < g->nbsom; i++){
        /*Creation du sommet correspondant au noeud*/
        /*Le numero du noeud (-1 car ils commencent a 1) correspond au numero du sommet, et donc de sa position dans le tableau*/
        num = (cr->nd->num) - 1;
        g->T_som[num] = creerSommet(num, cr->nd->x, cr->nd->y);
        
        /*Mise a jour aretes*/
        majAretes(g, g->T_som[num], cr->nd);

        cr = cr->suiv;
    }

    /*Boucle passant par toutes les commodites du Reseau*/
    CellCommodite *Cr = r->commodites;
    for(int i=0; i<(g->nbcommod); i++){
        Commod C;
        C.e1 = (Cr->extrA->num)-1;
        C.e2 = (Cr->extrB->num)-1;

        g->T_commod[i] = C;
        Cr = Cr->suiv;
    }

    return g;
}


//------------------------------PLUS_PETIT_NB_ARETES---------------------------------------

int plus_petit_nb_aretes(Graphe *graphe, int u, int v) {
    // On soustrait -1 des numero de sommets (indice du tableau commence par 0)


    // Si le graphe est null on retourne directement (On sort de la fonction)
    if (!graphe) {
        return -1;
    }

    // Si les numero de sommet depasse le numero max on sort de la fonction
    if (u < 0 || u >= graphe->nbsom || v < 0 || v >= graphe->nbsom) {
        return -1;
    }

    // On cree un tableau de boolean,(Si la case est 0 donc le sommet n'est pas encore visté)
    int *visit = (int *)malloc(sizeof(int) * graphe->nbsom);

    // On teste si l'allocation du tableau s'est bien passé
    if (!visit) {
        return -1;
    }

    // On cree un tableau pour garder les distances minimales entre les deux sommets
    int *D = (int *)malloc(sizeof(int) * graphe->nbsom);
    // On teste si l'allocation  du tableau s'est bien passé
    if (!D) {
        free(visit);
        return -1;
    }

    for (int i = 0; i < graphe->nbsom; i++)
        visit[i] = 0;

    visit[u] = 1;
    D[u] = 0;

    // On cree une file
    S_file *file = cree_file();

    // On teste si la file est bien allouer
    if (!file) {
        free(visit);
        free(D);
        return -1;
    }

    // On enfile le premier sommet pour le parcourir
    enfile(file, u);

    // Tant qu'il existe encore des sommets qu'on doit parcourir
    while (!estFileVide(file)) {
        // On defile le sommet a parcourir
        int curr = defile(file);

        // On boucle sur sa liste d'adjacents
        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            // On recupere le numero du sommet adjacent
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;

            // Si il n'st pas encore visiter
            if (visit[pos] == 0) {
                // On met le boolean a 1 pour ne pas l'ajouter une deuxieme fois
                visit[pos] = 1;
                enfile(file, pos);
                // On calcule la distance entre le sommet et le premier sommet
                D[pos] = D[curr] + 1;
            }
        }
    }

    // On recupere la distance du dernier sommet et on libere les tableau et la file
    int result = D[v];
    free(visit);
    free(D);
    liberer_file(file);
    return result;
}

//-------------------------------------------------------------------------------------------------------------

/* Retourne un liste de numeros de sommets de la plus courte chaine entre deux sommets */
void generate_plus_petit_chaine(Graphe *graphe, int u, int v, S_file *liste) {
    init_liste(liste);

    // On soustrait -1 des numero de sommets (indice du tableau commence par 0)
    u--;
    v--;
    if (!graphe) {
        return;
    }

    // Si les numero de sommet depasse le numero max on sort de la fonction
    if (u < 0 || u >= graphe->nbsom || v < 0 || v >= graphe->nbsom) {
        return;
    }

    // On cree un tableau de boolean,(Si la case est 0 donc le sommet n'est pas encore visté)
    int *visit = (int *)malloc(sizeof(int) * graphe->nbsom);

    if (!visit) {
        return;
    }
    // On cree un tableau de predecesseur
    //( si la case dont l'indice est le numero du sommet est a -1 donc elle n'a pas de predecesseur, sinon son predecesseur a pour numero de sommet la valeur de la case)
    // on l'utilise pour tracer un chemin entre deux sommets
    int *pred = (int *)malloc(sizeof(int) * graphe->nbsom);

    if (!pred) {
        free(visit);
        return;
    }
    for (int i = 0; i < graphe->nbsom; i++) {
        visit[i] = 0;
        pred[i] = -1;
    }

    visit[u] = 1;

    // On cree une file
    // Elle sert a stoquer les somets qui ne sont pas encore visité et qui doivent etre visité
    // A chaque fois on defile un sommets pour le parcourir et o enfile les sommets adjacents
    S_file *file = cree_file();

    // On teste si la file est bien allouer
    if (!file) {
        print_probleme("Erreur de creation");
        free(visit);
        free(pred);
        return;
    }

    // On enfile le premier sommet pour le parcourir
    enfile(file, u);

    // Tant qu'il existe encore des sommets qu'on doit parcourir
    while (!est_file_vide(file)) {
        // On defile le sommet a parcourir
        int curr = defile(file);

        // On boucle sur sa liste d'adjacents
        for (Cellule_arete *voisins = graphe->T_som[curr]->L_voisin; voisins; voisins = voisins->suiv) {
            // On recupere le numero du sommet adjacent
            int pos = voisins->a->u == curr ? voisins->a->v : voisins->a->u;

            // Si il n'st pas encore visiter
            if (visit[pos] == 0) {
                // On met le boolean a 1 pour ne pas l'ajouter une deuxieme fois
                visit[pos] = 1;
                enfile(file, pos);
                // On garde le precedent
                pred[pos] = curr;
            }
        }
    }

    int i = v;
    // On fait le parcours inverse pour recuperer le chemin
    while (pred[i] != -1) {
        ajoute_en_tete(liste, i + 1);
        i = pred[i];
    }

    ajoute_en_tete(liste, u + 1);

    free(visit);
    free(pred);
    liberer_file(file);
}

//------------------------------------------------------------------------------------------------------------------

/*Mise a jour des aretes d'un sommet*/
void MiseAjourAretes(Graphe *g, Sommet *s, Noeud *n){
    /*Boucle passant par tous les voisins du noeud*/
    CellNoeud *nvois = n->voisins;
    while(nvois){
        /*On cherche le sommet correspondant au noeud voisin*/
        Sommet *svois = g->T_som[((nvois->nd->num)-1)];

            /*Si le sommet correspondant au voisin a ete créé, on ajoute l'arete
            Sinon, ce sommet sera créé plus tard*/
            if(svois){
                /*On sait que l'arete n'existe pas car le nouveau sommet viens d'etre créé
                Nous créons donc l'arete et nous l'ajoutons en tete de liste (des deux sommets!)*/
                Arete *a = creerArete(s->num, svois->num);
                Cellule_arete *c = creerCellule_arete(a);
                Cellule_arete *cvois = cree_cellule_arete(a);

                c->suiv = s->L_voisin;
                s->L_voisin = c;

                cvois->suiv = svois->L_voisin;
                svois->L_voisin = cvois;
            }

        nvois = nvois->suiv;
    }
}

//------------------------------------------------------------------------------------------------------------------


int reorganise_reseau(Reseau *reseau) {
    // Si le reseau est null on sort de la fonction
    if (!reseau) {
        return 0;
    }

    // On cree un graphe et on teste l'allocation
    Graphe *graphe = creer_graphe(reseau);
    if (!graphe) {
        return 0;
    }

    // On cree une matrice (sommet-sommet) et on teste l'allocation
    int **mat_chaines = (int **)malloc(sizeof(int *) * graphe->nbsom);
    if (!mat_chaines) {
        liberer_graphe(graphe);
        return 0;
    }

    for (int i = 0; i < graphe->nbsom; i++) {
        mat_chaines[i] = (int *)malloc(sizeof(int) * graphe->nbsom);

        if (!mat_chaines[i]) {

            for (int j = 0; j < i; j++)
                free(mat_chaines[j]);

            free(mat_chaines);
            liberer_graphe(graphe);
            return 0;
        }

        // On initialise les case de la matrice a 0
        for (int j = 0; j < graphe->nbsom; j++)
            mat_chaines[i][j] = 0;
    }

    Cellule_file* liste;
    for (int i = 0; i < graphe->nbcommod; i++) {
        // On recupere pour chaque commodites le plus court chemin
        generate_plus_petit_chaine(graphe, graphe->T_commod[i].e1 + 1, graphe->T_commod[i].e2 + 1, &liste);

        Cellule_file *pred = NULL;

        // On incremente la case de chaque arete dans la matrice lorsqu'on la trouve dans le chemin de l'arete
        for (Cellule_file *curr = liste; curr; pred = curr, curr = curr->suiv) {
            if (pred) {
                mat_chaines[curr->val - 1][pred->val - 1]++;
                mat_chaines[pred->val - 1][curr->val - 1]++;

                // Si le nombre d'apparaissance est superieur a gamma on retourne faux (0) et on libere tout les tableu et la matrice
                if (mat_chaines[curr->val - 1][pred->val - 1] > graphe->gamma) {
                    for (int i = 0; i < graphe->nbsom; i++)
                        free(mat_chaines[i]);

                    free(mat_chaines);
                    liberer_graphe(graphe);
                    desalloue(&liste);

                    return 0;
                }
            }
        }

        desalloue(&liste);
    }

    // Sinon on retourne 1 si tout les apparaissance des arete est inferieur a gamma
    for (int i = 0; i < graphe->nbsom; i++)
        free(mat_chaines[i]);

    free(mat_chaines);

    liberer_graphe(graphe);
    return 1;
}