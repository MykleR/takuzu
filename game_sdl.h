// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef GAME_GRAPHIC_H
#define GAME_GRAPHIC_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "src/game.h"

/* **************************************************************** */
     
#define APP_NAME "Takuzu-02i"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define DEFAULT_SCREEN_WIDTH 1280
#define DEFAULT_SCREEN_HEIGHT 720
#define DELAY 100
#define NB_BUTTONS 5

#define BACKGROUND "images/Background.jpg"
#define IMG_IMMUTABLE_B "images/imutable_black.png"
#define IMG_IMMUTABLE_W "images/imutable_blue.png"
#define IMG_W "images/blue.png"
#define IMG_B "images/black.png"
#define IMG_CELL "images/gridCell4.png"
#define IMG_ERROR1 "images/red_cross.png"
#define IMG_ERROR2 "images/red_horizontal_line.png"
#define IMG_ERROR3 "images/red_vertical_line.png"
#define IMG_BUTTON "images/yellow_postit_default.png"
#define IMG_BUTTON_HOVERED "images/yellow_postit.png"
#define IMG_BUTTON_SAVE "images/cbook.png"
#define IMG_BUTTON_SAVE_HOVERED "images/obook.png"
#define FONT_PENCIL "images/Pencil.ttf"
#define FONT_CHILD "images/kindergarten.ttf"
#define FONT_INK "images/Holidays_Homework.ttf"
#define FONT_STYLIZED "images/font.ttf"

/* **************************************************************** */

typedef struct Env_t Env;

typedef struct Button_t{
    bool hovered;
    SDL_Texture* bg;
    SDL_Texture* bg_hovered;
    SDL_Texture* text_render;
    void (*on_click)(Env*);
    SDL_Rect rect;
} Button;

typedef struct Grid_t{
    SDL_Texture* immutable_black;
    SDL_Texture* immutable_white;
    SDL_Texture* black;
    SDL_Texture* white;
    SDL_Texture* border;
    SDL_Texture* error1;
    SDL_Texture* error2;
    SDL_Texture* error3;
    uint cell_size;
    uint x;
    uint y;
} Grid;

struct Env_t {
  SDL_Texture* background;
  SDL_Texture* victory_text;
  bool gameover;
  game g;
  Grid* grid;
  Button** buttons;
};

/* **************************************************************** */
     
#ifdef __ANDROID__ 
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);
void render(SDL_Window* win, SDL_Renderer* ren, Env * env);
void clean(SDL_Window* win, SDL_Renderer* ren, Env * env);
bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);

const char* HELP_MESSAGE = ("- press 'w <i> <j>' to put a zero/white at square (i,j)\n\
- press 'b <i> <j>' to put a one/black at square (i,j)\n\
- press 'e <i> <j>' to put a empty square (i,j)\n\
- press 's' <filename> to save\n\
- press 'r' to restart\n\
- press 'q' to quit\n\
- press 'z' to undo\n\
- press 'y' to redo\n");

/* **************************************************************** */

#endif
