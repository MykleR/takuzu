#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include "../src/game_aux.h"
#include "../src/game.h"

static square tdefault[] = {
    S_EMPTY, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO, S_EMPTY, S_EMPTY, S_EMPTY,
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY,
    S_EMPTY, S_IMMUTABLE_ZERO, S_EMPTY, S_EMPTY, S_IMMUTABLE_ZERO, S_EMPTY,
    S_EMPTY, S_IMMUTABLE_ZERO, S_IMMUTABLE_ONE, S_EMPTY, S_EMPTY, S_EMPTY,
    S_EMPTY, S_EMPTY, S_IMMUTABLE_ONE, S_EMPTY, S_EMPTY, S_IMMUTABLE_ZERO,
    S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, S_EMPTY, S_IMMUTABLE_ZERO
};

static square tdefault_sol[] = {
    S_ZERO, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO, S_ONE, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_ONE, S_ZERO, S_ONE, S_ZERO,
    S_ONE, S_IMMUTABLE_ZERO, S_ZERO, S_ONE, S_IMMUTABLE_ZERO, S_ONE,
    S_ONE, S_IMMUTABLE_ZERO, S_IMMUTABLE_ONE, S_ZERO, S_ZERO, S_ONE,
    S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_ZERO, S_ONE, S_IMMUTABLE_ZERO,
    S_ONE, S_ZERO, S_ZERO, S_ONE, S_ONE, S_IMMUTABLE_ZERO
};

bool test_result(bool success, int argc, ...){
    va_list args;
    va_start(args,argc);
    for(int i=0; i<argc; i++){
        game g = va_arg(args,game);
        game_delete(g);
    }
    va_end(args);
    return success;
}

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

bool test_dummy() {
    return true;
}

bool test_game_print() {
    game g = game_default();
    game_print(g);
    return test_result(true,1,g);
}

bool test_game_default() {
    game g1 = game_new(tdefault);
    game g2 = game_default();
    assert(g1);
    assert(g2);
    return test_result(game_equal(g1, g2),2,g1,g2);
}

bool test_game_default_solution() {
    game sol1 = game_new(tdefault_sol);
    game sol2 = game_default_solution();
    assert(sol1);
    assert(sol2);
    return test_result(game_equal(sol1, sol2),2,sol1,sol2);
}

int main(int argc, char *argv[]) {
	if(argc<=1)
		usage(argc, argv);
	
	fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
	bool ok = false;

    if( strcmp("dummy",argv[1])==0){
        ok = test_dummy();
    }
    else if(strcmp("game_print", argv[1])==0){
        ok = test_game_print();
    }
    else if(strcmp("game_default", argv[1])==0){
        ok = test_game_default();
    }
    else if(strcmp("game_default_solution", argv[1])==0){
        ok = test_game_default_solution();
    }
    else{
        fprintf(stderr,"Error: test \"%s\" not found\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!ok){
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
    else {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
}