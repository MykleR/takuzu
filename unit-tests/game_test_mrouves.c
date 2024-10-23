#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include "../src/game_aux.h"
#include "../src/game_ext.h"
#include "../src/game.h"

#define SQUARED_SIZE DEFAULT_SIZE*DEFAULT_SIZE

static square tdefault_sol[SQUARED_SIZE] = {
    S_ZERO, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO, S_ONE, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO,
    S_ONE, S_IMMUTABLE_ZERO, S_ZERO, S_ONE, S_IMMUTABLE_ZERO, S_ONE,
    S_ONE, S_IMMUTABLE_ZERO, S_IMMUTABLE_ONE, S_ZERO, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_ZERO, S_ONE, S_IMMUTABLE_ZERO,
    S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE, S_IMMUTABLE_ZERO
};

static square bad_sol1[] = {
    S_ZERO, S_ZERO, S_ZERO, S_ONE, S_ONE, S_ONE,
    S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO,
    S_ONE, S_ZERO, S_ZERO, S_ONE, S_ZERO, S_ONE,
    S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO,
    S_ZERO, S_ONE, S_ONE, S_ZERO, S_ZERO, S_ONE,
    S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO, S_ZERO
};
static square bad_sol2[] = {
    S_ZERO, S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE,
    S_ZERO, S_ONE, S_ZERO, S_ONE, S_ONE, S_ZERO,
    S_ONE, S_ZERO, S_ONE, S_ZERO, S_ZERO, S_ONE,
    S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO, S_ZERO,
    S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO
};
static square bad_sol3[] = {
    S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO, S_ONE, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO,
    S_ONE, S_IMMUTABLE_ZERO, S_ZERO, S_ONE, S_IMMUTABLE_ZERO, S_ONE,
    S_ONE, S_IMMUTABLE_ZERO, S_IMMUTABLE_ONE, S_ZERO, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_ZERO, S_ONE, S_IMMUTABLE_ZERO,
    S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE, S_IMMUTABLE_ZERO
};
static square bad_sol4[] = {
	S_ZERO, S_ONE, S_ONE,S_ZERO, S_ONE, S_ONE, S_ZERO, S_ZERO,
	S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO,S_ZERO, S_ONE, S_ONE,
	S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE,
	S_ONE, S_ZERO, S_ONE, S_ZERO, S_ONE, S_ONE, S_ZERO, S_ZERO,
	S_ONE, S_ZERO, S_ZERO, S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE
};
static square bad_sol5 [] = {
	S_ZERO, S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE,
	S_ONE, S_ZERO, S_ONE, S_ONE, S_ZERO, S_ZERO,
	S_ONE, S_ONE, S_ZERO, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE,
	S_ONE, S_ONE, S_ZERO, S_ZERO, S_ONE, S_ZERO,
	S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ONE, S_ZERO, S_ZERO, S_ONE,
	S_ZERO, S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE
};
static square bad_sol6 [] = {
	S_ONE, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ZERO, S_ONE,
	S_ONE, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ZERO, S_ONE
};
static square bad_sol7 [] = {
	S_ONE, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ZERO, S_ONE,
	S_ONE, S_ZERO, S_ONE, S_ZERO,
	S_ZERO, S_ONE, S_ZERO, S_ONE
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


int test_dummy() {
	return EXIT_SUCCESS;
}


int test_game_get_number(){
	game g = game_default_solution();
	assert(g);
	game_set_square(g,1,0,S_EMPTY);
	for(int y=0; y<DEFAULT_SIZE;y++){
		for(int x=0; x<DEFAULT_SIZE; x++){
			square s = game_get_square(g,y,x);
			int num = game_get_number(g,y,x);
			if( (num==-1 && s!=S_EMPTY) || 
				 (num==0 && s!=S_ZERO && s!=S_IMMUTABLE_ZERO) ||
				 (num==1 && s!=S_ONE && s!=S_IMMUTABLE_ONE)){
				return test_failed(1,g);
			}
		}
	}
	return test_success(1,g);
}


int test_game_is_empty(){
	game g = game_new_empty();
	game_set_square(g,1,0,S_ZERO);
	game_set_square(g,2,0,S_ONE);
	game_set_square(g,3,0,S_IMMUTABLE_ZERO);
	game_set_square(g,4,0,S_IMMUTABLE_ONE);
	if( !game_is_empty(g,0,0) || 
		 game_is_empty(g,1,0) ||
		 game_is_empty(g,2,0) ||
		 game_is_empty(g,2,0) ||
		 game_is_empty(g,2,0))
		return test_failed(1,g);
	return test_success(1,g);
}


int test_game_is_immutable(){
	game g = game_default();
	assert(g);
	for(int y=0;y<DEFAULT_SIZE;y++) {
		for(int x=0;x<DEFAULT_SIZE;x++) {
			square s = game_get_square(g,y,x);
			bool is_im = (s==S_IMMUTABLE_ONE || s==S_IMMUTABLE_ZERO);
			if( is_im && game_is_immutable(g,y,x)==false){
				return test_failed(1,g);
			}
			else if(!is_im && game_is_immutable(g,y,x)==true){
				return test_failed(1,g);
			}
		}
	}
	return test_success(1,g);
}


int test_game_check_move(){
	game g = game_default();
	assert(g);
	bool success = game_check_move(g,0,0,S_ONE)==true &&
		game_check_move(g,-1,3,S_EMPTY)==false &&
		game_check_move(g,2,-1,S_ZERO)==false &&
		game_check_move(g,0,1,S_EMPTY)==false &&
		game_check_move(g,0,0,S_IMMUTABLE_ZERO)==false &&
		game_check_move(g,0,0,S_IMMUTABLE_ONE)==false;
	if(success)
		return test_success(1,g);
	return test_failed(1,g);
}


int test_game_has_error(){
	game g = game_default();
	assert(g);
	for(int i=0;i<SQUARED_SIZE;i++){
		if(game_has_error(g,i/DEFAULT_SIZE,i%DEFAULT_SIZE)!=0){
			return test_failed(1,g);
		}
	}
	game_delete(g);
	g = game_default();
	assert(g);
	game_set_square(g,0,0,1);
	game_set_square(g,0,1,1);
	game_set_square(g,0,2,1);
	if(game_has_error(g,0,1) == 0)
		return test_failed(1,g);
	game g2 = game_new_empty_ext(6,6,true,false);
	assert(g2);
	game_set_square(g2,0,0,1);
	game_set_square(g2,0,1,1);
	game_set_square(g2,0,5,1);
	if(game_has_error(g2,0,1) == 0)
		return test_failed(2,g,g2);
	game_set_square(g2,0,0,1);
	game_set_square(g2,1,0,1);
	game_set_square(g2,5,0,1);
	if(game_has_error(g2,0,1) == 0)
		return test_failed(2,g,g2);
	game g3 = game_new_ext(4,4,bad_sol7,false,true);
	if(game_has_error(g3,0,0) == 0)
		return test_failed(3,g,g2,g3);
	game g4 = game_new_ext(4,4,bad_sol7,false,false);
	if(game_has_error(g4,0,0) != 0)
		return test_failed(4,g,g2,g3,g4);
	return test_success(4,g,g2,g3,g4);
}

int test_game_restart(){
	game g1 = game_default_solution();
	game g2 = game_default();
	assert(g1);
	assert(g2);
	game_restart(g1);
	if(!game_equal(g1,g2))
		return test_failed(2,g1,g2);
	game g3 = game_new_empty();
	game_play_move(g3,0,0,S_ZERO);
	game_play_move(g3,0,0,S_ONE);
	game_play_move(g3,0,0,S_ZERO);
	game_undo(g3);
	game_restart(g3);
	game_redo(g3);
	if(game_get_square(g3,0,0)!=S_EMPTY)
		return test_failed(3,g1,g2,g3);;
	game_undo(g3);
	if(game_get_square(g3,0,0)!=S_EMPTY)
		return test_failed(3,g1,g2,g3);
	return test_success(3,g1,g2,g3);
}


int test_game_get_next_number(){
	game g = game_default();
	assert(g);
	for(int i=0;i<DEFAULT_SIZE;i++){
		if ( game_get_next_number(g,0,i,UP,1)!=-1 ||
			 game_get_next_number(g,DEFAULT_SIZE-1,i,DOWN,1) !=-1 ||
			 game_get_next_number(g,i,0,LEFT,1)!=-1 ||
			 game_get_next_number(g,i,DEFAULT_SIZE-1,RIGHT,1)!=-1){
			return test_failed(1,g);
		}
	}
	for(int y=1; y<DEFAULT_SIZE-1; y++){
		for(int x=1;x<DEFAULT_SIZE-1; x++){
			if ( game_get_next_number(g,y,x,LEFT,1)!=game_get_number(g,y,x-1) || 
				 game_get_next_number(g,y,x,RIGHT,1)!=game_get_number(g,y,x+1) || 
				 game_get_next_number(g,y,x,DOWN,1)!=game_get_number(g,y+1,x) ||
				 game_get_next_number(g,y,x,UP,1)!=game_get_number(g,y-1,x)){
				return test_failed(1,g);
			}
		}
	}
	for(int y=2; y<DEFAULT_SIZE-2; y++){
		for(int x=2;x<DEFAULT_SIZE-2; x++){
			if ( game_get_next_number(g,y,x,LEFT,2)!=game_get_number(g,y,x-2) || 
				 game_get_next_number(g,y,x,RIGHT,2)!=game_get_number(g,y,x+2) || 
				 game_get_next_number(g,y,x,DOWN,2)!=game_get_number(g,y+2,x) ||
				 game_get_next_number(g,y,x,UP,2)!=game_get_number(g,y-2,x)){
				return test_failed(1,g);
			}
		}
	}
	game g2 = game_new_ext(DEFAULT_SIZE,DEFAULT_SIZE,tdefault_sol,true,false);
	for(int i=0;i<DEFAULT_SIZE;i++){
		if(game_get_next_number(g2,i,0,LEFT,1)!=game_get_number(g2,i,DEFAULT_SIZE-1))
			return test_failed(2,g,g2);
		if(game_get_next_number(g2,0,i,UP,1)!=game_get_number(g2,DEFAULT_SIZE-1,i))
			return test_failed(2,g,g2);
		if(game_get_next_number(g2,DEFAULT_SIZE-1,i,DOWN,1)!=game_get_number(g2,0,i))
			return test_failed(2,g,g2);
		if(game_get_next_number(g2,i,DEFAULT_SIZE-1,RIGHT,1)!=game_get_number(g2,i,0))
			return test_failed(2,g,g2);
	}
	return test_success(2,g,g2);
}


int test_game_get_next_square(){
	game g = game_default();
	assert(g);
	for(int i=0;i<DEFAULT_SIZE;i++){
		if ( game_get_next_square(g,0,i,UP,1)!=-1 ||
			 game_get_next_square(g,DEFAULT_SIZE-1,i,DOWN,1) !=-1 ||
			 game_get_next_square(g,i,0,LEFT,1)!=-1 ||
			 game_get_next_square(g,i,DEFAULT_SIZE-1,RIGHT,1)!=-1){
			return test_failed(1,g);
		}
	}
	for(int y=1; y<DEFAULT_SIZE-1; y++){
		for(int x=1;x<DEFAULT_SIZE-1; x++){
			if ( game_get_next_square(g,y,x,LEFT,1)!=game_get_square(g,y,x-1) || 
				 game_get_next_square(g,y,x,RIGHT,1)!=game_get_square(g,y,x+1) || 
				 game_get_next_square(g,y,x,DOWN,1)!=game_get_square(g,y+1,x) ||
				 game_get_next_square(g,y,x,UP,1)!=game_get_square(g,y-1,x)){
				return test_failed(1,g);
			}
		}
	}
	for(int y=2; y<DEFAULT_SIZE-2; y++){
		for(int x=2;x<DEFAULT_SIZE-2; x++){
			if ( game_get_next_square(g,y,x,LEFT,2)!=game_get_square(g,y,x-2) || 
				 game_get_next_square(g,y,x,RIGHT,2)!=game_get_square(g,y,x+2) || 
				 game_get_next_square(g,y,x,DOWN,2)!=game_get_square(g,y+2,x) ||
				 game_get_next_square(g,y,x,UP,2)!=game_get_square(g,y-2,x)){
				return test_failed(1,g);
			}
		}
	}
	game g2 = game_new_ext(DEFAULT_SIZE,DEFAULT_SIZE,tdefault_sol,true,false);
	for(int i=0;i<DEFAULT_SIZE;i++){
		if(game_get_next_square(g2,i,0,LEFT,1)!=game_get_square(g2,i,DEFAULT_SIZE-1))
			return test_failed(2,g,g2);
		if(game_get_next_square(g2,0,i,UP,1)!=game_get_square(g2,DEFAULT_SIZE-1,i))
			return test_failed(2,g,g2);
		if(game_get_next_square(g2,DEFAULT_SIZE-1,i,DOWN,1)!=game_get_square(g2,0,i))
			return test_failed(2,g,g2);
		if(game_get_next_square(g2,i,DEFAULT_SIZE-1,RIGHT,1)!=game_get_square(g2,i,0))
			return test_failed(2,g,g2);
	}
	return test_success(2,g,g2);
}


int test_game_play_move(){
	game g = game_default();
	assert(g);

	game_play_move(g,0,0,S_ZERO);
	if (game_get_square(g,0,0)!=S_ZERO)
		return test_failed(1,g);
	
	game_play_move(g,0,0,S_EMPTY);
	game_play_move(g,1,0,S_ONE);
	if (game_get_square(g,0,0)!=S_EMPTY || game_get_square(g,1,0)!=S_ONE)
		return test_failed(1,g);
	game_undo(g);
	if(game_get_square(g,1,0)!=S_EMPTY)
		return test_failed(1,g);
	game_undo(g);
	if(game_get_square(g,0,0)!=S_ZERO)
		return test_failed(1,g);
	return test_success(1,g);
}


int test_game_is_over(){
	game g1 = game_default_solution();
	assert(g1);
	if(!game_is_over(g1))
		return test_failed(1,g1);
	game_set_square(g1,0,0,S_EMPTY);
	if(game_is_over(g1))
		return test_failed(1,g1);
	game g2 = game_new(bad_sol1);
	if(game_is_over(g2))
		return test_failed(2,g1,g2);
	game g3 = game_new(bad_sol2);
	if(game_is_over(g3))
		return test_failed(3,g1,g2,g3);
	game g4 = game_new(bad_sol3);
	if(game_is_over(g4))
		return test_failed(4,g1,g2,g3,g4);
	game g5 = game_new_ext(8,6,bad_sol4,true,false); 
	if(game_is_over(g5))
		return test_failed(5,g1,g2,g3,g4,g5);
	game g6 = game_new_ext(6,8,bad_sol5,true,false);
	if(game_is_over(g6))
		return test_failed(6,g1,g2,g3,g4,g5,g6);
	game g7 = game_new_ext(4,4,bad_sol6,false,true);
	if(game_is_over(g7))
		return test_failed(7,g1,g2,g3,g4,g5,g6,g7);
	game g8 = game_new_ext(4,4,bad_sol7,false,true);
	if(game_is_over(g8))
		return test_failed(8,g1,g2,g3,g4,g5,g6,g7,g8);
	return test_success(8,g1,g2,g3,g4,g5,g6,g7,g8);
}


int main(int argc, char *argv[]) {
	if(argc==1)
		usage(argc, argv);
	
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	int exit_code = EXIT_FAILURE;
	
	if (strcmp("dummy", argv[1]) == 0)
    	exit_code = test_dummy();
	else if (strcmp("game_get_number", argv[1]) == 0)
    	exit_code = test_game_get_number();
	else if (strcmp("game_is_empty", argv[1]) == 0)
    	exit_code = test_game_is_empty();
	else if (strcmp("game_is_immutable", argv[1]) == 0)
    	exit_code = test_game_is_immutable();
	else if (strcmp("game_check_move", argv[1]) == 0)
    	exit_code = test_game_check_move();
	else if (strcmp("game_has_error", argv[1]) == 0)
    	exit_code = test_game_has_error();
	else if (strcmp("game_restart", argv[1]) == 0)
    	exit_code = test_game_restart();
	else if (strcmp("game_play_move", argv[1]) == 0)
    	exit_code = test_game_play_move();
	else if (strcmp("game_get_next_number", argv[1]) == 0)
    	exit_code = test_game_get_next_number();
	else if (strcmp("game_get_next_square", argv[1]) == 0)
    	exit_code = test_game_get_next_square();
	else if (strcmp("game_is_over", argv[1]) == 0)
    	exit_code = test_game_is_over();
	else {
		fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
	}
	
	char* s = "FAILURE";
	if(exit_code==EXIT_SUCCESS)s="SUCCESS";
	fprintf(stderr,"Test %s finished: %s\n", argv[1], s);
	return exit_code;
}
