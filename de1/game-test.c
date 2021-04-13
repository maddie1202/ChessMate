#include "include/game.h"
#include "include/possible-move-generators.h"
#include <stdlib.h>
#include <stdio.h>

char get_colour(char piece)
{
    return piece == 0 ? EMPTY : (piece > 0 ? WHITE : BLACK);
}

void update_game_state(game_t *current_game, board_t *new_board, char moved_last)
{
    // max of 2 pieces can be moved by a player in one turn
    char pieces_moved[2] = {0, 0};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // detect difference between boards: we will be recording the moved pieces on new_board so there is no doubling up
            // also, only consider pieces from the player that made the last move
            if ((*current_game->board)[i][j] != (*new_board)[i][j] && get_colour((*new_board)[i][j]) == moved_last) {
                // haven't detected any other changed pieces yet
                if (pieces_moved[0] == 0) {
                    pieces_moved[0] = (*new_board)[i][j];
                }
                // have now found the max number of possible pieces moved by the user
                else {
                    pieces_moved[1] = (*new_board)[i][j];
                    break;
                }

            }
        }
    }

    if (pieces_moved[0] == WROOK0) current_game->wrook0_has_moved = true;
    else if (pieces_moved[0] == WROOK1) current_game->wrook1_has_moved = true;
    else if (pieces_moved[0] == WKING) current_game->wking_has_moved = true;
    else if (pieces_moved[0] == BROOK0) current_game->brook0_has_moved = true;
    else if (pieces_moved[0] == BROOK1) current_game->brook1_has_moved = true;
    else if (pieces_moved[0] == BKING) current_game->bking_has_moved = true;

    if (pieces_moved[1] == WROOK0) current_game->wrook0_has_moved = true;
    else if (pieces_moved[1] == WROOK1) current_game->wrook1_has_moved = true;
    else if (pieces_moved[1] == WKING) current_game->wking_has_moved = true;
    else if (pieces_moved[1] == BROOK0) current_game->brook0_has_moved = true;
    else if (pieces_moved[1] == BROOK1) current_game->brook1_has_moved = true;
    else if (pieces_moved[1] == BKING) current_game->bking_has_moved = true;
}

void test0(void)
{
    // construct a game where nothing has moved yet
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // do a queenside castling for white
    board_t w_expected1 = {
            {EMPTY,    EMPTY,  WKING,  WROOK0,   EMPTY,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    // check that WROOK0 and WKING have now moved
    update_game_state(&game, &w_expected1, WHITE);
    if (game.wrook0_has_moved && game.wking_has_moved) printf("Test0: Success!\n");
    else printf("game.wrook0_has_moved: %d, game.wrook0_has_moved: %d, game.wking_has_moved: %d, game.brook0_has_moved: %d, game.brook0_has_moved: %d, game.bking_has_moved: %d\n",
                game.wrook0_has_moved, game.wrook1_has_moved, game.wking_has_moved, 
                game.brook0_has_moved, game.brook1_has_moved, game.bking_has_moved);
}

void test1(void)
{
    // construct a game where nothing has moved yet
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    // do queenside castling for black
    board_t b_expected1 = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {EMPTY,    EMPTY,  BKING,  BROOK0,   EMPTY,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    // check that BROOK0 and BKING have now moved
    update_game_state(&game, &b_expected1, BLACK);
    if (game.brook0_has_moved && game.bking_has_moved) printf("Test1: Success!\n");
    else printf("game.wrook0_has_moved: %d, game.wrook0_has_moved: %d, game.wking_has_moved: %d, game.brook0_has_moved: %d, game.brook0_has_moved: %d, game.bking_has_moved: %d\n",
                game.wrook0_has_moved, game.wrook1_has_moved, game.wking_has_moved, 
                game.brook0_has_moved, game.brook1_has_moved, game.bking_has_moved);
}

void test2(void)
{
    board_t curr = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   WPAWN1, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    game_t game;
    game.board = &curr;
    game.bking_has_moved = false;
    game.brook0_has_moved = false;
    game.brook1_has_moved = false;
    game.wking_has_moved = false;
    game.wrook0_has_moved = false;
    game.wrook1_has_moved = false;

    board_t exp = {
            {WROOK0,   EMPTY,  EMPTY,  EMPTY,    WKING,   WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0,   EMPTY, WPAWN2, WBISHOP0, WQUEEN0, WPAWN5,   WPAWN6,   WPAWN7},       
            {WKNIGHT0, EMPTY,  EMPTY,  WPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    WPAWN1,  EMPTY,  EMPTY,    WPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN4,  EMPTY,    EMPTY,    EMPTY},
            {BKNIGHT0, EMPTY,  EMPTY,  BPAWN3,   EMPTY,   EMPTY,    EMPTY,    EMPTY},
            {BPAWN0,   BPAWN1, BPAWN2, BBISHOP0, BQUEEN0, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0,   EMPTY,  EMPTY,  EMPTY,    BKING,   BBISHOP1, BKNIGHT1, BROOK1}
        };

    // check that nothing of consequence changed
    update_game_state(&game, &exp, WHITE);
    if (!game.wrook0_has_moved && !game.wrook1_has_moved && !game.wking_has_moved && !game.brook0_has_moved && !game.brook1_has_moved && !game.bking_has_moved) printf("Test2: Success!\n");
    else printf("game.wrook0_has_moved: %d, game.wrook0_has_moved: %d, game.wking_has_moved: %d, game.brook0_has_moved: %d, game.brook0_has_moved: %d, game.bking_has_moved: %d\n",
                game.wrook0_has_moved, game.wrook1_has_moved, game.wking_has_moved, 
                game.brook0_has_moved, game.brook1_has_moved, game.bking_has_moved);
}

int main(void)
{
    test0();
    test1();
    test2();

    return 0;
}