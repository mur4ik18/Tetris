#ifndef DISPLAY_H
#define DISPLAY_H

// ANSI color escape codes
#define COLOR_RESET "\x1B[0m"
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN "\x1B[36m"
#define COLOR_WHITE "\x1B[37m"


void afficher();
void game_over();

#endif  // DISPLAY_H
