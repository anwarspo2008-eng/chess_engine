#include "moves.h"
#include "pieces.h"

static int abs_val(int n){
    if (n < 0)
        return -n;
    return n;
}

static int sign(int n){
    if (n < 0)
        return -1;
    if (n > 0)
        return 1;
    return 0;
}

static int pawn_ok(int board[8][8], int fr, int fc, int tr, int tc)
{
    int dir;
    int start_row;
    int dr;
    int dc;

    if (board[fr][fc] > 0)
    {
        dir = -1;       
        start_row = 6;
    }
    else
    {
        dir = 1;        
        start_row = 1;
    }
    dr = tr - fr;
    dc = tc - fc;
    if (dc == 0 && dr == dir && board[tr][tc] == EMPTY)
        return 1;
    if (dc == 0 && dr == 2 * dir && fr == start_row
        && board[fr + dir][fc] == EMPTY && board[tr][tc] == EMPTY)
        return 1;
    if (abs_val(dc) == 1 && dr == dir && board[tr][tc] != EMPTY)
        return 1;
    return 0;
}

void pawn_promotion(int board[8][8], int row, int col, int new_piece)
{
    if (board[row][col] == WHITE_PAWN && row == 0)
        board[row][col] = WHITE_QUEEN; // Promote to queen by default
    else if (board[row][col] == BLACK_PAWN && row == 7)
        board[row][col] = BLACK_QUEEN; // Promote to queen by default
}

static int path_clear(int board[8][8], int fr, int fc, int tr, int tc)
{
    int step_r;
    int step_c;
    int r;
    int c;

    step_r = sign(tr - fr);
    step_c = sign(tc - fc);
    r = fr + step_r;
    c = fc + step_c;
    while (r != tr || c != tc)
    {
        if (board[r][c] != EMPTY)
            return 0;
        r += step_r;
        c += step_c;
    }
    return 1;
}

int is_legal_move(int board[8][8], int fr, int fc, int tr, int tc){
    int piece ;
    int type;
    int dr;
    int dc;

    piece = board[fr][fc];
    dr = tr - fr;
    dc = tc - fc;

    if (dr == 0 && dc == 0)
        return 0;
    if (piece * board[tr][tc] > 0)
        return 0;
    type = abs_val(piece);
    if (type == WHITE_PAWN)
        return pawn_ok(board, fr, fc, tr, tc);
     if (type == WHITE_KNIGHT)
        return (abs_val(dr) == 2 && abs_val(dc) == 1)
            || (abs_val(dr) == 1 && abs_val(dc) == 2);
    if (type == WHITE_KING)
        return abs_val(dr) <= 1 && abs_val(dc) <= 1;
    if (type == WHITE_ROOK || type == WHITE_QUEEN)
        if ((dr == 0 || dc == 0) && path_clear(board, fr, fc, tr, tc))
            return 1;
    if (type == WHITE_BISHOP || type == WHITE_QUEEN)
        if (abs_val(dr) == abs_val(dc) && path_clear(board, fr, fc, tr, tc))
            return 1;
    return 0;  
} 

int find_king(int board[8][8], int color)
{
    int row;
    int col;
    int king_piece;

    if (color == 1)
        king_piece = WHITE_KING;
    else
        king_piece = BLACK_KING;
    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            if (board[row][col] == king_piece)
                return row * 8 + col;
            col++;
        }
        row++;
    }
    return -1;
}

int is_in_check(int board[8][8], int color)
{
    int row;
    int col;
    int pos;
    int king_row;
    int king_col;
    int piece;
    int is_enemy;

    pos = find_king(board, color);
    if (pos == -1)
        return 0;
    king_row = pos / 8;
    king_col = pos % 8;
    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            piece = board[row][col];
            is_enemy = (piece != EMPTY) && ((piece > 0) != (color > 0));
            if (is_enemy && is_legal_move(board, row, col, king_row, king_col))
                return 1;
            col++;
        }
        row++;
    }
    return 0;
}

int is_valid_move(int board[8][8], int fr, int fc, int tr, int tc)

{
    int piece;
    int captured;
    int color;
    int result;

    piece = board[fr][fc];
    if (piece == EMPTY)
        return 0;
    if (!is_legal_move(board, fr, fc, tr, tc))
        return 0;
    color = (piece > 0) ? 1 : 0;
    captured = board[tr][tc];
    board[tr][tc] = piece;
    board[fr][fc] = EMPTY;
    result = !is_in_check(board, color);
    board[fr][fc] = piece;
    board[tr][tc] = captured;
    return result;
}

int has_any_moves(int board[8][8], int color)
{
    int row;
    int col;
    int tr;
    int tc;

    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            if ((board[row][col] != EMPTY) && ((board[row][col] > 0) == (color > 0)))
            {
                tr = 0;
                while (tr < 8)
                {
                    tc = 0;
                    while (tc < 8)
                    {
                        if (is_valid_move(board, row, col, tr, tc))
                            return 1;
                        tc++;
                    }
                    tr++;
                }
            }
            col++;
        }
        row++;
    }
    return 0;
}