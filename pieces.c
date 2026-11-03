#include "pieces.h"
#include "board.h"
#include "raylib.h"

void init_pieces(int board[8][8])
{
    int i;

    i = 0;
    while (i < 8)
    {
        board[1][i] = BLACK_PAWN;
        board[6][i] = WHITE_PAWN;
        i++;
    }

    board[0][0] = BLACK_ROOK;
    board[0][7] = BLACK_ROOK;
    board[0][1] = BLACK_KNIGHT;
    board[0][6] = BLACK_KNIGHT;
    board[0][2] = BLACK_BISHOP;
    board[0][5] = BLACK_BISHOP;
    board[0][3] = BLACK_QUEEN;
    board[0][4] = BLACK_KING;

    board[7][0] = WHITE_ROOK;
    board[7][7] = WHITE_ROOK;
    board[7][1] = WHITE_KNIGHT;
    board[7][6] = WHITE_KNIGHT;
    board[7][2] = WHITE_BISHOP;
    board[7][5] = WHITE_BISHOP;
    board[7][3] = WHITE_QUEEN;
    board[7][4] = WHITE_KING;
}

void draw_pieces(int board[8][8])
{
    int row;
    int col;
    int x;
    int y;
    char piece[2];

    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            if (board[row][col] != 0)
            {
                x = BOARD_X + col * SQUARE_SIZE;
                y = BOARD_Y + row * SQUARE_SIZE;

                piece[0] = piece_letter(board[row][col]);
                piece[1] = '\0';

                DrawText(piece, x + 25, y + 20, 40, BLACK);
            }
            col++;
        }
        row++;
    }
}

static char piece_letter(int piece)
{
    switch (piece)
    {
        case WHITE_PAWN:
            return 'P';
        case WHITE_ROOK:
            return 'R';
        case WHITE_KNIGHT:
            return 'N';
        case WHITE_BISHOP:
            return 'B';
        case WHITE_QUEEN:
            return 'Q';
        case WHITE_KING:
            return 'K';
        case BLACK_PAWN:
            return 'p';
        case BLACK_ROOK:
            return 'r';
        case BLACK_KNIGHT:
            return 'n';
        case BLACK_BISHOP:
            return 'b';
        case BLACK_QUEEN:
            return 'q';
        case BLACK_KING:
            return 'k';
        default:
            return '?';
    }
}