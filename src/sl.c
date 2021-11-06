#include <stdlib.h>
#include <string.h>

#include "tonc.h"

#include "train.h"
#include "screen.h"

#define ERR 1
#define OK 0

static char *LOCO[LOGOANIMSTEPS][LOGOHEIGHT + 1] =
    {{LOGO1, LOGO2, LOGO3, LOGO4, LWHL11, LWHL12, DELLN},
    {LOGO1, LOGO2, LOGO3, LOGO4, LWHL21, LWHL22, DELLN},
    {LOGO1, LOGO2, LOGO3, LOGO4, LWHL31, LWHL32, DELLN},
    {LOGO1, LOGO2, LOGO3, LOGO4, LWHL41, LWHL42, DELLN},
    {LOGO1, LOGO2, LOGO3, LOGO4, LWHL51, LWHL52, DELLN},
    {LOGO1, LOGO2, LOGO3, LOGO4, LWHL61, LWHL62, DELLN}};

// static char *COAL[LOGOHEIGHT + 1] = {LCOAL1, LCOAL2, LCOAL3, LCOAL4, LCOAL5, LCOAL6, DELLN};

// static char *CAR[LOGOHEIGHT + 1] = {LCAR1, LCAR2, LCAR3, LCAR4, LCAR5, LCAR6, DELLN};

char debug[32];

void drawWhite(int x, int y, char* str) {
    se_puts(x*8, y*8, str, 0x4000);
}

// int mvaddch(int y, int x, const chtype ch);
int my_mvaddstr(int y, int x, char* ch) {

    siprintf(debug, "x=%3d", x); se_puts(160, 0, debug, 0x3000);
    siprintf(debug, "y=%3d", y); se_puts(160, 10, debug, 0x3000);

    char* buf = malloc(COLS);

    if (x < 0) {
        int nums2cpy = LOGOLENGTH + x;
        siprintf(debug, "n2c=%3d", nums2cpy); se_puts(160, 30, debug, 0x3000);
        memcpy(buf, ch-x, nums2cpy);
        buf[nums2cpy] = '\0';
        drawWhite(0, y, buf);
    } else {
        int nums2cpy = min(LOGOLENGTH, COLS-x);
        siprintf(debug, "n2c=%3d", nums2cpy); se_puts(160, 30, debug, 0x3000);
        memcpy(buf, ch, nums2cpy);
        buf[nums2cpy] = '\0';
        drawWhite(x, y, buf);
    }

    free(buf);

    return 0;
}

// mainly copied logic from original sl 5.03

#define SMOKEPTNS 16
void add_smoke(int y, int x) {
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0, 0, 0};
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1,
                                 2,  2, 2, 2, 2, 3, 3, 3};

    if (x % 4 == 0) {
        for (int i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}

int add_sl(int x) {
    siprintf(debug, "x=%3d", x);
    se_puts(10, 10, debug, 0x3000);

    int i, y;

    if (x < -LOGOLENGTH)
        return ERR;
    y = LINES / 2; // uppermost line is in the middle

    for (i = 0; i <= LOGOHEIGHT; ++i) {
        my_mvaddstr(y + i, x, LOCO[(LOGOLENGTH + x) / 2 % LOGOANIMSTEPS][i]);
        // my_mvaddstr(y + i, x + 21, COAL[i]);
        // my_mvaddstr(y + i, x + 42, CAR[i]);
        // my_mvaddstr(y + i, x + 63, CAR[i]);
    }
    // TODO: Later...
    //add_smoke(y - 1, x + LOGOFUNNEL);
    return OK;
}
