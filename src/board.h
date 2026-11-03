#ifndef BOARD_H
# define BOARD_H

# include "raylib.h"

# define BOARD_SIZE 640
# define SQUARE_SIZE 80
# define BOARD_X 80
# define BOARD_Y 80

extern int board[8][8];

void start_game(void);
void draw_board(void);
void init_board(void);
void draw_selection(void);
void handle_input(void);
void draw_message(const char *text, int y, Color color);

#endif