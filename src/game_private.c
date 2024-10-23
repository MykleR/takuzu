#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"



// Retourne le signe d'un nombre, -1 si négatif, 1 si positif
static int sign(int x){
  return (x<0) ? -1 : 1;
}


int mod(int a, int b){
  if (b==0 || a==0)
      return 0;
  int r = a%abs(b);
  if (sign(a)!=sign(b) && r!=0)
      return r+b;
  return r;
}

bool is_empty_string(char* s){
  return s==NULL || *s=='\0' || *s=='\n';
}


void _crash(char *format, ...){
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  fprintf(stderr,"\n");
  va_end(args);
  exit(EXIT_FAILURE);
}


void _history_new_move(queue* q, move m){
  assert(q);
  move* new = (move*)malloc(sizeof(move));
  assert(new);
  *new = m;
  queue_push_tail(q, new);
}


move _history_pop_move(queue* q){
  assert(q);
  move* m = queue_pop_tail(q);
  assert(m);
  move result = *m;
  free(m);
  return result;
}


void _history_clear(queue* q){
  assert(q);
  queue_clear_full(q, free);
  assert(queue_is_empty(q));
}


void _dir_to_coords(direction dir, uint dist, int* d_x, int* d_y){
  int x_dir=0, y_dir=0;
  switch (dir){
      case UP:
          y_dir=-dist;break;
      case DOWN:
          y_dir=dist;break;
      case LEFT:
          x_dir=-dist;break;
      case RIGHT:
          x_dir=dist;break;
      default:break;
  }
  *d_x += x_dir;
  *d_y += y_dir;
}


char _square_to_char(square s, bool save_format){
  switch (s) {
    case S0: return 'w';
    case S1: return 'b';
    case SI0:return 'W';
    case SI1:return 'B';
    default: return save_format ? 'e' : ' ';
  }
}

square _char_to_square(char c){
  switch (c) {
    case 'w':return S0;
    case 'b':return S1;
    case 'W':return SI0;
    case 'B':return SI1;
    default: return SE;
  }
}


int _count_in_row(cgame g, uint i, int number) {
  assert(g);
  assert(i < game_nb_rows(g));
  int count = 0;
  for (uint j=0; j<game_nb_cols(g); j++)
    if(game_get_number(g, i, j)==number)
      count++;
  return count;
}


int _count_in_col(cgame g, uint j, int number) {
  assert(g);
  assert(j < game_nb_cols(g));
  int count = 0;
  for (uint i=0; i<game_nb_rows(g); i++)
    if(game_get_number(g, i, j)==number)
      count++;
  return count;
}


bool _three_consecutive(cgame g, uint i, uint j) {
  assert(g);
  assert(i < game_nb_rows(g));
  assert(j < game_nb_cols(g));

  int h0 = game_get_number(g, i, j);
  if (h0 == -1) return false;

  int l1 = game_get_next_number(g, i, j, LEFT, 1);
  int l2 = game_get_next_number(g, i, j, LEFT, 2);
  int r1 = game_get_next_number(g, i, j, RIGHT, 1);
  int r2 = game_get_next_number(g, i, j, RIGHT, 2);
  int u1 = game_get_next_number(g, i, j, UP, 1);
  int u2 = game_get_next_number(g, i, j, UP, 2);
  int d1 = game_get_next_number(g, i, j, DOWN, 1);
  int d2 = game_get_next_number(g, i, j, DOWN, 2);

  return (l2 == h0 && l1 == h0) ||
     (l1 == h0 && r1 == h0) ||
     (r1 == h0 && r2 == h0) ||
     (u2 == h0 && u1 == h0) ||
     (u1 == h0 && d1 == h0) ||
     (d1 == h0 && d2 == h0);
}


/* compare rows i0 and i1 and return true if they are equal. */
static bool _compare_row(cgame g, uint i0, uint i1) {
  assert(g);
  assert(i0 < game_nb_rows(g));
  assert(i1 < game_nb_rows(g));
  for (int j = 0; j < game_nb_cols(g); j++) {
    int val0 = game_get_number(g, i0, j);
    int val1 = game_get_number(g, i1, j);
    if (val0 == -1 || val1 ==-1 || val0!=val1)
        return false;
  }
  return true;
}


/* compare cols j0 and j1 and return true if they are equal. */
static bool _compare_col(cgame g, uint j0, uint j1) {
  assert(g);
  assert(j0 < game_nb_cols(g));
  assert(j1 < game_nb_cols(g));
  for (int i = 0; i < game_nb_rows(g); i++) {
    int val0 = game_get_number(g, i, j0);
    int val1 = game_get_number(g, i, j1);
    if (val0 == -1 || val1==-1 || val0!=val1)
        return false;
  }
  return true;
}


bool _unique_row(cgame g, uint i) {
  assert(g);
  assert(i < game_nb_rows(g));
  /* compare row i with row ii */
  for (uint ii=0; ii<game_nb_rows(g); ii++) {
    if (i == ii) continue;  // skip it
    if (_compare_row(g, i, ii))
        return false;
  }
  return true;
}


bool _unique_col(cgame g, uint j) {
  assert(g);
  assert(j < game_nb_cols(g));
  /* compare row j with row jj */
  for (uint jj=0; jj < game_nb_cols(g); jj++) {
    if (j == jj) continue;  // skip it
    if ( _compare_col(g, j, jj))
        return false;
  }
  return true;
}


uint _rec_game_nb_solutions(game g, uint pos){
    assert(g);
    if(pos>=game_nb_cols(g)*game_nb_rows(g))
        return 1;
    
    uint i=pos/game_nb_cols(g);
    uint j=pos%game_nb_cols(g);
    uint nb_solutions = 0;

    if(game_has_error(g,i,j))
      return false;
    if(!game_is_empty(g,i,j))
      return _rec_game_nb_solutions(g,pos+1);

    game_set_square(g,i,j,S_ONE);
    if(game_has_error(g,i,j)==0)
        nb_solutions += _rec_game_nb_solutions(g,pos+1);

    game_set_square(g,i,j,S_ZERO);
    if(game_has_error(g,i,j)==0)
        nb_solutions += _rec_game_nb_solutions(g,pos+1);
        
    game_set_square(g,i,j,S_EMPTY);
    return nb_solutions;
}

bool _rec_game_solve(game g, uint pos){
    assert(g);
    if(pos>=game_nb_cols(g)*game_nb_rows(g))
        return true;
    
    uint i=pos/game_nb_cols(g);
    uint j=pos%game_nb_cols(g);

    if(game_has_error(g,i,j))
      return false;
    if(!game_is_empty(g,i,j))
      return _rec_game_solve(g,pos+1);

    game_set_square(g,i,j,S_ONE);
    if(game_has_error(g,i,j)==0){
        if(_rec_game_solve(g,pos+1))
            return true;
    }

    game_set_square(g,i,j,S_ZERO);
    if(game_has_error(g,i,j)==0){
        if(_rec_game_solve(g,pos+1))
            return true;
    }
    game_set_square(g,i,j,S_EMPTY);
    return false;
}