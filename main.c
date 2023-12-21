#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define ROWS 10
#define COLS 20


#define LEFT   68
#define RIGHT  67
#define ROTATE 65
#define DOWN   66
#define EXIT   113

// Ici on declare notre champe de jeux, on va l'utiliser pour sauvegarder les positions de chaque piece.
char FIELD[ROWS][COLS] = {0};

struct termios saved_attributes;
typedef struct
{
  char x;
  char y;
  char *piece;
}current_piece;










// # ==== Terminal Input mode ==== #
void
reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void
set_input_mode (void)
{
  struct termios tattr;
  char *name;

  /* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

  /* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}






// # ==== MAIN ==== #

int main(void)
{
  char option = 1;
  FIELD[5][5] = 1;
  char ch;
  set_input_mode();
  while (option)
    {
      // =================== afficher le champ de jeux =========
      // a ameilleurer
      printf("\e[?25l");
      printf(":");

      for (size_t i =0; i<COLS; i++) {
	printf("-");
      }
      printf(":\n");

      for (size_t j=0; j<ROWS; j++){
	printf(":");
	for (size_t i =0; i<COLS; i++) {
	  if (FIELD[j][i] == 1) {
	    printf("#");
	  }
	  else {
	    printf(".");
	  }
	}
	printf(":\n");
      }
      printf(":");
      for (size_t i =0; i<COLS; i++) {
	printf("-");
      }
      printf(":\n");
      // Move cursor back to top
      printf("\e[%iA", ROWS+2); 



      
      
      // ==================== Obtenir la touche =================
      read (STDIN_FILENO, &ch, 1);
      if (ch == EXIT)
        break;
      if (ch == LEFT)
	printf("LEFT");
      if (ch == ROTATE)
	printf("ROTATE");
      if (ch == RIGHT)
	printf("RIGHT");
      if (ch == DOWN)
	printf("DOWN");
    }
 
  
  return 1;
}
