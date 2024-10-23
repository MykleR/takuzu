#ifndef __GAME_PRIVATE_H__
#define __GAME_PRIVATE_H__

#include <stdbool.h>

#include "game.h"
#include "game_struct.h"
#include "queue.h"

#define SE S_EMPTY
#define S0 S_ZERO
#define S1 S_ONE
#define SI0 S_IMMUTABLE_ZERO
#define SI1 S_IMMUTABLE_ONE

#define SQUARED_SIZE (DEFAULT_SIZE*DEFAULT_SIZE)

/**
 * @brief Structure move
 * @details représente les actions du jeu utilisées pour l'historique 
 */
struct move_s{
    uint i;//   la ligne
    uint j;//   la colonne
    square prev;//  état de la cellule avant que le coup soit joué
    square next;//  état après le coup
}; 
typedef struct move_s move;

/**
 * @brief Retourne le module de a par b, fonctionne aussi avec des nombres négatifs
 */
int mod(int a, int b);

bool is_empty_string(char* s);


void _crash(char *format, ...);

void _history_new_move(queue* q, move m);
move _history_pop_move(queue* q);
void _history_clear(queue* q);

int _count_in_row(cgame g, uint i, int number);
int _count_in_col(cgame g, uint j, int number);
bool _unique_row(cgame g, uint i);
bool _unique_col(cgame g, uint j);
bool _three_consecutive(cgame g, uint i, uint j);

uint _rec_game_nb_solutions(game g, uint pos);
bool _rec_game_solve(game g, uint pos);


/**
 * @brief Met à jour une position à partir d'une distance et d'une direction
 * @param dir La direction, UP, DOWN, LEFT ou RIGHT
 * @param dist La distance >0
 * @param d_x La coordonées horizontal à mettre à jour
 * @param d_y La coordonées vertical à mettre à jour
 */
void _dir_to_coords(direction dir, uint dist, int* d_x, int* d_y);

/**
 * @brief Convertion du type square à char
 * @param s Le square à convertir
 * @return le caractère correspondant au square 
 */
char _square_to_char(square s, bool save_format);
square _char_to_square(char c);


#endif  // __GAME_PRIVATE_H__