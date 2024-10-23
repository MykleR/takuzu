#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "game_private.h"
#include "game_struct.h"
#include "game_ext.h"
#include "queue.h"



game game_new(square* squares) {
    return game_new_ext(DEFAULT_SIZE,DEFAULT_SIZE,squares,false,false);
}


game game_new_empty(void) {
    return game_new_empty_ext(DEFAULT_SIZE,DEFAULT_SIZE,false,false);
}


game game_copy(cgame g) {
    // Recréation d'un jeu avec les même paramètres
    return game_new_ext(
        game_nb_rows(g),
        game_nb_cols(g),
        g->cells,
        game_is_wrapping(g),
        game_is_unique(g));
}


bool game_equal(cgame g1, cgame g2) {
    assert(g1 && g2);
    // doit avoir les même dimensions
    if (game_nb_rows(g1)!=game_nb_rows(g2) || game_nb_cols(g1)!=game_nb_cols(g2))
        return false;
    // doit avoir les même options
    if (game_is_wrapping(g1)!=game_is_wrapping(g2))
        return false;
    if(game_is_unique(g1)!=game_is_unique(g2))
        return false;

    for(int i=0; i<game_nb_rows(g1) ;i++){
        for(int j=0; j<game_nb_cols(g1); j++) {
            // doit avoir la même valeur dans la case i,j
            if(game_get_square(g1,i,j)!=game_get_square(g2,i,j))
                return false;
        }
    }
    return true;// Aucune différence détectée
}


void game_delete(game g) {
    if (g==NULL)
        return;
    // Libération du tableau de la grille, des historiques, et du jeu
    free(g->cells);
    queue_free_full(g->stack_redo,free);
    queue_free_full(g->stack_undo,free);
    free(g);
}


void game_set_square(game g, uint i, uint j, square s) {
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    SQUARE(g,i,j) = s;
}


square game_get_square(cgame g, uint i, uint j){
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    return SQUARE(g,i,j);
}


int game_get_number(cgame g, uint i, uint j){
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    square s = game_get_square(g,i,j);
    if ((s == S0) || (s == SI0)) 
        return 0;
    if ((s == S1) || (s == SI1))
        return 1;
    return -1;
}


int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist){
    assert(g);
    assert(dist<=2);
    int d_x=j, d_y=i;
    // Calcul des nouvelles coordonnées
    _dir_to_coords(dir,dist,&d_x,&d_y);
    // Option Wrapping on reste dans la grille
    if (game_is_wrapping(g)){
        d_y = mod(d_y,game_nb_rows(g));
        d_x = mod(d_x,game_nb_cols(g));
    }
    //on vérifie que l'on soit toujours dans la grille
    if(d_x<0 || d_x>=game_nb_cols(g) || d_y<0 || d_y>=game_nb_rows(g))
        return -1;
    return game_get_square(g,d_y,d_x);
}


int game_get_next_number(cgame g, uint i, uint j, direction dir, uint dist){
    assert(g);
    assert(dist<=2);
    int s = game_get_next_square(g, i, j, dir, dist);
    if ((s == S0) || (s == SI0))
        return 0;
    if ((s == S1) || (s == SI1))
        return 1;
    return -1;
}


bool game_is_empty(cgame g, uint i, uint j){
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    return game_get_square(g,i,j)==SE;
}


bool game_is_immutable(cgame g, uint i, uint j){
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    square s = game_get_square(g,i,j);
    return s==SI1 || s==SI0;
}


int game_has_error(cgame g, uint i, uint j){
    assert(g);
    assert(i<game_nb_rows(g));
    assert(j<game_nb_cols(g));
    // Pas d'erreur possible si la case est vide
    if(game_is_empty(g,i,j))
        return 0;

    // check number of ones and zeros in row & col i,j
    if(_count_in_row(g, i,0) > (game_nb_cols(g)/2)) return 1;
    if(_count_in_row(g, i,1) > (game_nb_cols(g)/2)) return 1;
    if(_count_in_col(g, j,0) > (game_nb_rows(g)/2)) return 2;
    if(_count_in_col(g, j,1) > (game_nb_rows(g)/2)) return 2;

    // check three consecutive in row & col i,j
    if (_three_consecutive(g, i, j)) return 3;

    if (game_is_unique(g)){
        // check if row & col is unique
        if (!_unique_row(g, i)) return 5;
        if (!_unique_col(g, j)) return 6;
    }
    return 0;
}


bool game_check_move(cgame g, uint i, uint j, square s){
    assert(g);
    // Un coup joué est légal si: 1- Les coordonnées sont dans la grille
    //  2- La nouvelle valeur n'est pas immutable
    //  3- La case cible n'est pas immutable
    return i<game_nb_rows(g) && j<game_nb_cols(g)
         && i>=0 && j>=0
         && s!=S_IMMUTABLE_ONE && s!=S_IMMUTABLE_ZERO 
         && !game_is_immutable(g,i,j);
}


void game_play_move(game g, uint i, uint j, square s){
    assert(game_check_move(g,i,j,s));
    // Mise à jour de la grille
    square before = game_get_square(g,i,j);
    game_set_square(g,i,j,s);
    // Mise à jour des historiques
    move m = {i,j,before,s};
    _history_new_move(g->stack_undo, m);
    _history_clear(g->stack_redo);
}


bool game_is_over(cgame g){
    assert(g);
    // Verifie si la grille est pleine et s'il n'y a pas d'erreur des cases
    for(int i=0; i<game_nb_rows(g); i++) {
        for(int j=0; j<game_nb_cols(g); j++) {
            if(game_is_empty(g,i,j))
                return false;
            if(game_has_error(g,i,j))
                return false;
        }
    }
    // Le jeu est bien terminé
    return true;
}


void game_restart(game g){
    assert(g);
    // On vide l'historique
    _history_clear(g->stack_redo);
    _history_clear(g->stack_undo);
    // On vide toutes les cases immutables
    for(int i=0; i<game_nb_rows(g) ;i++) {
        for(int j=0; j<game_nb_cols(g); j++) {
            if(!game_is_immutable(g,i,j))
                game_set_square(g,i,j,SE);
        }
    }
}