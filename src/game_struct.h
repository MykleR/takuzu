#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

#include <stdbool.h>

#include "game.h"
#include "queue.h"

/**
 * @brief structure représentant le jeu
 */
struct game_s
{
    unsigned int width;//   nombre de colonnes
    unsigned int height;//  nombre de lignes

    //tableau représentant grille du jeu
    square* cells;

    //options du jeu
    bool wrapping;
    bool unique;

    //2 queue utilisées comme des piles pour gérer l'historique
    queue* stack_undo; // pile des coups déjà joués et annulables
    queue* stack_redo; // pile de coups déjà annulés et rejouable 
};

#define INDEX(i,j,w) ((i) * (w) + (j))
#define SQUARE(g, i, j) ((g)->cells[(INDEX(i, j, g->width))])

#endif  // __GAME_STRUCT_H__