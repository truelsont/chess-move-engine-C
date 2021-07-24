#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <time.h>

#include "board.h"
#include "list.h"

int search_depth = 2; 

void main(){
    printf("PRESS ANY KEY TO START"); 
while (1)
{
    //this is the main menu promppt
    /*
    while ((getchar()) != '\n');

    while (1){
        printf("CHESS ENGINE QUERY \n ... WANT TO PLAY (y/n)?");
        char scan = tolower(getchar()); 
        if(scan == 'y') {break;}
        if(scan == 'n') { 
            printf("CLOSING PROGRAM ..."); 
            return; 
        }
        printf("UNKNOWN INPUT"); 
    }

    while ((getchar()) != '\n');

    char player_turn;
    while (1)
    {
        printf("CHESS ENGINE QUERY \n ... SELECT COLOR (white/black/random)?");
        char input[20];
        scanf("%s", input); 
        input[20] = '\0'; //safety

        if(strcmp(input, "white") == 0){ player_turn = 'W'; break; }
        if(strcmp(input, "black") == 0){ player_turn = 'B'; break; }
        if(strcmp(input, "random") == 0){
            long lt = time(NULL);
            if(lt % 2 == 0) { player_turn = 'W'; break; }
            player_turn = 'B'; 
            break; 
        }
        printf("UKNOWN INPUT\n"); 
    }



    printf("Player is %c\n", player_turn); 
    printf("GAME START"); 
    */ 

    char player_turn = 'W'; 
    board *board = new_board(); 


    char input[9]; input[8] = '\0'; 

    while(1){
        list_t moves = gen_moves(board);   
        display_board(board); 
        if(list_empty(moves)){
            list_free(moves, &free_move); 
            printf("checkmate"); 
            break; 
        }

        //player turn logic 
        if(get_turn(board) == player_turn){
            printf("Player Move:");
            /*shows the moves
            while(!list_empty(moves)){
                char rep[8]; 
                move *temp = list_pop(moves); 
                move_string_rep(rep,temp); 
                free_move(temp); 
                printf("%s\n",rep); 
            }*/
            scanf("%s", input);  
            input[8] = '\0'; 
            if(strcmp(input, "quit") == 0){
                list_free(moves, &free_move); 
                break; 
            }
            int move_worked = apply_move_user(board, input); 
            if(move_worked == 0){
                printf("Invalid Move\n");
            }
            list_free(moves, &free_move); 
            continue; 
        }
        
        //computer turn logic
        if(get_turn(board) != player_turn){
            printf("calculating ...\n");
            move *computer_choice = computer_find_move(board, search_depth);
            make_move(board, computer_choice); 
            free_move(computer_choice); 
        }
        list_free(moves, &free_move); 
    }

    board_free(board); 
    return; 
}
    return; 
}


