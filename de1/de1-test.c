#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "include/game.h"

#define pawn_offset 0x2040
#define rook_offset 0x2080
#define knight_offset 0x20C0 
#define king_offset 0x2100
#define queen_offset 0x2140 
#define bishop_offset 0x2180
#define generator_span 0x3F

#define sdram_offset 0xC0000000
#define sdram_span 0x03FFFFFF

#define lw_bridge_offset 0xFF200000
#define lw_bridge_span 0x00005000

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

int main()
{
    board_t pawn_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t rook_board = {
            {EMPTY, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {EMPTY, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    WROOK0,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {WPAWN0, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };
    
    board_t bishop_board = {
            {WROOK0, WKNIGHT0, EMPTY,    WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, EMPTY,  WBISHOP0, EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t knight_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, WQUEEN0, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, WPAWN4, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t queen_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY, WKING,  WBISHOP1, WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   WPAWN3, EMPTY, WPAWN5,   WPAWN6,   WPAWN7},       
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  WPAWN4,  EMPTY,    WQUEEN0,    EMPTY},
            {EMPTY, EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    EMPTY,  EMPTY,  EMPTY,    EMPTY,    EMPTY},
            {BPAWN0, BPAWN1,   BPAWN2,   BPAWN3, BPAWN4, BPAWN5,   BPAWN6,   BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, BQUEEN0, BKING,  BBISHOP1, BKNIGHT1, BROOK1},
        };

    board_t king_board = {
            {WROOK0, WKNIGHT0, WBISHOP0, EMPTY,  EMPTY,  EMPTY,    WKNIGHT1, WROOK1},
            {WPAWN0, WPAWN1,   WPAWN2,   EMPTY,  WKING,  EMPTY,    EMPTY,    WPAWN7},       
            {EMPTY,  EMPTY,    WQUEEN0,   EMPTY,  EMPTY,  EMPTY,    WPAWN6,   WBISHOP1},
            {EMPTY,  EMPTY,    EMPTY,    WPAWN3, WPAWN4, WPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  EMPTY,    EMPTY,    BPAWN3, BPAWN4, BPAWN5,   EMPTY,    EMPTY},
            {EMPTY,  BQUEEN0,   EMPTY,    EMPTY,  EMPTY,  EMPTY,    BPAWN6,   BBISHOP1},
            {BPAWN0, BPAWN1,   BPAWN2,   EMPTY,  BKING,  EMPTY,    EMPTY,    BPAWN7},
            {BROOK0, BKNIGHT0, BBISHOP0, EMPTY,  EMPTY,  EMPTY,    BKNIGHT1, BROOK1}
        };

    // get virtual addrs
    volatile int *pawn_ptr, *knight_ptr, *rook_ptr, *bishop_ptr, *queen_ptr, *king_ptr;
    int lw_fd = -1, sdram_fd = -1, count;
    void *lw_virtual, *sdram_virtual;

    // Create virtual memory access to the FPGA light-weight bridge
    if ((lw_fd = open_physical (lw_fd)) == -1) return (-1);
    if ((lw_virtual = map_physical (lw_fd, lw_bridge_offset, lw_bridge_span)) == NULL) return (-1);

    // Create virtual memory access to the SDRAM
    if ((sdram_fd = open_physical (sdram_fd)) == -1) return (-1);
    if ((sdram_virtual = map_physical (sdram_fd, sdram_offset, sdram_span)) == NULL) return (-1);

    pawn_ptr = (unsigned int *) (lw_virtual + pawn_offset);
    knight_ptr = (unsigned int *) (lw_virtual + knight_offset);
    rook_ptr = (unsigned int *) (lw_virtual + rook_offset);
    bishop_ptr = (unsigned int *) (lw_virtual + bishop_offset);
    queen_ptr = (unsigned int *) (lw_virtual + queen_offset);
    king_ptr = (unsigned int *) (lw_virtual + king_offset);

    int i;
    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = pawn_board[i / 8][i % 8];
    }
    *(pawn_ptr + 1) = 0; // src
    *(pawn_ptr + 2) = 64*4; // dest
    *(pawn_ptr + 3) = 2; // x = 2 for pawn
    *(pawn_ptr + 4) = 1; // y = 1 for pawn
    *pawn_ptr = 0; // start the module
    count = *pawn_ptr; // expect 2
    printf("pawn board count: %d (should be 2)\n", count);

    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = rook_board[i / 8][i % 8];
    }
    *(rook_ptr + 1) = 0; // src
    *(rook_ptr + 2) = 64*4; // dest
    *(rook_ptr + 3) = 3; // x = 3 for rook
    *(rook_ptr + 4) = 3; // y = 3 for rook
    *rook_ptr = 0; // start the module
    count = *rook_ptr; // expect 11
    printf("rook board count: %d (should be 11)\n", count);

    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = bishop_board[i / 8][i % 8];
    }
    *(bishop_ptr + 1) = 0; // src
    *(bishop_ptr + 2) = 64*4; // dest
    *(bishop_ptr + 3) = 5; // x = 5 for bishop
    *(bishop_ptr + 4) = 3; // y = 3 for bishop
    *bishop_ptr = 0; // start the module
    count = *bishop_ptr; // expect 9
    printf("bishop board count: %d (should be 9)\n", count);

    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = knight_board[i / 8][i % 8];
    }
    *(knight_ptr + 1) = 0; // src
    *(knight_ptr + 2) = 64*4; // dest
    *(knight_ptr + 3) = 1; // x = 1 for knight
    *(knight_ptr + 4) = 0; // y = 0 for knight
    *knight_ptr = 0; // start the module
    count = *knight_ptr; // expect 2
    printf("knight board count: %d (should be 2)\n", count);

    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = queen_board[i / 8][i % 8];
    }
    *(queen_ptr + 1) = 0; // src
    *(queen_ptr + 2) = 64*4; // dest
    *(queen_ptr + 3) = 6; // x = 6 for queen
    *(queen_ptr + 4) = 3; // y = 3 for queen
    *queen_ptr = 0; // start the module
    count = *queen_ptr; // expect 14
    printf("queen board count: %d (should be 14)\n", count);

    for (i = 0; i < 64; i++) {
        *((volatile int*)sdram_virtual + i) = king_board[i / 8][i % 8];
    }
    *(king_ptr + 1) = 0; // src
    *(king_ptr + 2) = 64*4; // dest
    *(king_ptr + 3) = 4; // x = 4 for king
    *(king_ptr + 4) = 1; // y = 1 for king
    *king_ptr = 0; // start the module
    count = *king_ptr; // expect 8
    printf("king board count: %d (should be 8)\n", count);

    unmap_physical (lw_virtual, lw_bridge_span);
    close_physical (lw_fd);
    unmap_physical (sdram_virtual, sdram_span);
    close_physical (sdram_fd);

    return(0);
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
