// Louisa OULD BOUALI 21100589
// Inès BENAMER BELKACEM 21204927

#include <stdlib.h>
#include <stdio.h>
#include "Struct_File.h"


// j'ai ajouter la fonction cree_file et liberer_file pour la question 2 de l'exercice 7 .
S_file *cree_file() {
    S_file *file = malloc(sizeof(S_file));
    if (!file) {
        return NULL;
    }

    Init_file(file);
    return file;
}

void Init_file(S_file *f){
  f->tete=NULL;
  f->dernier=NULL;
}

int estFileVide(S_file *f){
  return f->tete == NULL;
}

void enfile(S_file * f, int donnee){
 Cellule_file *nouv=(Cellule_file *) malloc(sizeof(Cellule_file));
  nouv->val=donnee;
  nouv->suiv=NULL;
  if (f->tete==NULL)
    f->tete=nouv;
  else
    f->dernier->suiv=nouv;
  f->dernier=nouv;
}


int defile(S_file *f){
  int v=f->tete->val;
  Cellule_file *temp=f->tete;
  if (f->tete==f->dernier)
    f->dernier=NULL;
  f->tete=f->tete->suiv;
  free(temp);
  return v;

}

void liberer_file(S_file *f) {
    if (!f) return;

    while (!(estFileVide(f))) defile(f);
    free(f);
}
