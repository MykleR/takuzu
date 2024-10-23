// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game_sdl.h"

#include "src/game.h"
#include "src/game_ext.h"
#include "src/game_aux.h"
#include "src/game_tools.h"



static const SDL_Color DARK_GREY = {60,60,60,255};

/* **************************************************************** */
/* Usefull fonctions for purified code */

static int apply_scale(int val, uint size, uint ref){
  double scale = (double)size/ref;
  return scale*val;
}

int scale_X(int x, uint size){
  return apply_scale(x,size, DEFAULT_SCREEN_WIDTH);
}
int scale_Y(int y, uint size){
  return apply_scale(y,size, DEFAULT_SCREEN_HEIGHT);
}

int scale_XY(int val, uint w, uint h){
  double scale = (double)(w+h)/(DEFAULT_SCREEN_HEIGHT+DEFAULT_SCREEN_WIDTH);
  return scale*val;
}

SDL_Rect scale_rect(SDL_Rect rect, uint w, uint h, bool ratio_preservation){
  SDL_Rect result;
  result.x = scale_X(rect.x, w);
  result.y = scale_Y(rect.y, h);
  if(ratio_preservation){
    result.w = scale_XY(rect.w, w, h);
    result.h = scale_XY(rect.h, w, h);
    return result;
  }
  result.w = scale_X(rect.w, w);
  result.h = scale_Y(rect.h, h);
  return result;
}


/* **************************************************************** */

Grid* grid_create(uint size, uint x, uint y){
  Grid* grid = (Grid*)malloc(sizeof(Grid));
  assert(grid),
  grid->cell_size = size;
  grid->x = x;
  grid->y = y;
  return grid;
}

bool grid_find_cell(Grid* grid, uint x, uint y,
     uint nb_cols, uint nb_rows,
     uint screen_width, uint screen_height,
     uint* outi, uint* outj) {
  int grid_size = scale_X(grid->cell_size, screen_width);
  int grid_x = scale_X(grid->x, screen_width);
  int grid_y = scale_Y(grid->y, screen_height);
  if(x<grid_x || y<grid_y || x>grid_x+grid_size*nb_cols || y>grid_y+grid_size*nb_rows)
    return false;
  *outi = (y-grid_y)/grid_size;
  *outj = (x-grid_x)/grid_size;
  return true;
}

void grid_process(Grid* grid, game g, int x, int y, uint w, uint h){
  uint i,j;
  uint nb_cols = game_nb_cols(g);
  uint nb_rows = game_nb_rows(g);
  if(grid_find_cell(grid, x, y, nb_cols, nb_rows, w, h, &i, &j)){
    square playing;
    switch (game_get_number(g, i, j)) {
      case -1: playing = S_ZERO; break;
      case 0:  playing = S_ONE; break;
      default: playing = S_EMPTY;
    }
    if(game_check_move(g,i,j,playing))
      game_play_move(g,i,j,playing);
  }
}


void grid_render(SDL_Renderer *ren, SDL_Window *win, Grid* grid, game g){
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  int size = scale_X(grid->cell_size, w);
  int x = scale_X(grid->x, w);
  int y = scale_Y(grid->y, h);

  for(int i=0; i<game_nb_rows(g); i++)
  {
    for(int j=0; j<game_nb_cols(g); j++) 
    {
      SDL_Rect cell = {x+j*size,y+i*size,size,size};
      SDL_RenderCopy(ren,grid->border,NULL,&cell);
      square s = game_get_square(g,i,j);
      if(s==S_EMPTY) continue;
      SDL_Texture* img;
      if(s==S_IMMUTABLE_ONE)
        img = grid->immutable_black;
      else if(s==S_IMMUTABLE_ZERO)
        img = grid->immutable_white;
      else if(s==S_ZERO)
        img = grid->white;
      else if(s==S_ONE)
        img = grid->black;
      SDL_RenderCopy(ren, img, NULL, &cell);
      int code = game_has_error(g,i,j);
      if(code==0) continue;
      if(code==1 || code==2 || code==3 || code==4) 
        img = grid->error1;
      if(code==5) 
        img = grid->error2;
      if(code==6)
        img = grid->error3;
      SDL_RenderCopy(ren, img, NULL, &cell);
    }
  }
}


void grid_delete(Grid* grid){
  if(grid==NULL)
    return;
  SDL_DestroyTexture(grid->border);
  SDL_DestroyTexture(grid->black);
  SDL_DestroyTexture(grid->white);
  SDL_DestroyTexture(grid->error1);
  SDL_DestroyTexture(grid->error2);
  SDL_DestroyTexture(grid->error3);
  SDL_DestroyTexture(grid->immutable_black);
  SDL_DestroyTexture(grid->immutable_white);
  free(grid);
}

/* **************************************************************** */

SDL_Texture* text_create(SDL_Renderer* ren, char* txt, char* font_path, uint font_size, SDL_Color color){
  TTF_Font* font = TTF_OpenFont(font_path, font_size);
  assert(font);
  SDL_Surface* surf = TTF_RenderText_Blended(font,txt,color);
  assert(surf);
  SDL_Texture* result = SDL_CreateTextureFromSurface(ren, surf);
  assert(result);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return result;
}


Button* button_create(SDL_Renderer* ren, uint x, uint y, uint w, uint h,
     SDL_Texture* bg, SDL_Texture* bg_hovered, SDL_Texture* text,
     void (*on_click)(Env*)){
  Button* result = malloc(sizeof(Button));
  assert(result);
  result->bg = bg;
  result->bg_hovered = bg_hovered;
  result->on_click = on_click;
  result->rect.x = x;
  result->rect.y = y;
  result->rect.w = w;
  result->rect.h = h;
  result->text_render = text;
  result->hovered = false;
  return result;
}

void button_render(SDL_Renderer *ren, SDL_Window *win, Button* button) {
  if(button==NULL)
    return;
  
  int w,h;
  SDL_GetWindowSize(win,&w,&h);

  SDL_Rect draw_rect = scale_rect(button->rect,w,h,true);
  if(button->hovered)
    SDL_RenderCopy(ren, button->bg_hovered, NULL, &draw_rect);
  else
    SDL_RenderCopy(ren, button->bg, NULL, &draw_rect);
  
  SDL_Rect text_rect;
  SDL_QueryTexture(button->text_render, NULL, NULL, &text_rect.w, &text_rect.h);
  text_rect.w = scale_XY(text_rect.w,w,h);
  text_rect.h = scale_XY(text_rect.h,w,h);
  text_rect.x = draw_rect.x + draw_rect.w/2 - text_rect.w/2;
  text_rect.y = draw_rect.y + draw_rect.h/2 - text_rect.h/2;
  SDL_RenderCopy(ren, button->text_render, NULL, &text_rect);

}

bool button_collide(Button* button, int x, int y, int w, int h){
  if(button==NULL)
    return false;
  SDL_Rect rect = scale_rect(button->rect,w,h,true);
  return x>rect.x && x<rect.x+rect.w && y>rect.y && y<rect.y+rect.h;
}

void button_delete(Button* button){
  if(button==NULL)
    return;
  SDL_DestroyTexture(button->bg);
  SDL_DestroyTexture(button->bg_hovered);
  SDL_DestroyTexture(button->text_render);
  free(button);
}


void on_button_undo(Env* env) {game_undo(env->g);}
void on_button_redo(Env* env) {game_redo(env->g);}
void on_button_reset(Env* env) {game_restart(env->g);}
void on_button_solve(Env* env) {env->gameover = game_solve(env->g);}
// void on_button_save(Env* env, int argc, char *argv[]) {
//   if(argc==2)
//     game_save(env->g, argv[1]);
//   else
//     printf("Cannot save the game");
//   }

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

    if (argc>=2)
        env->g = game_load(argv[1]);
    else
        env->g = game_default();
        
  env->gameover = false;
  SDL_Color test = {60,60,60,255};
  env->victory_text = text_create(ren,"Victory",FONT_STYLIZED,130,test);

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int size = (w+h)/(game_nb_cols(env->g)+game_nb_rows(env->g))/2;
  int x = w/2 - scale_XY(size,w,h)*(game_nb_cols(env->g))/2;
  env->grid = grid_create(size,x,50);

  SDL_Texture* button = IMG_LoadTexture(ren,IMG_BUTTON);
  SDL_Texture* button_hovered = IMG_LoadTexture(ren,IMG_BUTTON_HOVERED);
  SDL_Texture* button_save = IMG_LoadTexture(ren,IMG_BUTTON_SAVE);
  SDL_Texture* button_save_hovered = IMG_LoadTexture(ren,IMG_BUTTON_SAVE_HOVERED);
  env->buttons = calloc(sizeof(Button*), NB_BUTTONS);
  env->buttons[0] = button_create(ren,1000,150,150,150,button,button_hovered,
     text_create(ren,"Undo",FONT_INK,30,DARK_GREY), on_button_undo);
  env->buttons[1] = button_create(ren,1000,350,150,150, button,button_hovered, 
     text_create(ren,"Redo",FONT_INK,30,DARK_GREY), on_button_redo);
  env->buttons[2] = button_create(ren,100,75,150,150, button,button_hovered,
     text_create(ren,"Reset",FONT_PENCIL,40,DARK_GREY), on_button_reset);
  env->buttons[3] = button_create(ren,100,275,150,150, button,button_hovered,
     text_create(ren,"Solve",FONT_CHILD,40,DARK_GREY), on_button_solve);
  env->buttons[4] = button_create(ren,25,400,300,300, button_save,button_save_hovered,
     text_create(ren,"Save",FONT_CHILD,40,DARK_GREY), on_button_solve);

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  env->grid->border = IMG_LoadTexture(ren, IMG_CELL);
  env->grid->error1 = IMG_LoadTexture(ren, IMG_ERROR1);
  env->grid->error2 = IMG_LoadTexture(ren, IMG_ERROR2);
  env->grid->error3 = IMG_LoadTexture(ren, IMG_ERROR3);
  env->grid->black = IMG_LoadTexture(ren, IMG_B);
  env->grid->white = IMG_LoadTexture(ren, IMG_W);
  env->grid->immutable_white = IMG_LoadTexture(ren, IMG_IMMUTABLE_W);
  env->grid->immutable_black = IMG_LoadTexture(ren, IMG_IMMUTABLE_B);

/* Error while loading struct env fields */
  if (!button) ERROR("IMG_LoadTexture: %s\n", IMG_BUTTON);
  if (!button_hovered) ERROR("IMG_LoadTexture: %s\n", IMG_BUTTON_HOVERED);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);
  if (!env->grid->border) ERROR("IMG_LoadTexture: %s\n", IMG_CELL);
  if (!env->grid->error1) ERROR("IMG_LoadTexture: %s\n", IMG_ERROR1);
  if (!env->grid->error2) ERROR("IMG_LoadTexture: %s\n", IMG_ERROR2);
  if (!env->grid->error3) ERROR("IMG_LoadTexture: %s\n", IMG_ERROR3);
  if (!env->grid->black) ERROR("IMG_LoadTexture: %s\n", IMG_B);
  if (!env->grid->white) ERROR("IMG_LoadTexture: %s\n", IMG_W);
  if (!env->grid->immutable_black) ERROR("IMG_LoadTexture: %s\n", IMG_IMMUTABLE_B);
  if (!env->grid->immutable_white) ERROR("IMG_LoadTexture: %s\n", IMG_IMMUTABLE_W);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  SDL_RenderCopy(ren, env->background, NULL, NULL);
  grid_render(ren,win,env->grid,env->g);
  for(int i=0;i<NB_BUTTONS;i++)
    button_render(ren,win,env->buttons[i]);
  if(env->gameover){
    SDL_Rect draw_rect;
    SDL_QueryTexture(env->victory_text, NULL, NULL, &draw_rect.w, &draw_rect.h);
    draw_rect.w = scale_X(draw_rect.w,w);
    draw_rect.h = scale_Y(draw_rect.h,h);
    draw_rect.x = w / 2 - draw_rect.w / 2;
    draw_rect.y = scale_Y(env->grid->y, h) + scale_X(env->grid->cell_size, w)*game_nb_rows(env->g);
    SDL_RenderCopy(ren, env->victory_text, NULL, &draw_rect);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT)
    return true;

  int w, h, mousex,mousey;
  SDL_GetWindowSize(win, &w, &h);
  SDL_GetMouseState(&mousex, &mousey);
  
  if (e->type == SDL_MOUSEMOTION)
  {
    for(int i=0;i<NB_BUTTONS;i++){
      if(button_collide(env->buttons[i],mousex,mousey,w,h))
        env->buttons[i]->hovered=true;
      else
        env->buttons[i]->hovered=false;
    }
  }
  else if (e->type == SDL_MOUSEBUTTONDOWN)
  {
    if(!env->gameover)
      grid_process(env->grid,env->g,mousex,mousey,w,h);
    for(int i=0;i<NB_BUTTONS;i++){
      if(button_collide(env->buttons[i],mousex,mousey,w,h))
        env->buttons[i]->on_click(env);
    }
    env->gameover = game_is_over(env->g);
  }
  else if(e->type==SDL_KEYDOWN)
  {
    if (e->key.keysym.sym == SDLK_h)
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "help pannel",HELP_MESSAGE, NULL);
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  for(int i=0;i<NB_BUTTONS;i++)
    button_delete(env->buttons[i]);
  free(env->buttons);
  grid_delete(env->grid);
  game_delete(env->g);
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->victory_text);
  free(env);
}

/* **************************************************************** */

int main(int argc, char* argv[]) {
  /* initialize SDL2 and some extensions */
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    ERROR("Error: SDL_Init VIDEO (%s)", SDL_GetError());
  if (IMG_Init(IMG_INIT_PNG & IMG_INIT_PNG) != IMG_INIT_PNG)
    ERROR("Error: IMG_Init PNG (%s)", SDL_GetError());
  if (TTF_Init() != 0) ERROR("Error: TTF_Init (%s)", SDL_GetError());

  /* create window and renderer */
  SDL_Window* win = SDL_CreateWindow(
      APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
      SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!win) ERROR("Error: SDL_CreateWindow (%s)", SDL_GetError());
  SDL_Renderer* ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  if (!ren) ERROR("Error: SDL_CreateRenderer (%s)", SDL_GetError());

  /* initialize your environment */
  Env* env = init(win, ren, argc, argv);

  /* main render loop */
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    /* manage events */
    while (SDL_PollEvent(&e)) {
      /* process your events */
      quit = process(win, ren, env, &e);
      if (quit) break;
    }

    /* background in gray */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    /* render all what you want */
    render(win, ren, env);
    SDL_RenderPresent(ren);
    SDL_Delay(DELAY);
  }

  /* clean your environment */
  clean(win, ren, env);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}