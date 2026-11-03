#ifndef BOARD_H
# define BOARD_H

# define BOARD_SIZE 640
# define SQUARE_SIZE 80
# define BOARD_X 80
# define BOARD_Y 80

extern int board[8][8];

void draw_board(void);
void init_board(void);

#endif