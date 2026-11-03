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
 