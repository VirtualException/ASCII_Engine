/* 
 * #####################################################################################
 * #                        Virtual Exception's ASCII-ENGINE                           #
 * # Implementation of all the functions, with the declaration of non-extern variables #
 * #####################################################################################
*/

#include "ascii_engine.h"

unsigned int COLS;
unsigned int ROWS;

#ifdef WIN32
CHAR_INFO* scr_buffer;
SMALL_RECT rectWindow;
#else
struct termios saved;
struct Char* scr_buffer;
#endif

char* light_scale = " .:-=+*%#@";

void ascii_init();
void ascii_die(int s);
void ascii_ctrl_die(int s);
void ascii_drawPixel(float x, float y, uint8_t g_col);
void ascii_clear();
void ascii_render(unsigned long wait);

void
ascii_init() {
#ifdef WIN32
    // Terminal size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COLS = csbi.srWindow.Right - csbi.srWindow.Left;
    ROWS = csbi.srWindow.Bottom - csbi.srWindow.Top;

    // Ctrl-c handler
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)ascii_ctrl_die, TRUE);

    // Hide cursor
    CONSOLE_CURSOR_INFO cur_info;
    cur_info.dwSize = 100;
    cur_info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

    scr_buffer = malloc(COLS * ROWS * sizeof(CHAR_INFO));
    rectWindow = (SMALL_RECT) { 0, 0, COLS, ROWS };
#else
    // Terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    COLS = w.ws_col;
    ROWS = w.ws_row;

    // Ctrl-c handler
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ascii_ctrl_die;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    // Hide cursor
    printf("\e[?25l");

    // Dont print "^C"
    struct termios attributes;
    tcgetattr(STDIN_FILENO, &saved);
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &attributes);

    scr_buffer = malloc((COLS * ROWS) * sizeof(struct Char));

#endif
    ascii_clear();
    
}

void
ascii_die(int s) {

    ascii_clear();
    ascii_render(0);
    ascii_clear();

#ifdef WIN32
    // Re-enable cursor
    CONSOLE_CURSOR_INFO cur_info;
    cur_info.dwSize = 5;
    cur_info.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);
#else
    // Restore atributtes
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);
    // Re-enable cursor
    printf("\e[?25h");
#endif

    // Clear heap
    free(scr_buffer);

    exit(s);
}

void ascii_ctrl_die(int s) {
    ascii_die(0);
}

void
ascii_drawPixel(float x, float y, uint8_t g_col) {

    int i;
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        i = UID((int)x,(int)y);
        if (i < COLS * ROWS && i >= 0) {
#ifdef WIN32
            scr_buffer[i].Char.AsciiChar = light_scale[(int) (g_col / 28.333333339f)];
            scr_buffer[i].Attributes = 0x000F;
#else
            scr_buffer[i].c = light_scale[(int)(g_col / 28.333333339f)];
#endif
        }
    }
}

void
ascii_drawLine(float x0, float y0, float x1, float y1, uint8_t g_col) {
    
    float x_1 = x0, x_2 = x1, y_1 = y0, y_2 = y1;

    // If the vector is opposite... ¿?
    if (x0 > x1) {
        x_1 = x1;
        x_2 = x0;
        
        y_1 = y1;
        y_2 = y0;
    }

    float r = (float) (y_2 - y_1) / (x_2 - x_1);

    float x = x_1;

    while (x <= x_2) {
        float y = r * (x - x_1) + y_1;
        if (x >= 0 && x < COLS && y >= 0 && y < ROWS) ascii_drawPixel(x, y, g_col);

        x += 0.2f;

        
    }

}

void
ascii_clear() {

    for (int i = 0; i < COLS * ROWS; i++) {
#ifdef WIN32
        scr_buffer[i].Attributes = 0;
        scr_buffer[i].Char.AsciiChar = ' ';
#else
        scr_buffer[i].c = ' ';
#endif
    }

}

void
ascii_render(unsigned long wait) {

#ifdef WIN32

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){ 0, 0 });

    WriteConsoleOutputA(GetStdHandle(STD_OUTPUT_HANDLE), scr_buffer,
                        (COORD){(short)COLS, (short)ROWS }, (COORD){ 0, 0 }, &rectWindow);
    Sleep(wait);
#else

    printf("\e[0;0H");

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            //printf("%s", scr_buffer[UID(x, y)].atr);
            putchar(scr_buffer[UID(x,y)].c);
        }
    }

    printf("\e[0;0H");

    usleep(wait*10000);
#endif
}