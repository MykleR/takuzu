#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/game.h"
#include "src/game_aux.h"
#include "src/game_ext.h"
#include "src/game_tools.h"
#include <stdarg.h>


static void _crash(char *format, ...){
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stderr,"\n");
    va_end(args);
    exit(EXIT_FAILURE);
}


int main(int argc , char* argv[]){
    if(argc<3)
        _crash("Not enought arguments\n");
    bool is_solve_option = strcmp(argv[1],"-s")==0;
    bool is_count_option = strcmp(argv[1],"-c")==0;
    bool must_save = argc == 4;
    if(!(is_solve_option || is_count_option))
        _crash("Invalid option %s\n",argv[1]);

    game g = game_load(argv[2]);
    if(g==NULL)
        _crash("Unable to load game [%s]",argv[2]);

    if (is_solve_option){
       if (!game_solve(g))
            _crash("No solution to this game");
       else if (must_save)
            game_save(g,argv[3]);
       else
            game_print(g);
        return EXIT_SUCCESS;
    }

    uint nb_solutions = game_nb_solutions(g);      
    if (must_save){
        FILE* f =fopen(argv[3],"w");
        if(f==NULL)
            _crash("Can't save to file [%s]",argv[3]);
        char str_number [3];
        sprintf(str_number, "%d", nb_solutions);
        if(fputs(str_number,f)!=1)
            _crash("Can't save to file [%s]",argv[3]);
    } else 
        printf("%u\n",nb_solutions);
    game_delete(g);
    return EXIT_SUCCESS;
}