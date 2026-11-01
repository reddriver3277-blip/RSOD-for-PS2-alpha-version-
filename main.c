/*
 * PS2 RSOD - истински BIOS exception
 */

#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>

int main(int argc, char *argv[])
{
    SifInitRpc(0);

    /* Break instruction - MIPS exception
     * PS2 BIOS хваща това и показва истински RSOD */
    __asm__ volatile("break 0");

    while(1) { }

    return 0;
}
