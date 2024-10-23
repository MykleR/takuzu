#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "queue.h"
#include "game_private.h"
#include "game_struct.h"
#include "game.h"


//même principe que game_new, crée une nouvelle partie en initialisant les options de cette dernière à partir des bouléens passés en paramètre
game game_new_ext(uint nb_rows, uint nb_cols, square* squares, bool wrapping, bool unique){
    assert(squares);
    assert(nb_cols>0);
    assert(nb_rows>0);
    assert(nb_rows%2==0);
    assert(nb_cols%2==0);
    game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, unique);
    assert(g);
    // set squares
    for (int i=0; i<nb_rows; i++)
        for (int j=0; j<nb_cols; j++) {
            square s = squares[INDEX(i,j,nb_cols)];
            game_set_square(g, i, j, s);
    }
    return g;
}

//création d'une partie vide et assignation de chaque cellule du tableau à S_EMPTY
game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, bool unique){
    assert(nb_cols>0);
    assert(nb_rows>0);
    assert(nb_cols % 2 == 0);
    assert(nb_rows % 2 == 0);
    game g = (game)malloc(sizeof(struct game_s));
    assert(g);
    g->height = nb_rows;
    g->width = nb_cols;
    g->wrapping = wrapping;
    g->unique = unique;
    g->cells = (square*)calloc(nb_rows*nb_cols, sizeof(uint));
    assert(g->cells);

    g->stack_undo = queue_new();
    assert(g->stack_undo);
    g->stack_redo = queue_new();
    assert(g->stack_redo);
    return g;
}

//getter : nombre de lignes
uint game_nb_rows(cgame g){
    assert(g);
    return g->height;
}

//getter : nombre de colonnes
uint game_nb_cols(cgame g){
    assert(g);
    return g->width;
}

//getter : savoir si l'option WRAPPING est activée
bool game_is_wrapping(cgame g){
    assert(g);
    return g->wrapping;
}

//getter : savoir si l'option UNIQUE est activée
bool game_is_unique(cgame g){
    assert(g);
    return g->unique;
}

//fonction permetant d'annuler un coup et de le stocker si le coup à besoin d'être rétabli
void game_undo(game g){
    assert(g);
    if (queue_is_empty(g->stack_undo))return;
    move m = _history_pop_move(g->stack_undo);
    game_set_square(g, m.i, m.j, m.prev);
    _history_new_move(g->stack_redo, m);
}

//fonction permetant de rétablir un coup et de le stocker si le coup à besoin d'être annuler une n-ième fois
void game_redo(game g){
    assert(g);
    if (queue_is_empty(g->stack_redo))return;
    move m = _history_pop_move(g->stack_redo);
    game_set_square(g, m.i, m.j, m.next);
    _history_new_move(g->stack_undo, m);
}