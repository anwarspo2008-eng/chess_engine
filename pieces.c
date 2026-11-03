#include "pieces.h"

void init_pieces(int board[8][8])
{
    int i;

    i = 0;
    while (i < 8)
    {
        board[1][i] = -1;
        board[6][i] = 1;
        i++;
    }

    board[0][0] = -2;
    board[0][7] = -2;
    board[0][1] = -3;
    board[0][6] = -3;
    board[0][2] = -4;
    board[0][5] = -4;
    board[0][3] = -5;
    board[0][4] = -6;

    board[7][0] = 2;
    board[7][7] = 2;
    board[7][1] = 3;
    board[7][6] = 3;
    board[7][2] = 4;
    board[7][5] = 4;
    board[7][3] = 5;
    board[7][4] = 6;
}