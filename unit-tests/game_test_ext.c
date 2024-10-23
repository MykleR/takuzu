#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include "../src/game_aux.h"
#include "../src/game.h"
#include "../src/game_ext.h"
#include "../src/game_tools.h"

static square tnot_solvable[] = {
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY,
    S_EMPTY, S_EMPTY, S_EMPTY, S_IMMUTABLE_ONE,
    S_EMPTY, S_EMPTY, S_EMPTY, S_IMMUTABLE_ONE,
    S_EMPTY, S_IMMUTABLE_ZERO, S_IMMUTABLE_ZERO, S_EMPTY
};


void usage(int argc, char *argv[]) {
	fprintf(stderr,"Usage: %s <testname> [<...>]\n",argv[0]);
	exit(EXIT_FAILURE);
}

int test_failed(int argc, ...){
    va_list args;
    va_start(args,argc);
    for(int i=0; i<argc; i++){
        game g = va_arg(args,game);
        game_delete(g);
    }
    va_end(args);
    return EXIT_FAILURE;
}

int test_success(int argc, ...){
    va_list args;
    va_start(args,argc);
    for(int i=0; i<argc; i++){
        game g = va_arg(args,game);
        game_delete(g);
    }
    va_end(args);
    return EXIT_SUCCESS;
}

int test_game_new_ext(){
    uint width=8;
    uint height = 6;
    uint size=width*height;
    square cells[size];
    int t[]= {S_EMPTY, S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO};
    for (int i=0 ; i<size; i++){
        cells[i] = t[i%5];
    }  
    game g = game_new_ext(height,width,cells,false,true);
    assert(g);
    assert(!game_is_wrapping(g));
    assert(game_is_unique(g));
    assert(game_nb_rows(g)==height);
    assert(game_nb_cols(g)==width);
    for (int i=0; i<size; i++){
        int x = i%width;
        int y = i/width;
        if(game_get_square(g, y, x)!=cells[i])
            return test_failed(1,g);
    }
    return test_success(1,g);
}

int test_game_new_empty_ext(){
    game g = game_new_empty_ext(6,8,true,false);
    assert(g);
    assert(game_is_wrapping(g));
    assert(!game_is_unique(g));
    assert(game_nb_rows(g)==6);
    assert(game_nb_cols(g)==8);
    for (int y=0; y < DEFAULT_SIZE ; y++){
        for (int x=0; x < DEFAULT_SIZE ; x++){
            if(game_get_square(g, x, y)!=S_EMPTY)
                return test_failed(1,g);
        }
    }
    return test_success(1,g);
}

int test_game_nb_rows(){
    game g1 = game_new_empty();
    if(g1==NULL || game_nb_rows(g1)!=DEFAULT_SIZE)
        return test_failed(1,g1);
    game g2 = game_new_empty_ext(4,6,false,false);
    if(g2==NULL || game_nb_rows(g2)!=4)
        return test_failed(2,g1,g2);
    return test_success(2,g1,g2);
    
}

int test_game_nb_cols(){
    game g1 = game_new_empty();
    if(g1==NULL || game_nb_cols(g1)!=DEFAULT_SIZE)
        return test_failed(1,g1);
    game g2 = game_new_empty_ext(6,4,false,false);
    if(g2==NULL || game_nb_cols(g2)!=4)
        return test_failed(2,g1,g2);
    return test_success(2,g1,g2);
}

int test_game_is_wrapping(){
    game g1 = game_new_empty();
    if(g1==NULL || game_is_wrapping(g1))
        return test_failed(1,g1);
    game g2 = game_new_empty_ext(DEFAULT_SIZE,DEFAULT_SIZE,true,false);
    if(g2==NULL || !game_is_wrapping(g2))
        return test_failed(2,g1,g2);
    return test_success(2,g1,g2);
}

int test_game_is_unique(){
    game g1 = game_new_empty();
    if(g1==NULL || game_is_unique(g1))
        return test_failed(1,g1);
    game g2 = game_new_empty_ext(DEFAULT_SIZE,DEFAULT_SIZE,false,true);
    if(g2==NULL || !game_is_unique(g2))
        return test_failed(2,g1,g2);
    return test_success(2,g1,g2);
}

int test_game_undo(){
    game g = game_new_empty();
    assert(g);
    // TESTING A SINGLE UNDO
    game_play_move(g,1,1,S_ONE);
    game_undo(g);
    if(game_get_square(g,1,1)!=S_EMPTY)
        return test_failed(1,g);
    // TEST THAT UNDO UPDATES REDO
    game_redo(g);
    if(game_get_square(g,1,1)!=S_ONE)
        return test_failed(1,g);
    // TESTING MULTIPLE UNDO
    game_play_move(g,1,2,S_ZERO);
    game_undo(g);
    game_undo(g);
    if(game_get_square(g,1,2)!=S_EMPTY && game_get_square(g,1,1)!=S_EMPTY)
        return test_failed(1,g);
    return test_success(1,g);
}

int test_game_redo(){
    game g = game_new_empty();
    assert(g);
    game_play_move(g,1,2,S_ZERO);
    game_play_move(g,1,1,S_ONE);
    game_undo(g);
    game_undo(g);
    game_redo(g);
    if(game_get_square(g,1,2)!=S_ZERO)
        return test_failed(1,g);
    game_redo(g);
    if(game_get_square(g,1,1)!=S_ONE)
        return test_failed(1,g);
    return test_success(1,g);
}
// Tests Game IO

int test_game_load(){
    
    game g0 = game_default();
    assert(g0);
    game_save(g0, "default.txt");
    game g1 = game_load("default.txt");
    assert(g1);

    if(game_equal(g0, g1)){
        return test_success(2,g0,g1);
    } 
    return test_failed(2, g0, g1);
}

int test_game_save(){
    return test_game_load();
}

int test_game_solve(){
    game g1 = game_new_empty();
    bool b1 = game_solve(g1);

    if(!game_is_over(g1) && !b1){
        return test_failed(1,g1);
    }
    
    game g2 = game_new_ext(4, 4, tnot_solvable, false, false);
    game g2_copied = game_copy(g2);
    bool b2 = game_solve(g2);

    if(game_is_over(g2) && b2 && !game_equal(g2, g2_copied)){
        return test_failed(3,g1, g2, g2_copied);
    }
    return test_success(3, g1, g2, g2_copied);
}

int test_game_nb_solutions(){
    game g0 = game_new_ext(4, 4, tnot_solvable, false, false);
    uint nb_sol_g0 = game_nb_solutions(g0);
    bool solved_g0 = game_solve(g0);
    if(solved_g0 && nb_sol_g0>0)
        return test_failed(1, g0);

    game g1 = game_default();
    int nb_sol_g1 = game_nb_solutions(g1);
    if(nb_sol_g1!=1)
        return test_failed(2, g0,g1);
    return test_success(2, g0,g1);
}



int main(int argc, char *argv[]) {
	if(argc==1)
		usage(argc, argv);
	
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	int exit_code = EXIT_FAILURE;
	
	if (strcmp("game_new_ext", argv[1]) == 0)
    	exit_code = test_game_new_ext();
    else if(strcmp("game_new_empty_ext", argv[1]) == 0)
        exit_code = test_game_new_empty_ext();
    else if(strcmp("game_nb_rows", argv[1]) == 0)
        exit_code = test_game_nb_rows();
    else if(strcmp("game_nb_cols", argv[1]) == 0)
        exit_code = test_game_nb_cols();
    else if(strcmp("game_is_wrapping", argv[1]) == 0)
        exit_code = test_game_is_wrapping();
    else if(strcmp("game_is_unique", argv[1]) == 0)
        exit_code = test_game_is_unique();
    else if(strcmp("game_undo", argv[1]) == 0)
        exit_code = test_game_undo();
    else if(strcmp("game_redo", argv[1]) == 0)
        exit_code = test_game_redo();
    else if(strcmp("game_load", argv[1]) == 0)
        exit_code = test_game_load();
    else if(strcmp("game_save", argv[1]) == 0)
        exit_code = test_game_save();
    else if(strcmp("game_solve", argv[1]) == 0)
        exit_code = test_game_solve();
    else if(strcmp("game_nb_solutions", argv[1]) == 0)
        exit_code = test_game_nb_solutions();
	else {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
	}
	
	char* s = (exit_code==EXIT_SUCCESS) ? "SUCCESS" :"FAILURE";
	fprintf(stderr,"Test %s finished: %s\n", argv[1], s);
	return exit_code;
}