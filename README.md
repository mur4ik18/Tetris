# Tetris

On va avoir:

char FIELD[ROW][COLL] = {0};

<img width="559" alt="image" src="https://github.com/mur4ik18/Tetris/assets/79057640/9af70031-d82b-470d-bbb0-82a345fee532">

O

{0, 0, 0, 0}

{0, 0, 0, 0}

{1, 1, 0, 0}

{1, 1, 0, 0}

I

{1, 0, 0, 0}

{1, 0, 0, 0}

{1, 0, 0, 0}

{1, 0, 0, 0}

L

{0, 0, 0, 0}

{1, 0, 0, 0}

{1, 0, 0, 0}

{1, 1, 0, 0}

S

{0, 0, 0, 0}

{0, 0, 0, 0}

{0, 0, 1, 1}

{0, 1, 1, 0}

Z

{0, 0, 0, 0}

{0, 0, 0, 0}

{1, 1, 0, 0}

{0, 1, 1, 0}

J

{0, 0, 0, 0}

{0, 1, 0, 0}

{0, 1, 0, 0}

{1, 1, 0, 0}

T

{0, 0, 0, 0}

{0, 0, 0, 0}

{1, 1, 1, 0}

{0, 1, 0, 0}

Nous avons nos pieces, nous allons les prendre aleatoirement et on va le mettre dans une structure (current piece)
```
current piece
  x = 0
  y = (coll/2-1)
  peice = piece aletoire
```
Nous allons avoir une fonction qui va turner notre matrice en utilisant une algorythme de rotate en 90°


Mais tout d'abbord nous allons construire une champe de jeux

|__________|

|__________|

|__________|

+++++++++


Pour prendre des touches de joueurs nous allons chenger le mode de terminal en non-canonical mode. Nous alons lire input charracter par charracter et si nous avons une de touches suivants:

← espace → ↓ ou q  

Nous allons faire qqch.

Nos pieces vont étre déplacés chque "tick" (chaque 1 sec)

Si current_piece a touché le sol ou une autre pice - nous allons le lesé sur ce place. Nous allons mettre une autre piece dans current piece at nous allons anuller x et y

# Install
Install make

Then write it near makefile:
make claen
make
Lance le jeu
./tetris

