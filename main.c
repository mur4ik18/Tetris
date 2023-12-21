#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

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

char* piece = {{{0, 0, 0, 0},{0, 0, 0, 0},{1, 1, 0, 0},{1, 1, 0, 0}}
    ,{{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0}}
    ,{{0, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 1, 0, 0}}
    ,{{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 1, 1},{0, 1, 1, 0}}
    ,{{0, 0, 0, 0},{0, 0, 0, 0},{1, 1, 0, 0},{0, 1, 1, 0}}
    ,{{0, 0, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{1, 1, 0, 0}}
    ,{{0, 0, 0, 0},{0, 0, 0, 0},{1, 1, 1, 0},{0, 1, 0, 0}}};

typedef struct
{
  char x;
  char y;
  char piece[4][4];
}Current_piece;

Current_piece current_piece;








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


void afficher(void)
{
      // =================== afficher le champ de jeux =========
      // a ameilleurer
      printf("\e[?25l");
      printf(":");

      for (size_t i =0; i<COLS; i++) {
	printf("-");
      }
      printf(":\n");

      for (size_t j=ROWS; j>0; j--){
	printf(":");
	for (size_t i=0; i<COLS; i++) {
	  if (FIELD[j][i] == 1) {
	    printf("#");
	  }
	  else if ((((current_piece.x + 3) >= j) && (j >= current_piece.x)) && (((current_piece.y + 3) >= i) && (i >= current_piece.y)))
	    {
	      printf("%d\n", current_piece.piece[0]);
	      
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

}

void init_piece(void)
{
  current_piece.x = ROWS;
  current_piece.y = COLS/2;
  for (int i = 0; )
  
}


// # ==== MAIN ==== #

int main(void)
{
  char option = 1;
  char ch;
  set_input_mode();
  int time = 1000;

  init_piece();
  
  while (option)
    {
      afficher();
	
      

      //printf("%d\n",piece[0][3][2]);
      //      usleep(game_speed);


      if (current_piece.x == 0)
	{
	  /* for (int x = 0; x < 3; x++) */
	  /*   { */
	  /*     for (int y = 0; y < 3; y++) */
	  /* 	{ */
	  /* 	  FIELD[current_piece.x][current_piece.y] = 1; */
	  /* 	} */
	  /*   } */
	  FIELD[current_piece.x][current_piece.y] = 1;
	  init_piece();
	}

      if (time >= 1000) {
	current_piece.x--;
	time = 0;
      }
      
      
      // ==================== Obtenir la touche =================
      //read (STDIN_FILENO, &ch, 1);

      struct timeval tv;
      fd_set fds;
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);
      select(STDIN_FILENO +1, &fds, NULL, NULL, &tv);
      if(FD_ISSET(STDIN_FILENO, &fds)) {
	char ch = getchar();
	if (ch == EXIT)
	  break;
	if (ch == LEFT)
	  current_piece.y--;
	if (ch == ROTATE)
	  printf("ROTATE");
	if (ch == RIGHT)
	  current_piece.y++;
	if (ch == DOWN)
	  printf("DOWN");
      }
      time++;
    }
 
  
  return 1;
}
