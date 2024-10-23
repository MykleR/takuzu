#include "game_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "queue.h"
#include "game_struct.h"
#include "game.h"
#include "game_ext.h"
#include "game_private.h"

#define NB_ARGS 4
#define ARG_MAX_LENGTH 3

game game_load(char *filename){
    assert(filename);
    FILE* f = fopen(filename,"r");
    if(f==NULL)
        _crash("Unable to open file [%s]",filename);
    
    int args[NB_ARGS];
    uint line_size = NB_ARGS*(ARG_MAX_LENGTH+1);
    char line[line_size];
    
    // CHARGEMENT DES PARAMETRES
    fgets(line, line_size, f);
    char *to_convert = strtok(line, " ");
    
    uint i;
    for(i=0; !is_empty_string(to_convert) && i<NB_ARGS; i++){
        args[i] = atoi(to_convert);
        to_convert=strtok(NULL, " ");// On passe au paramètre suivant
    }
    if(i!=NB_ARGS)
        _crash("Missing parameters in save file");

    // CHARGEMENT DE LA GRILLE
    uint tsize=args[0]*args[1];
    square tload[tsize];
    for(uint i=0; i<tsize ; i++){
        char c = fgetc(f);
        if(c!='\n')
            tload[i]=_char_to_square(c);
        else i--;
    }
    fclose(f);
    return game_new_ext(args[0], args[1], tload, args[2], args[3]);
}


void game_save(cgame g, char *filename){
    assert(g);
    assert(filename);
    
    FILE* f = fopen(filename,"w");
    assert(f);

    int max_lenght = pow(10,ARG_MAX_LENGTH+1)-1;
    if(game_nb_cols(g)>max_lenght || game_nb_rows(g)>max_lenght)
        _crash("Size limit exceeded, max is %i",max_lenght);
    //écriture du nombre de lignes
    char nb_rows[ARG_MAX_LENGTH];
    sprintf(nb_rows, "%d ", game_nb_rows(g));
    fputs(nb_rows,f);

    //écriture du nombre de lignes
    char nb_cols[ARG_MAX_LENGTH];
    sprintf(nb_cols, "%d ", game_nb_cols(g));
    fputs(nb_cols,f);

    int wrap = game_is_wrapping(g);
    fputc((char)wrap +48, f);
    fputc(' ',f);

    int uniq = game_is_unique(g);
    fputc((char)uniq +48 , f);
    fputs(" \n",f);

    //ecriture du tableau de squares
    for(uint i = 0; i<game_nb_rows(g); i++){
        for(uint j=0; j<game_nb_cols(g); j++){
            fputc(_square_to_char(game_get_square(g,i,j),true), f);
        }
        fputc('\n',f);
    }
    fclose(f);
}


bool game_solve(game g){
    assert(g);
    // PREREMPLISSAGE
    return _rec_game_solve(g,0);
}


uint game_nb_solutions(cgame g){
    assert(g);
    game gcopy = game_copy(g);
    // PREREMPLISSAGE
    uint nb_sol = _rec_game_nb_solutions(gcopy,0);
    game_delete(gcopy);
    return nb_sol;
}
