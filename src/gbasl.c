#include <stdio.h>

#include "tonc.h"

int main() {
    REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

    // Init BG 0 for text on screen entries.
    tte_init_se_default(0, BG_CBB(0)|BG_SBB(31));

    // Enable TTE's console functionality
    tte_init_con();

    tte_printf("#{P:72,64}");        // Goto (72, 64).
    tte_printf("Hello World!");      // Print "Hello world!"

    while(1);

    return 0;
}
