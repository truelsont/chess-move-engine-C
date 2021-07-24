
#ifndef _CHESS_BOARD_
#define _CHESS_BOARD_

#include <stdlib.h>

#include "list.h"

typedef struct chess_board board; 
typedef struct move move; 

board *new_board(); 

int material_advantage(board *board)
/*Will be material + for white */;

int sum_of_heurisitcs(board *board)
/*Will give the advantage of white in the board state */; 

char get_turn(board *board)
/* W or B */; 

void display_board(board *board); 

list_t gen_moves(board *board)
/*list of moves that are vaild */; 

void make_move(board *board, move *move); 

void unmake_move(board *board, move *move); 

bool equal_move(move *move1, move *move2); 

int hash_move(move *move); 

void free_move(void *move); 

void board_free(board *board); 

void move_string_rep(char *buffer, move *move_input); 

int apply_move_user(board *board, char *move_string); 

move *computer_find_move(board *current_board, int depth)
/*current_moves cannot be empty*/; 

#endif