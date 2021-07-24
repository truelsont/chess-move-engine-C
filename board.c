#ifndef _CHESS_BOARD_
#define _CHESS_BOARD_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

// returns -1 if not on board or in range [0.64) if on board]
int idx(int row, int col)
{
    if (row >= 0 && row < 8 && col < 8 && col >= 0)
    {
        return 8 * row + col;
    }
    return -1;
}

int get_row(int idx)
{
    return idx / 8;
}

int get_col(int idx)
{
    return idx % 8;
}


/* 
Will return -1 if it is not a col 
or will return a positive int for the 
col 
*/
char col_to_char(int col){
    if(col == 0){return 'A'; }
    if(col == 1){return 'B';}
    if(col == 2){return 'C'; }
    if(col == 3){return 'D'; }
    if(col == 4){return 'E';}
    if(col ==5){return 'F'; }
    if(col ==6){return 'G'; }
    if(col ==7){return 'H'; }
    return '\0'; 
}

//denbug
void print_bit_board(int *board)
{
    if (board == NULL)
    {
        return;
    }
    for (int i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            printf("\n| ");
        }
        printf("%d |", board[i]);
    }
    printf("\n");
    return;
}

struct piece_header
{
    int board_idx;
    bool taken; // 0 false, else true
    int type;  // K, Q, B, P, R, N
    char color;
};
typedef struct piece_header piece;

piece *new_piece(int row, int col, char type, char color)
{
    piece *ret = malloc(sizeof(piece));
    ret->board_idx = idx(row, col);
    ret->taken = false; 
    ret->type = type;
    ret->color = color;
    return ret;
}

struct board_header
{
    char turn;
    piece **pieces;
    int an_pessant; /*gives idx of where the an-pessant is */
};
typedef struct board_header board;

board *new_board()
{
    board *ret = malloc(sizeof(board));
    ret->turn = 'W';
    ret->an_pessant = -1;

    piece **pieces = calloc(32, sizeof(piece *));
    //White
    pieces[0] = new_piece(7, 0, 'R', 'W');
    pieces[1] = new_piece(7, 1, 'N', 'W');
    pieces[2] = new_piece(7, 2, 'B', 'W');
    pieces[3] = new_piece(7, 3, 'Q', 'W');
    pieces[4] = new_piece(7, 4, 'K', 'W');
    pieces[5] = new_piece(7, 5, 'B', 'W');
    pieces[6] = new_piece(7, 6, 'N', 'W');
    pieces[7] = new_piece(7, 7, 'R', 'W');
    pieces[8] = new_piece(6, 0, 'P', 'W');
    pieces[9] = new_piece(6, 1, 'P', 'W');
    pieces[10] = new_piece(6, 2, 'P', 'W');
    pieces[11] = new_piece(6, 3, 'P', 'W');
    pieces[12] = new_piece(6, 4, 'P', 'W');
    pieces[13] = new_piece(6, 5, 'P', 'W');
    pieces[14] = new_piece(6, 6, 'P', 'W');
    pieces[15] = new_piece(6, 7, 'P', 'W');

    //Black
    pieces[16] = new_piece(0, 0, 'R', 'B');
    pieces[17] = new_piece(0, 1, 'N', 'B');
    pieces[18] = new_piece(0, 2, 'B', 'B');
    pieces[19] = new_piece(0, 3, 'Q', 'B');
    pieces[20] = new_piece(0, 4, 'K', 'B');
    pieces[21] = new_piece(0, 5, 'B', 'B');
    pieces[22] = new_piece(0, 6, 'N', 'B');
    pieces[23] = new_piece(0, 7, 'R', 'B');
    pieces[24] = new_piece(1, 0, 'P', 'B');
    pieces[25] = new_piece(1, 1, 'P', 'B');
    pieces[26] = new_piece(1, 2, 'P', 'B');
    pieces[27] = new_piece(1, 3, 'P', 'B');
    pieces[28] = new_piece(1, 4, 'P', 'B');
    pieces[29] = new_piece(1, 5, 'P', 'B');
    pieces[30] = new_piece(1, 6, 'P', 'B');
    pieces[31] = new_piece(1, 7, 'P', 'B');

    ret->pieces = pieces; //pieces;
    return ret;
}

struct move_header
{
    int idx_from;
    int idx_to;
    int id_piece;
    int id_take;
    char prom;
}; //special case if prom is 'a' then that denotes an_pessant
typedef struct move_header move;

int material_advantage(board *board)
{
    if (board == NULL)
    {
        return 0;
    }
    int ret = 0;
    for (int i = 0; i < 32; i++)
    {
        piece *current = board->pieces[i];
        if (current == NULL || current->taken)
        {
            continue;
        }
        int coeff = 1; 
        if(current->color == 'B'){coeff = -1; }
        int value = 0; 
        switch (current->type)
        {
        case 'Q':
            value = 9; 
            break;
        case 'P':
            value = 1; 
            break; 
        case 'R':
            value = 5; 
            break; 
        case 'B':
            value = 3; 
            break; 
        case 'N': 
            value = 3; 
            break; 
        default:
            value = 0; 
            break;
        }
        ret += (value * coeff); 
    }
    return ret;
}



/* 
Heuristics for search
will be in the range of [-200,200]

a negative number implies a advantage in 
game state to black and a posotive one implies 
an advantage for white 

If there is a number outside of this
rnage it denotes the vicotry condition for the 
two colors i.e if it it white then 
it will positive out the range
*/ 
int sum_of_heurisitcs(board *board){
    int ret = 0; 
    ret += material_advantage(board); 
    return ret; 
}

void display_board(board *board)
{ 
    if (board == NULL)
    {
        return;
    }

    char col_letters[8] = {'A','B','C','D','E','F','G','H'};
    int indices[64];
    for (int i = 0; i < 64; i++)
    {
        indices[i] = -1;
    }

    for (int i = 0; i < 32; i++)
    {
        piece *current = board->pieces[i];
        if (current == NULL || current->taken)
        {
            continue;
        }
        indices[current->board_idx] = i;
    }

    for (int i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            int col = 8 - (i/8); 
            printf("\n\n %d |", col);
        }

        int idx = indices[i];
        if (idx < 0)
        {
            printf(" ___ |");
            continue;
        }

        piece *current = (board->pieces)[idx];
        printf(" %c-%c |", current->color, current->type);
    }
    printf("\n    "); 
    for(int i = 0; i < 8; i++){
        printf("  %c   ", col_letters[i]); 
    }
    printf("\n Material Advantage : %d \n Turn : %c\n", material_advantage(board), board->turn);
    return;
}

void board_free(board *B)
{
    if (B == NULL)
    {
        return;
    }

    for (int i = 0; i < 32; i++)
    {
        piece *current = B->pieces[i];

        if (current == NULL)
        {
            continue;
        }
        free(current);
    }

    free(B->pieces);
    free(B);
    return;
}

char get_turn(board *B)
{
    if (B == NULL)
    {
        return 0;
    }
    return B->turn;
}

/* NOW WE GET THE MOVES prepare */

piece *get_current_king(board *board)
{
    if (board->turn == 'B')
        return board->pieces[20];
    return board->pieces[4];
}
/*init a board with the init value oneach square */
int *bit_board(int init_value)
{
    int *ret = calloc(64, sizeof(int));
    for (int i = 0; i < 64; i++)
    {
        ret[i] = init_value;
    }
    return ret;
}

int *occ_board(board *board)
{
    int *ret = bit_board(-1);

    for (int i = 0; i < 32; i++)
    {
        piece *current = board->pieces[i];
        if (current == NULL || current->taken)
        {
            continue;
        }
        ret[current->board_idx] = i;
    }
    return ret;
}

/*Given a piece finds the squares it can attack, the offest
is a positive number which is used for if the attacked squares
should include the piece or not which is useful for other computations */
void attacked_squares(piece *current, board *board, int *occ_board,
                      int off_set, int *ret)
{
    if (current == NULL)
    {
        return;
    }
    if (current->taken)
    {
        return;
    }

    int row0 = get_row(current->board_idx);
    int col0 = get_col(current->board_idx);

    switch (current->type)
    {
    case 'K':
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }
                int index_to = idx(row0 + i, col0 + j);
                if (index_to < 0)
                {
                    continue;
                }
                ret[index_to] += 1;
            }
        }
        if (off_set == 0)
        {
            ret[current->board_idx] += 1;
        }
        break;
    case 'Q':
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }
                for (int t = 1; t < 8; t++)
                {
                    int index_to = idx(row0 + i * t, col0 + j * t);
            
                    if (index_to < 0)
                    {
                        break;
                    }
                    int id = occ_board[index_to];
                    if (id >= 0)
                    {
                        ret[index_to] += 1;
                        break;
                    }
                    ret[index_to] += 1;
                }
            }
        }
        if(off_set == 0){ret[idx(row0,col0)] += 1; }

        break;
    case 'B':
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }
                if (i == 0 || j == 0)
                {
                    continue;
                }
                for (int t = 1; t < 8; t++)
                {
                    int index_to = idx(row0 + i * t, col0 + j * t);
                    if (index_to < 0)
                    {
                        break;
                    }
                    int id = occ_board[index_to];
                    if (id >= 0)
                    {
                        ret[index_to] += 1;
                        break;
                    }
                    ret[index_to] += 1;
                }
            }
        }
         if(off_set == 0){ret[idx(row0,col0)] += 1; }

        break;
    case 'N':;

        int indices[8];
        indices[0] = idx(row0 + 2, col0 + 1);
        indices[1] = idx(row0 + 2, col0 - 1);
        indices[2] = idx(row0 - 2, col0 + 1);
        indices[3] = idx(row0 - 2, col0 - 1);
        indices[4] = idx(row0 - 1, col0 + 2);
        indices[5] = idx(row0 + 1, col0 + 2);
        indices[6] = idx(row0 - 1, col0 - 2);
        indices[7] = idx(row0 + 1, col0 - 2);

        for (int i = 0; i < 8; i++)
        {
            int index_to = indices[i];

            if (index_to < 0)
            {
                continue;
            }
            ret[index_to] += 1;
        }
        if (off_set == 0)
        {
            ret[current->board_idx] += 1;
        }

        break;
    case 'R':
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                {
                    continue;
                }
                if (i != 0 && j != 0)
                {
                    continue;
                }
                for (int t = 1; t < 8; t++)
                {
                    int index_to = idx(row0 + i * t, col0 + j * t);
                    if (index_to < 0)
                    {
                        break;
                    }
                    int id = occ_board[index_to];
                    if (id >= 0)
                    {
                        ret[index_to] += 1;
                        break;
                    }
                    ret[index_to] += 1;
                }
            }
        }
         if(off_set == 0){ret[idx(row0,col0)] += 1; }
        break;
    case 'P':;
        int coeff = -1;
        if (current->color == 'B')
        {
            coeff = 1;
        }
        for (int i = -1; i < 3; i = i + 2)
        {
            int index_to = idx(row0 + coeff, col0 + i);
            if (index_to < 0)
            {
                continue;
            }
            ret[index_to] += 1;
        }
        if (off_set == 0)
        {
            ret[current->board_idx] += 1;
        }

        break;

    default:
        break;
    }
    return;
}

/*array access by id, if null is not pinned else will have a 
bit board with the squares you can attack as a positive */
int **pin_squares(board *board, int *occ_board)
{
    int **ret = calloc(32, sizeof(int *));
    int king_idx = get_current_king(board)->board_idx;
    int row0 = get_row(king_idx); 
    int col0 = get_col(king_idx); 

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i == 0 && j == 0) {continue; }

            int *squares = bit_board(-1); 
            piece *pinned_piece = NULL; 
            int pinned_id = -1;  
            for(int t = 1; t <= 8; t++){
                int index = idx(row0 + i * t , col0 + j * t); 
                if(index < 0){ free(squares) ; break; }

                int id = occ_board[index]; 
                if(id < 0){ 
                    if(pinned_piece != NULL){
                        squares[index] = 1;  
                    } 
                    continue; 
                }
                piece *found = board->pieces[id]; 
                if(pinned_piece == NULL){
                    if(found->color == board->turn){
                        pinned_piece = found; 
                        pinned_id = id;
                        continue;
                    }
                    free(squares); 
                    break; 
                }

                if(pinned_piece != NULL){
                    if(found->color != board->turn && (
                        found->type == 'Q' ||
                        (found->type == 'R' && (i == 0 || j == 0)) ||
                        (found->type == 'B' && (i != 0 && j != 0)) )
                    ){
                        ret[pinned_id] = squares;  
                        break;
                    }
                    free(squares); 
                    break; 
                }

                free(squares); 
                break; 
                
            }
        }
    }
    return ret; 
}



//not safe but is usable for small ints
int abs_unit(int x){
    if(x == 0){return 0; }
    if(x < 0){ return -1; }
    return 1; 
}

/*Given we know that a given piece is checking tell which squares are critical */
void collapse_check_attacks(int *attacks, piece *attacker, piece *king){
    int king_idx = king->board_idx; 
    int check_idx = attacker->board_idx; 

    int a[64]; for(int i = 0; i < 64; i++){
        a[i] = -1; 
    }
    //find vector of direction if they are not equal in magnitude 
    //then it is a knight the math works out when we do the and 
    //operation on the boards 
    int i_hat = abs_unit(get_row(king_idx) - get_row(check_idx)); 
    int j_hat = abs_unit(get_col(king_idx) - get_col(check_idx)); 

    for(int t = 1; t < 8; t++){
        int index = idx(get_row(check_idx) + i_hat * t, get_col(check_idx) + j_hat *t
        ); 
        if(index < 0){break; }
        a[index] = 0; 
        if(index == king_idx){ break; }
    }

    //merge the two
    for(int i = 0; i < 64; i++){
        if(a[i] >= 0 && attacks[i] >= 0){
            attacks[i] = 0; 
            continue; 
        }
        attacks[i] = -1; 
    }

    attacks[king_idx] = 0; 
    attacks[check_idx] = 0; 
    return;
}

/*returns negative where attacks are */
int *heatmap_generator(board *board, int *occ_board, piece **check1,
                       piece **check2)
{
    int *ret = bit_board(0);
    piece *K = get_current_king(board);
    *check1 = NULL; 
    *check2 = NULL; 
    for (int i = 0; i < 32; i++)
    {

        int now = ret[K->board_idx];

        piece *current = board->pieces[i];
        if (current == NULL || current->taken)
        {
            continue;
        }
        if (current->color == board->turn)
        {
            continue;
        }
        attacked_squares(current, board, occ_board, 1, ret);

        int then = ret[K->board_idx];
        if ((then - now) == 0)
        {
            continue;
        }
        if (*check1 == NULL)
        {
            *check1 = current; 
            continue;
        }
        if (*check2 == NULL)
        {
            *check2 = current;
            continue; 
        }
    }

    for (int i = 0; i < 64; i++)
    {
        ret[i] = ret[i] * -1;
    }
    return ret;
}

void make_move_helper(list_t moves, int index_from, int index_to,
                      int *occ_board, board *board, char prom)
{

    move *ret = malloc(sizeof(move));
    ret->idx_from = index_from;
    ret->idx_to = index_to;
    ret->prom = prom;
    ret->id_piece = occ_board[index_from];
    ret->id_take = occ_board[index_to];
    if(prom == 'a'){
        int coeff = -1; 
        if(board->turn == 'B'){coeff =1; }
        int row0 = get_row(index_from); 
        int col0 = get_col(index_to);
        //if there is an an-pessant we are taking a piece on the same
        //row as we were move from on the same column we moved to 
        //thus that is the idx of the take

        ret->id_take = occ_board[idx(row0, col0)];  
    }

    list_add((void *)ret, moves);
    return;
}

/*So we are given req1 which is the squares a piece can only go 
to which is heat_map on the king or pinned pieces if less than 0 
cant go there. req2 is the attack heat so then we see if 
both accesses are positive if so we can go there so we add the move
note: this is not for pawn */
void move_maker_notpawn(list_t moves, int *req1, int *req2, int *occ_board,
                        board *board, int index_from)
{
    if (req1 == NULL)
    {
        int a[64];
        for (int i = 0; i < 64; i++)
        {
            a[i] = 1;
        }
        req1 = a;
    }

    for (int i = 0; i < 64; i++)
    {
        if (req1[i] < 0 || req2[i] < 0)
        {
            continue;
        }
        int id = occ_board[i];
        if (id >= 0 && (((board->pieces[id])->color) == board->turn))
        {
             continue;
        }
        make_move_helper(moves, index_from, i, occ_board, board, 0);
    }
    return;
}

/*
Given a board state find the possible moves of a pawn.
*/
void move_maker_pawn(list_t moves, int *req1, piece *pawn, int *occ_board,
                     board *board)
{
    int coeff = -1;
    if (pawn->color == 'B')
    {
        coeff = 1;
    }
    if (req1 == NULL)
    {
        int a[64];
        for (int i = 0; i < 64; i++)
        {
            a[i] = 1;
        }
        req1 = a;
    }
    /*Here we have to find what squares we can go to */

    /*the pawn can attack if the two diagonal squares are occupied or in the case
    of an-enpessant
    can move 1 forward if not blocked or 2 if starting
    can promote if reaches the back rank */
    char prom[4] = {'Q', 'N', 'B', 'R'};

    int row0 = get_row(pawn->board_idx);
    int col0 = get_col(pawn->board_idx);
    int starting_row = -1;
    int backrank = -1;

    if (pawn->color == 'B')
    {
        starting_row = 1;
        backrank = 7;
    }
    if (pawn->color == 'W')
    {
        starting_row = 6;
        backrank = 0;
    }

    int idx_of_moves[4];
    idx_of_moves[0] = idx(row0 + coeff, col0 - 1); //attacking to the left
    idx_of_moves[1] = idx(row0 + coeff, col0 + 1); //attacking to the right
    idx_of_moves[2] = idx(row0 + coeff, col0);     //moving up 1
    idx_of_moves[3] = -1;                          //moving up two
    if (starting_row == row0)
    {
        idx_of_moves[3] = idx(row0 + 2 * coeff, col0);
    }

    for (int i = 0; i < 4; i++)
    {
        if (idx_of_moves[i] < 0){
            continue;
        }
        int index = idx_of_moves[i]; //so is a square
        if (req1[index] < 0)
        {
            continue;
        }
        //we gotta case on if attack or move and check
        if (i == 0 || i == 1)
        {
            if (occ_board[index] < 0)
            {
                if (board->an_pessant == index)
                {
                    make_move_helper(moves, pawn->board_idx, index, occ_board, board, 'a');
                    continue;
                }
                continue;
            }
            piece *attacked = board->pieces[occ_board[index]];
            if (attacked->color != pawn->color)
            {
                //can attack check if we got to the back rank
                if (get_row(index) == backrank)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        make_move_helper(moves, pawn->board_idx, index, occ_board, board, prom[i]);
                    }
                    continue;
                }
                make_move_helper(moves, pawn->board_idx, index, occ_board, board, 0);
            }
            continue;
        }
        if (i == 2)
        {
            if (occ_board[index] < 0)
            { //nothing is in front
                if (get_row(index) == backrank)
                {
                    for (int i = 0; i < 4; i++)
                    { 
                        make_move_helper(moves, pawn->board_idx, index, occ_board, board, prom[i]);
                    }
                    continue;
                }
                make_move_helper(moves, pawn->board_idx, index, occ_board, board, 0);
            }
        }
        if (i == 3)
        {
            //if we can move up two times intrinsically
            //we have to check if we can move up 1 as well
            if ((occ_board[idx_of_moves[2]] < 0) && (occ_board[index] < 0))
            {
                make_move_helper(moves, pawn->board_idx, index, occ_board, board, 0);
            }
        }
    }

    return;
}


/* 
Frees the pin array
*/
void free_pins(int **pins)
{
    for (int i = 0; i < 32; i++)
    {
        if (pins[i] == NULL)
        {
            continue;
        }
        free(pins[i]);
    }
    free(pins); 
    return;
}

/* 
Generates a list type of the valid moves on a given 
board state
*/
list_t gen_moves(board *board)
{
    // Get the stuff set up
    list_t ret = list_make();
    int *occupancy = occ_board(board);
    piece *check1; 
    piece *check2;
    int *heat = heatmap_generator(board, occupancy, &check1, &check2);

    //just move around the king
    if (check2 != NULL)
    {
        piece *K = get_current_king(board);

        int *attacks = bit_board(-1);
        attacked_squares(K, board, occupancy, 1, attacks);
        move_maker_notpawn(
            ret, heat, attacks, occupancy, board, K->board_idx);

        free(attacks);
        free(occupancy);
        free(heat);
    }

  
    //create the pins
    int **pins = pin_squares(board, occupancy);
    /* if 1 check  
    1) move_king 
    2) take check piece
    3) block the piece  
    3i) dont move any piece that is pinned
    */
    if (check1 != NULL)
    {
        int *check_squares = bit_board(-1);
        attacked_squares(check1, board, occupancy, 0, check_squares);
        collapse_check_attacks(check_squares,check1, get_current_king(board)); 
        for (int i = 0; i < 32; i++)
        {
            piece *current = board->pieces[i];
            if (current->taken || (current->color != board->turn))
            {
                continue;
            }
            //check if pin here is so dont move
            if (pins[i] != NULL)
            {
                continue;
            }

            int *attacks = bit_board(-1);
            attacked_squares(current, board, occupancy, 1, attacks);
            switch (current->type)
            {
            case 'K':
                move_maker_notpawn(
                    ret, heat, attacks, occupancy, board, current->board_idx);
                break;
            case 'P':
                move_maker_pawn(ret, check_squares, current, occupancy, board);
                break;
            default:
                move_maker_notpawn(
                    ret, check_squares, attacks, occupancy, board, current->board_idx);
                break;
            }
            free(attacks);
        }

        //free all the alloced arrays and helper stuff
        free_pins(pins);
        free(check_squares);
        free(occupancy);
        free(heat);
        return ret;
    }
  

    //no checks
    //move everything no pins
    for (int i = 0; i < 32; i++)
    {
        piece *current = board->pieces[i];
        if (current->taken || current->color != board->turn)
        {
            continue;
        }

        int *allowed_squares = NULL;
        if (pins[i] != NULL)
        {
        
            allowed_squares = pins[i];
        }

        int *attacks = bit_board(-1);
        attacked_squares(current, board, occupancy, 1, attacks);

        switch (current->type)
        {
        case 'K':
            move_maker_notpawn(
                ret, heat, attacks, occupancy, board, current->board_idx);
            break;
        case 'P':
            move_maker_pawn(ret, allowed_squares, current, occupancy, board);
            break;
        default:
            move_maker_notpawn(
                ret, allowed_squares, attacks, occupancy, board, current->board_idx);
            break;
        }
        free(attacks);
        //case on if the pin is for the piec
    }

    free_pins(pins);
    free(occupancy);
    free(heat);
    return ret;
}

/*
frees a pointe to a move datatypes
*/
void free_move(void *move)
{
    if(move == NULL){return; }
    free(move);
    return;
}

/*
Given a board state applies a move to it.
*/
void make_move(board *board, move *move){
    if(board == NULL) { return; }
    if(move == NULL) {return; }

    if(board->turn == 'W') {board->turn = 'B'; }
    else{board->turn = 'W'; }
    
    piece *moved = board->pieces[move->id_piece];
    moved->board_idx = move->idx_to;
    if(move->prom == 'Q' || move->prom == 'R' 
       || move->prom == 'B' || move->prom == 'N'){
        moved->type = move->prom; 
    }

    board->an_pessant = -1; 
    piece *P = board->pieces[move->id_piece]; 
    if(P->type == 'P'){
        //go to find idx of where the taker would go for an-pessant
        int diff = get_row(move->idx_to) - get_row(move->idx_from); 
        if(diff == 2 || diff == -2){
            board->an_pessant = idx( (diff/2) + get_row(move->idx_from),
           get_col(P->board_idx));
        }
    }

    if(move->id_take >= 0){
        piece *taken = board->pieces[move->id_take]; 
        taken->taken = true; 
    }
    return; 
}

/* 
Takes a given board and the move that was applied 
to it and returns the board as it was before the 
move was made
*/
void unmake_move(board *board, move *move){
    if(board == NULL) { return; }
    if(move == NULL) {return; }

    if(board->turn == 'W') {board->turn = 'B'; }
    else{board->turn = 'W'; }

    if(move->prom == 'a'){
        board->an_pessant = move->idx_to; 
    }

    piece *moved = board->pieces[move->id_piece];
    moved->board_idx = move->idx_from;
    if(move->prom == 'Q' || move->prom == 'R' 
       || move->prom == 'B' || move->prom == 'N'){
        moved->type = 'P'; 
    }

    if(move->id_take >= 0){
        piece *taken = board->pieces[move->id_take]; 
        taken->taken = false;  
    }
    return; 
}

/* 
This function is the equality function for two
moves. As a move is the same as long as we are 
moving to the same postitions and the special
character 'prom' is the same then the moves are the 
same.
*/
bool equal_move(move *move1, move *move2){
    if(move1 == NULL){return false; }
    if(move2 == NULL){return false; }

    if(move1->idx_from != move2->idx_from) {return false;}
    if(move1->idx_to != move2->idx_to) {return false; }
    if(move1->prom != move2->prom) {return false; }

    return true; 
}


/* 
This takes in a given move and hashes it. 
*/ 
int hash_move(move *move){
    if(move == NULL) { return 0; }
    int ret = 0; 
    ret += ((move->idx_from * 3010349) + 49); 
    ret += ((move->idx_to * 54018521) + 73); 
    ret += ((move->prom * 3571) + 11); 

    while (ret < 0)
    {
       ret *= 769; 
    }
    return ret; 
}


/* 
This function takes in a string buffer as the param 
and modifies it to be the string representation of a given move
*/
void move_string_rep(char *buffer, move *move_input){
    if(move_input == NULL) { return; }
    if(buffer == NULL) {return; }

    buffer[8] = '\0'; 
    buffer[2] = '-'; 
    buffer[3] = '>'; 
    buffer[6] = ','; 

    buffer[0] = col_to_char(get_col(move_input->idx_from)); 
    char row_from = (8 - get_row(move_input->idx_from)); 
    buffer[1] = row_from + '0'; 
    buffer[4] = col_to_char(get_col(move_input->idx_to)); 
    char row_to = (8 - get_row(move_input->idx_to));
    buffer[5] = row_to + '0';
    buffer[7] = move_input->prom;  
    if(buffer[7] == buffer[8]){ buffer[6] = '\0'; }
    return; 
}

/*
This function takes a string represnting the move 
and if that move is allowed for the board lets the 
user make it.

The function returns a 0 if the move could not 
be executed as it was not in the list. The function 
returns a 1 if it could apply the move to the board. 
*/
int apply_move_user(board *board, char *move_string){
    if(board == NULL) {return 0; }
    if(move_string == NULL) {return 0; }
    list_t moves = gen_moves(board); 

    while(!list_empty(moves)){ 
        move *current = list_pop(moves); 

        char rep[8]; 
        move_string_rep(rep, current); 
        //printf("%s\n",rep); 
        
        if(strcmp(rep,move_string) == 0){
            make_move(board, current); 
            free_move(current);
            list_free(moves, &free_move); 
            return 1; 
        }
        free_move(current); 
    }
    list_free(moves, &free_move); 
    return 0; 
}



/* 
Iplementation of the minimax algo for the board



*/
int dfs_helper(board *board, list_t moves, int depth){
    if(list_empty(moves)){
        //if at a terminal state of the board
        if(board->turn == 'W'){ return -10000; }
        return 10000;
    }

    if(depth <= 0){
        return sum_of_heurisitcs(board); 
    }

    int max_min = 10000; 
    bool max_q = false; 
    if(board->turn == 'B'){max_q = true; max_min = -10000; }
    
    //max part of the algo
   

    while(!list_empty(moves)){
        move *current_move = list_pop(moves); 
        make_move(board, current_move); 

        list_t sub_moves = gen_moves(board); 
        int result = dfs_helper(board, sub_moves, depth-1);    

         //minimazation part of the algorithim 
        if(!max_q && result < max_min){
            max_min = result; 
        }
        //maximazation part of the algorithim
        if(max_q && result > max_min){
            max_min = result; 
        }

        unmake_move(board, current_move); 
        free(current_move); 
        list_free(sub_moves, &free_move); 
    }

    //main part of the algo as black 
    return max_min; 
}



/*
requires that the current board not be in checkmate

Move will return the best move in a given 
board state found via the minimax algo to a 
given depth

This is the driver function while the helper recurses
to find the best
*//*dfs_helper(current_board, sub_moves, 0); */
move *computer_find_move(board *current_board, int depth){
    if(current_board == NULL) { return NULL; }
    list_t current_moves = gen_moves(current_board); 

    move *max_move = NULL; 
    int minimax = -10000; bool max_q = true; 
    if(current_board->turn == 'B'){minimax = 10000; max_q = false; }

    while(!list_empty(current_moves)){ 
        move *current_move = list_pop(current_moves); 
        make_move(current_board,current_move); 

        list_t submoves = gen_moves(current_board);
        int result = dfs_helper(current_board,submoves,depth);
        list_free(submoves, &free_move);

        unmake_move(current_board, current_move); 
        //trying to max move
        if(max_q && result > minimax){
            move *temp = max_move; 
            max_move = current_move;
            current_move = temp; 
        }
        //trying to min move
        if(!max_q && result < minimax){
            move *temp = max_move; 
            max_move = current_move;
            current_move = temp; 
        }
        free_move(current_move); 
    }
    return max_move; 
}


/* 
I think this is fine although maybe want a hashmap of moves
although as human is inputting the noves the speed doesnt really matter 

for the search of the program might add a few heuristics that make pieces better
i.e putting bishops on long diagonals, pawns in center or devolping pices

could even help by adding a lib of moves to respond to openings but that 
part is abstracted away from this interface
*/


#endif