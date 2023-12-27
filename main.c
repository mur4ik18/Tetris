#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <time.h>

#define ROWS 15
#define COLS 20


#define LEFT   68
#define RIGHT  67
#define ROTATE 32
#define DOWN   66
#define EXIT   113
#define GAME_SPEED 5000
#define PIECE_SIZE 4

// Ici on declare notre champe de jeux, on va l'utiliser pour sauvegarder les positions de chaque piece.
char FIELD[ROWS][COLS];


struct termios saved_attributes;

char piece[7][4][4] = {{{1, 1, 0, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0}}
                       ,{{1, 1, 0, 0},{1, 0, 0, 0},{1, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 1, 0},{0, 0, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}}
                       ,{{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}
                       ,{{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}};

typedef struct
{
  char x;
  char y;
  char piece[4][4];
}Current;

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

void afficher(Current current_piece)
{
  // =================== afficher le champ de jeux =========
  // a ameilleurer
  printf("\e[?25l");

  for (size_t j=ROWS-1; j>0; j--){
    printf(":");
    for (size_t i=0; i<COLS; i++) {
      if (FIELD[j][i] == 1) {
        printf("#");
      }
      else if ((((current_piece.x + 3) >= j) && (j >= current_piece.x)) && (((current_piece.y + 3) >= i) && (i >= current_piece.y)))
        {
          if (current_piece.piece[j - current_piece.x][i - current_piece.y])
            printf("0");
          else
            printf(".");
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
  printf("\e[%iA", ROWS); 

}

int check_collision(Current current_piece)
{
  // ======== bas ==========
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x -1][current_piece.y +y] == 1))
          return 1;
      }
  }
  // ======== droit ========
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x][current_piece.y +y + 1] == 1))
          return 3;
      }
  }
  // ======== gauche =======
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++)
      {
        //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
        if ((current_piece.piece[x][y] == 1) && (FIELD[current_piece.x + x][current_piece.y + y - 1] == 1))
          return 2;
      }
  }
  return 0;
}


void game_field_init(void)
{
  for (int i = 0; i<ROWS; i++)
    {
      for (int j = 0; j<COLS; j++)
        {
          if (j == 0)
            FIELD[i][j] = 1;
          else if (j == COLS -1)
            FIELD[i][j] = 1;
          else if (i > 0)
            FIELD[i][j] = 0;
          else
            FIELD[i][j] = 1;
        }
    }
}

void rotate(Current *current_piece) {
    char temp[4][4];

    // Transpose the piece
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = current_piece->piece[j][i];
        }
    }

    // Reverse each row to implement the rotation
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            char swap = temp[i][j];
            temp[i][j] = temp[i][3 - j];
            temp[i][3 - j] = swap;
        }
    }
    // If rotation is valid, update the piece
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            current_piece->piece[i][j] = temp[i][j];
        }
    }
}
// Function to move the piece down as fast as possible
void fastDown(Current *current_piece) {
  while (!check_collision(*current_piece)) {
    current_piece->x--;
  }
  current_piece->x++;  // Adjust for the last valid position
}


Current init_piece(Current current_piece)
{
  current_piece.x = ROWS-1;
  current_piece.y = COLS/2;


  // Seed the random number generator with the current time
  srand(time(NULL));

  // Generate a random number between 0 and 6
  int randomNumber = rand() % 7;


  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      current_piece.piece[i][j] = piece[randomNumber][i][j];
    }
  }
  return current_piece;
}

void fin(void)
{
  printf("\e[?25l");  // Hide the cursor

  // Clear the screen
  for (int x = 0; x < ROWS + 2; x++)
    {
      for (int y = 0; y < COLS + 2; y++)
        {
          printf(" ");
        }
      printf("\n");
    }

  // Move cursor to the center of the screen
  printf("\e[%iA", ROWS + 2);
  printf("\e[%iC", COLS / 2 - 5);

  printf("GAME OVER!\n");

  // Wait for user input to exit
  printf("Press 'q' to quit...\n");
  char ch;
  while (1)
    {
      if (read(STDIN_FILENO, &ch, 1) > 0)
        {
          if (ch == 'q')
            {
              printf("\e[?25h");  // Show the cursor
              exit(EXIT_SUCCESS);
            }
          else
            printf("%d\n", ch);
        }
    }
}


// # ==== MAIN ==== #

int main(void)
{
  char option = 1;
  char ch;
  game_field_init();
  set_input_mode();
  int time = GAME_SPEED;
  Current current_piece;
  current_piece =  init_piece(current_piece);
  while (option)
    {
      afficher(current_piece);
      //printf("%d\n", current_piece.x);
      // ================= verification de la colision ================

      if (time >= GAME_SPEED) {
        //usleep(GAME_SPEED*100);
        if (check_collision(current_piece) == 1)
          {
            for (int x = 0; x < 4; x++)
              {
                for (int y = 0; y < 4; y++)
                  {
                    //printf("%d %d = %d %d == %d %d\n", current_piece.x, current_piece.y, x, y, current_piece.x + x, current_piece.y + y);
                    if (current_piece.piece[x][y] == 1) 
                      FIELD[current_piece.x + x][current_piece.y + y] = 1;
                  }
              }
            current_piece = init_piece(current_piece);
            for (int y = 1; y<COLS-1; y++)
              {
                if (FIELD[ROWS-1][y] == 1) {
                  fin();
                }
              }
          }
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
        if (ch == LEFT) {
          if ((check_collision(current_piece) != 2) && (check_collision(current_piece) != 1))
            current_piece.y--;
        }
        if (ch == ROTATE)
          rotate(&current_piece);
        if (ch == RIGHT)
          {
            if ((check_collision(current_piece) != 3) && (check_collision(current_piece) != 1))
              current_piece.y++;
          }
        if (ch == DOWN)
          fastDown(&current_piece);
      }
      time++;
    }

  printf("\e[?25h");
  return 1;
}
