/* 
 * ###############################################################################
 * #                     Virtual Exception's ASCII-ENGINE                        #
 * # Declaration of the structs used, followed by external variables & functions #
 * ###############################################################################
 *
 * Important definitions: COLS, ROWS, buffer interval
*/

#ifndef ASCII_ENGINE_H
#define ASCII_ENGINE_H

#ifdef WIN32
#include <windows.h>
#include <wincon.h>
#include <synchapi.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

#define UID(x, y) ((x) + (y) * COLS)
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

extern unsigned int COLS;
extern unsigned int ROWS;

#ifdef WIN32
extern CHAR_INFO* scr_buffer;
extern SMALL_RECT rectWindow;
#else
struct Char {
	char c;
};
extern struct termios saved;
extern struct Char* scr_buffer;
#endif

extern char* light_scale;

extern void ascii_init();
extern void ascii_die(int s);
extern void ascii_ctrl_die(int s);
extern void ascii_drawPixel(float x, float y, uint8_t g_col);
extern void ascii_drawLine(float x0, float y0, float x1, float y1, uint8_t g_col);
extern void ascii_clear();
extern void ascii_render(unsigned long wait);

#endif