#include <tonc.h>

#include "screen.h"
#include "sl.h"

// === CONSTANTS & STRUCTS ============================================


// === FUNCTIONS ======================================================

void init() {
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	txt_init_std();

	// Basic se text initialization
	txt_init_se(0, BG_CBB(0) | BG_SBB(31), 0x1000, CLR_RED, 0x0E);

	// set some extra colors
	pal_bg_mem[0x1F] = CLR_RED;
	pal_bg_mem[0x2F] = CLR_GREEN;
	pal_bg_mem[0x3F] = CLR_BLUE;
	pal_bg_mem[0x4F] = CLR_WHITE;

	pal_bg_mem[0xE8] = pal_bg_mem[0x08]; // bg
	pal_bg_mem[0xEE] = CLR_ORANGE; // shadow
	pal_bg_mem[0xEF] = pal_bg_mem[0x0F]; // text
}

int main() {
	init();

	se_puts(8*(COLS/2-2), 16, "gbasl",   0x1000);
	for (int x=COLS; x>-1; x--) {
		if (x<1)
			x=COLS;
		// se_puts(8, 16, "bank 1:\n  red",   0x1000);
		// se_puts(8, 40, "bank 2:\n  green", 0x2000);
		// se_puts(8, 72, "bank 3:\n  blue",  0x3000);
		// se_puts(8, 100, "white",  0x4000);
		add_sl(x);
		VBlankIntrWait();
	}
	return 0;
}
