#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define ROWS 20
#define COLS 10

// Ici on declare notre champe de jeux, on va l'utiliser pour sauvegarder les positions de chaque piece.
char FIELD[ROWS][COLS] = {0};

typedef struct
{
  char x;
  char y;
  char *piece;
}current_piece;



int main()
{
    printf("Hello world!\n");
    return 0;
}
