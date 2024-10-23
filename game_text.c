#include <stdio.h>
#include <stdlib.h>
#include "src/game.h"
#include "src/game_aux.h"
#include "src/game_ext.h"
#include "src/game_tools.h"

//affichage des actions que le joueur peut effectuer
void game_help(){
    printf("- press 'w <i> <j>' to put a zero/white at square (i,j)\n");
    printf("- press 'b <i> <j>' to put a one/black at square (i,j)\n");
    printf("- press 'e <i> <j>' to put a empty square (i,j)\n");
    printf("- press 's' <filename> to save\n");
    printf("- press 'r' to restart\n");
    printf("- press 'q' to quit\n");
    printf("- press 'z' to undo\n");
    printf("- press 'y' to redo\n");
}

//boucle de l'exécution du jeu jusqu'à ce qu'il soit terminé
int main(int argc, char* argv[]) {
    game g = NULL;
    if (argc>=2){
        g = game_load(argv[1]);
    }
    else{
        g = game_default();
    }
    while (game_is_over(g) == false){
        //vérification qu'une case n'est pas mal placée
        for (int i=0; i < game_nb_rows(g); i++) {
            for (int j=0; j < game_nb_cols(g); j++) {
                if(game_has_error(g, i, j)){
                    printf("Error at %d, %d\n",i,j);
                }
            }
        }
        game_print(g);
        printf(" ? [h for help]\n");
        char c;
        if(scanf(" %c",&c)!=1)continue;
        
        //traitement des actions effectuées par l'utilisateur
        if(c=='h'){
            printf(" action: help\n");
            game_help();
        } else if (c=='s') {
            char s[50];
            if(scanf("%s",s)!=1)continue;
            printf(" action: save at '%s'\n",s);
            game_save(g,s);
        } else if (c=='r') {
            printf(" action: restart\n");
            game_restart(g);
        } else if (c=='q'){
            printf(" action: quit\nWhat a shame, you gave up :-(\n");
            game_delete(g);
            return EXIT_SUCCESS;
        } else if (c=='z'){
            printf(" action: undo\n");
            game_undo(g);
         }else if (c=='y'){
            printf(" action: redo\n");
            game_redo(g);

        }else if (c == 'w' || c=='b' || c=='e') {
            int i;
            int j;
            square state;
            if(scanf(" %d %d", &i, &j)!=1)continue;
            //assignation du type de square à placer
            if(c=='w'){
                state = S_ZERO;
            } else if (c=='b') {
                state = S_ONE;
            } else if (c=='e') {
                state = S_EMPTY;
            }
            //placer le square si c'est possible
            if (game_check_move(g, i, j, state)==true){
                game_play_move(g, i, j, state);
                printf(" action:  play move '%c' into square (%d,%d)\n", c, i, j);
            }
            else{
                printf(" cannot play move '%c' into square (%d,%d)\n", c, i, j);
            }
        }
    }
    //jeu terminé, sortie de boucle
    printf("Congratulation\n");
    game_delete(g);
    return EXIT_SUCCESS;
}