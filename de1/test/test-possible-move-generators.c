#include "include/test-possible-move-generators.h"
#include <stdio.h> 

void test_possible_moves()
{
    test_pawn_generator();
    test_rook_generator();
    test_knight_generator();
    test_bishop_generator();
    test_queen_generator();
    test_king_generator();
    test_castling_generator();
    test_all_moves();
}

void test_all_moves()
{
    
}