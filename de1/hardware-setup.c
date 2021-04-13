#include "include/HW.h"
#include <stdlib.h>
#include <stdbool.h>

bool setup_hardware()
{
    // Create virtual memory access to the FPGA light-weight bridge
    lw_fd = -1;
    sdram_fd = -1;
    if ((lw_fd = open_physical (lw_fd)) == -1) return false;
    if ((lw_virtual = map_physical (lw_fd, lw_bridge_offset, lw_bridge_span)) == NULL) return false;

    // Create virtual memory access to the SDRAM
    if ((sdram_fd = open_physical (sdram_fd)) == -1) return false;
    if ((sdram_virtual = map_physical (sdram_fd, sdram_offset, sdram_span)) == NULL) return false;

    if (lw_virtual == NULL || sdram_virtual == NULL) return false;

    return true;
}

void teardown_hardware()
{
    unmap_physical (lw_virtual, lw_bridge_span);
    close_physical (lw_fd);
    unmap_physical (sdram_virtual, sdram_span);
    close_physical (sdram_fd);
}