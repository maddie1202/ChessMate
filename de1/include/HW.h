#define pawn_offset 0x2040
#define rook_offset 0x2080
#define knight_offset 0x20C0 
#define king_offset 0x2100
#define queen_offset 0x2140 
#define bishop_offset 0x2180

#define sdram_offset 0xC0000000
#define sdram_span 0x03FFFFFF

#define lw_bridge_offset 0xFF200000
#define lw_bridge_span 0x00005000

#define push_buttons_offset 0x0010
#define pb_edgecapture_offset 0x001C
#define leds_offset 0x0020

extern int lw_fd, sdram_fd;
extern void *lw_virtual, *sdram_virtual;

/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

int setup_hardware();
void teardown_hardware();
