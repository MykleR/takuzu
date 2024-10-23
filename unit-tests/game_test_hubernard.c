#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/game.h"
#include "../src/game_ext.h"
#include "../src/game_aux.h"

#define SQUARED_SIZE DEFAULT_SIZE*DEFAULT_SIZE

void usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

bool  test_dummy(){
    return true;
}


bool test_game_new(){

  square gtest[SQUARED_SIZE];
  int t[]= {S_EMPTY, S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO};
  for (int i=0 ; i<SQUARED_SIZE; i++){
    gtest[i] = t[i%5];
  }  
  game g = game_new(gtest);
  assert(g);
  
  for (int i=0; i<SQUARED_SIZE; i++){
    int x = i%DEFAULT_SIZE;
    int y = i/DEFAULT_SIZE;
    if(game_get_square(g, y, x)!=gtest[i]){
      game_delete(g);
      return false;
    } 
  }
  game_delete(g);
  return true;
}

bool test_game_new_empty(){
  game g = game_new_empty();
  assert(g);
  for (int y=0; y < DEFAULT_SIZE ; y++){
    for (int x=0; x < DEFAULT_SIZE ; x++){
      if(game_get_square(g, x, y)!=S_EMPTY){
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_equal(){
  square gtest1[SQUARED_SIZE];
  square gtest2[SQUARED_SIZE];
  int t[]= {S_EMPTY, S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO};
  for (int i=0 ; i<SQUARED_SIZE; i++){
    gtest1[i] = t[i%5];
    gtest2[i] = t[i%5];
  }  
  game g1 = game_new(gtest1);
  game g2 = game_new(gtest2);
  assert(g1);
  assert(g2);
  if (game_equal(g1, g2)==false){
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g1,0,1,S_IMMUTABLE_ZERO);
  game_set_square(g2,0,1,S_ZERO);
  if (game_equal(g1,g2)==true){
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;  
}

bool test_game_copy(){
  square gtest1[SQUARED_SIZE];
  int t[]= {S_EMPTY, S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO};
  for (int i=0 ; i<SQUARED_SIZE; i++){
    gtest1[i] = t[i%5];
  }  
  game g1 = game_new_ext(4,6,gtest1,true,true);
  game g2 = game_copy(g1);
  assert(g1);
  assert(g2);
  if (!game_equal(g1, g2) || !game_is_unique(g2) || !game_is_wrapping(g2)){
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_game_delete() {
  square gtest1[SQUARED_SIZE];
  game g = game_new(gtest1);
  assert(g);
  game_delete(g);
  return true;
}

bool test_game_set_square() {
  square gtest[SQUARED_SIZE];
  int t[]= {S_EMPTY, S_ZERO, S_ONE, S_IMMUTABLE_ONE, S_IMMUTABLE_ZERO};
  for (int i=0 ; i<SQUARED_SIZE; i++){
    gtest[i] = t[i%5];
  }  
  game g = game_new(gtest);
  assert(g);
  for (int i=0; i<SQUARED_SIZE; i++){
    int x = i%DEFAULT_SIZE;
    int y = i/DEFAULT_SIZE;
    if((!game_get_square(g, x, y))==gtest[i]){
      game_delete(g);
      return false;
    } 
  }
  game_delete(g);
  return true;
}

bool test_game_get_square(){
  game g = game_new_empty();
  assert(g);
  game_set_square(g, 5 , 1, S_IMMUTABLE_ONE);
  if (game_get_square(g, 5, 1)!=S_IMMUTABLE_ONE){
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}


int main (int argc , char *argv[]){
    if (argc==1){
        usage(argc, argv);
    }
    // start test
    fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
    bool ok = false;
    if (strcmp("dummy", argv[1]) == 0) { 
      ok = test_dummy(); 
    } else if (strcmp("game_new", argv[1]) == 0) {
      ok = test_game_new();
    } else if (strcmp("game_new_empty", argv[1]) == 0) {
      ok = test_game_new_empty();
    } else if (strcmp ("game_equal", argv[1]) == 0) {
      ok = test_game_equal();
    } else if (strcmp ("game_copy", argv[1]) == 0) {
      ok = test_game_copy();
    } else if ( strcmp ("game_delete", argv[1]) == 0){
      ok = test_game_delete();
    } else if ( strcmp ("game_set_square", argv[1]) == 0){
      ok = test_game_set_square();
    } else if (strcmp("game_get_square", argv[1]) == 0){
      ok = test_game_get_square();
    } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}