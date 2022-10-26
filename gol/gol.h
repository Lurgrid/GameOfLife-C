// gol.h: Game of life, module permettant de gérer le jeux de la vie

#ifndef GOL__H
#define GOL__H

#include <stdio.h>
#include <stdlib.h>

#define ALP "-0"

typedef struct matrice matrice;

// mc_empty : crée un objet de type matrice. La matrice de largueur w et
//    de hauteur h est initialement vide. Renvoie un pointeur vers l'objet en
//    cas de succès, NULL en cas d'échec.
extern matrice *mc_empty(size_t w, size_t h);

// get_size : lit sur le fichier pointé par f la représentation de la matrice
//    d'alaphabet alp de largeur *w et de hauteur *h. Renvoie une valuer nulle
//    en cas de succès, une valeur non nulle si un carractère lu n'est pas dans
//    alp et en cas d'échec.
extern int get_size(FILE *f, size_t *w, size_t *h, const char *alp);

// mc_init : lit sur le fichier pointé par f la représentation de la matrice
//    d'alaphabet alp et initialise la matrice pointé par m avec les valeurs
//    représenté. Renvoie une valuer nulle en cas de succès, une valeur non
//    nulle si un carractère lu n'est pas dans alp et en cas d'échec.
extern int mc_init(FILE *f, matrice *m, const char *alp);

// mc_future : initialise a partir de la matrice pointé par src, la matrice next
//     qui est la prochaine étape du jeu de la vie. Renvoit une valeur nulle si
//     tout c'est bien passé, renvoit une valeur non nulle si les deux matrcie
//     ne sont pas de même taille ou en cas d'échec.
extern int mc_next(const matrice *src, matrice *next);

//  mc_dispose : si m ne vaut pas NULL, libère les ressources allouées
//    à m ainsi qu'a la matrice associée puis affecte la valeur NULL à m.
extern void mc_dispose(matrice **m);

// fprint_mc : écrit sur la sortie standard la matrice pointé par m. Renvoie EOF
//    en cas d'échec et zéro en cas de succès.
extern int print_mc(const matrice *m);

// clear_out : remet a zero la sortie standard, Renvoie une valeur nulle en cas
//    réussite, une valeur non nulle en cas d'échec.
extern int clear_out(void);

// sleep_gol : endort le processus appelant jusqu'à ce que ms millisecondes se
//    soient écoulées.
extern void sleep_gol(int ms);

#endif
