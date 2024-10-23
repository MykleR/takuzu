#include "game_aux.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "game_private.h"
#include "game_struct.h"
#include "game.h"
#include "game_ext.h"


//création d'une grille par défaut pour la création d'une partie par défaut
static square tdefault[] = {
    SE, SI1, SI0, SE, SE , SE,
    SE, SE , SE , SE, SE , SE,
    SE, SI0, SE , SE, SI0, SE,
    SE, SI0, SI1, SE, SE , SE,
    SE, SE , SI1, SE, SE , SI0,
    SE, SE , SE , SE, SE , SI0
};

//création d'une grille de solution par défaut pour la création d'une solution par défaut
static square tdefault_sol[] = {
    S0, SI1, SI0, S1, S0 , S1,
    S0, S1 , S1 , S0, S1 , S0,
    S1, SI0, S0 , S1, SI0, S1,
    S1, SI0, SI1, S0, S0 , S1,
    S0, S1 , SI1, S0, S1 , SI0,
    S1, S0 , S0 , S1, S1 , SI0
};


//affichage du jeu et de la grille
void game_print(cgame g){
    assert(g);
    printf("   ");
    for(int i=0;i<game_nb_cols(g);i++)
        printf("%d",i);
    printf("\n   ");
    for(int i=0;i<game_nb_cols(g);i++)
        printf("-");
    printf("\n");
    for (int i=0; i<game_nb_rows(g); i++){
        printf("%d |",i);
        for (int j=0; j<game_nb_cols(g); j++){
            char c = _square_to_char(game_get_square(g, i, j),false);
            printf("%c", c);
        }
        printf("|\n");
    }
    printf("   ");
    for(int i=0;i<game_nb_cols(g);i++)
        printf("-");
    printf("\n");
}

//création d'une partie par défaut
game game_default(){
    return game_new(tdefault);
}

//création d'une partie par défaut déjà résolue
game game_default_solution(){
    return game_new(tdefault_sol);
}