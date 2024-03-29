#include "include/possible-move-generators.h"
#include "include/ai-move-generator.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "include/game.h"
#include "include/HW.h"

/* Generate moves for the given board, generator, and piece located at (x, y) */
move_list_t *generate_moves(board_t *board, int generator_offset, int x, int y)
{
    volatile int *generator_ptr = (unsigned int *) (lw_virtual + generator_offset);

    // put current board into sdram
    for (int i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = (*board)[i / 8][i % 8];
    }

    // run HW module
    *(generator_ptr + 1) = 0; // src
    *(generator_ptr + 2) = 64 * 4; // dest
    *(generator_ptr + 3) = x; // x
    *(generator_ptr + 4) = y; // y
    *generator_ptr = 0; // start the module
    int count = *generator_ptr;

    move_list_t *moves = create_move_list(count);

    // Copy moves into a move list
    for (int i = 0; i < count; i++) {
        board_t *move = malloc(sizeof(board_t));
        int move_start = (i + 1) * 64; // move starts after src board & prev moves in SDRAM

        for (int j = 0; j < 64; j++) {
            (*move)[j / 8][j % 8] = *((volatile int*)sdram_virtual + move_start + j);
        }

        moves->moves[i] = move;
        move = NULL;
    }
    moves->num_moves = count;

    return moves;
}

/*
 * Upgrades the pawn on the board to a rook, knight, bishop and queen.
 * If a specific upgrade wasn't selected, defaults to queen.
 * Alters move_list directly.
 */
static move_list_t* upgrade_pawns(move_list_t *move_list, char pawn)
{
    move_list_t *upgraded_move_list = create_move_list(12); // max 12 moves for a pawn

    char colour = get_colour(pawn);

    for (int i = 0; i < move_list->num_moves; i++) {
        int dest_x = -1, dest_y = -1;
        if (!find_piece(move_list->moves[i], pawn, &dest_x, &dest_y)) return NULL;

        if ((colour == WHITE && (dest_y == 7)) || (colour == BLACK && (dest_y == 0))) {
            char rook = colour == WHITE ? pawn + 10 : pawn - 10;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = rook;

            char knight = colour == WHITE ? pawn + 20 : pawn - 20;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = knight;

            char bishop = colour == WHITE ? pawn + 30 : pawn -30;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = bishop;

            char queen = colour == WHITE ? pawn + 39 : pawn - 39;
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
            (*upgraded_move_list->moves[upgraded_move_list->num_moves - 1])[dest_y][dest_x] = queen;
        } else {
            add_move_to_list(upgraded_move_list, move_list->moves[i], pawn, dest_x, dest_y);
        }
    }

    destroy_move_list(move_list);
    return upgraded_move_list;
}

move_list_t *generate_pawn_moves(board_t *board, char pawn)
{
    if (board == NULL || !is_pawn(pawn)) return NULL;

    int src_x = -1, src_y = -1;
    if (!find_piece(board, pawn, &src_x, &src_y)) return NULL;

    move_list_t *moves = generate_moves(board, pawn_offset, src_x, src_y);

    return upgrade_pawns(moves, pawn);
}

move_list_t *generate_rook_moves(board_t *board, char rook)
{
    if (board == NULL || !is_rook(rook)) return NULL;
    
    int rook_x = -1, rook_y = -1;
    if (!find_piece(board, rook, &rook_x, &rook_y)) return NULL;

    return generate_moves(board, rook_offset, rook_x, rook_y);
}

move_list_t *generate_knight_moves(board_t *board, char knight)
{
    if (board == NULL || !is_knight(knight)) return NULL;

    int src_x = -1, src_y = -1;
    if (!find_piece(board, knight, &src_x, &src_y)) return NULL;

    return generate_moves(board, knight_offset, src_x, src_y);
}

move_list_t *generate_bishop_moves(board_t *board, char bishop) 
{
    if (board == NULL || !is_bishop(bishop)) return NULL;
    
    int bishop_x = -1, bishop_y = -1;
    if (!find_piece(board, bishop, &bishop_x, &bishop_y)) return NULL;

    return generate_moves(board, bishop_offset, bishop_x, bishop_y);
}

move_list_t *generate_queen_moves(board_t *board, char queen) 
{
    if (board == NULL || !is_queen(queen)) return NULL;
    
    int queen_x = -1, queen_y = -1;
    if (!find_piece(board, queen, &queen_x, &queen_y)) return NULL;

    return generate_moves(board, queen_offset, queen_x, queen_y);
}

move_list_t *generate_king_moves(board_t *board, char king)
{
    if (board == NULL || !is_king(king)) return NULL;

    int src_x = -1, src_y = -1;
    if (!find_piece(board, king, &src_x, &src_y)) return NULL;

    return generate_moves(board, king_offset, src_x, src_y);
}

/* Open /dev/mem to give access to physical addresses */
int open_physical (int fd)
{
    if (fd == -1) // check if already open
        if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1) {
            printf ("ERROR: could not open \"/dev/mem\"...\n");
            return (-1);
        }

    return fd;
}

/* Close /dev/mem to give access to physical addresses */
void close_physical (int fd)
{
    close (fd);
}

/* Establish a virtual address mapping for the physical addresses starting
* at base and extending by span bytes */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
    void *virtual_base;

    // Get a mapping from physical addresses to virtual addresses
    virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);

    if (virtual_base == MAP_FAILED) {
        printf ("ERROR: mmap() failed...\n");
        close (fd);
        return (NULL);
    }

    return virtual_base;
}

/* Close the previously-opened virtual address mapping */
int unmap_physical(void * virtual_base, unsigned int span)
{
    if (munmap (virtual_base, span) != 0) {
        printf ("ERROR: munmap() failed...\n");
        return (-1);
    }

    return 0;
}
