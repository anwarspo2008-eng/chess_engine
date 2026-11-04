#include "moves.h"
#include "pieces.h"

int last_move[4] = {-1, -1, -1, -1};
int has_moved[8][8];

static int abs_val(int n)
{
    if (n < 0)
        return -n;
    return n;
}

static int sign(int n)
{
    if (n > 0)
        return 1;
    if (n < 0)
        return -1;
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

static int en_passant_ok(int board[8][8], int fr, int fc, int tr, int tc,
                         int last[4])
{
    int mover;
    int victim;

    if (last[0] == -1)
        return 0;
    mover = board[fr][fc];
    victim = board[last[2]][last[3]];
    if (abs_val(mover) != WHITE_PAWN || abs_val(victim) != WHITE_PAWN)
        return 0;
    if (mover * victim > 0)
        return 0;
    if (abs_val(last[2] - last[0]) != 2)
        return 0;
    if (fr != last[2])
        return 0;
    if (abs_val(fc - tc) != 1 || tc != last[3])
        return 0;
    if (tr != last[2] + sign(victim))
        return 0;
    return 1;
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
    if (en_passant_ok(board, fr, fc, tr, tc, last_move))
        return 1;
    return 0;
}

int is_legal_move(int board[8][8], int fr, int fc, int tr, int tc)
{
    int piece;
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

int is_en_passant_capture(int board[8][8], int fr, int fc, int tr, int tc)
{
    return abs_val(board[fr][fc]) == WHITE_PAWN
        && fc != tc && board[tr][tc] == EMPTY;
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

static int king_safe_on(int board[8][8], int fr, int fc, int tr, int tc,
                        int color)
{
    int king;
    int result;

    king = board[fr][fc];
    board[fr][fc] = EMPTY;
    board[tr][tc] = king;
    result = !is_in_check(board, color);
    board[tr][tc] = EMPTY;
    board[fr][fc] = king;
    return result;
}

static int castle_ok(int board[8][8], int fr, int fc, int tr, int tc)
{
    int color;
    int rook;
    int rook_col;
    int step;

    color = (board[fr][fc] > 0) ? 1 : 0;
    rook = (color == 1) ? WHITE_ROOK : -WHITE_ROOK;
    if (fr != ((color == 1) ? 7 : 0) || fc != 4)
        return 0;
    if (board[tr][tc] != EMPTY)
        return 0;
    if (tc == 6)
        rook_col = 7;
    else if (tc == 2)
        rook_col = 0;
    else
        return 0;
    if (has_moved[fr][fc] || has_moved[fr][rook_col])
        return 0;
    if (board[fr][rook_col] != rook)
        return 0;
    if (!path_clear(board, fr, fc, fr, rook_col))
        return 0;
    if (is_in_check(board, color))
        return 0;
    step = (tc > fc) ? 1 : -1;
    if (!king_safe_on(board, fr, fc, fr, fc + step, color))
        return 0;
    if (!king_safe_on(board, fr, fc, tr, tc, color))
        return 0;
    return 1;
}

int is_castling_move(int board[8][8], int fr, int fc, int tr, int tc)
{
    return abs_val(board[fr][fc]) == WHITE_KING
        && fr == tr && abs_val(tc - fc) == 2;
}

void castle_move_rook(int board[8][8], int row, int king_col)
{
    if (king_col == 6)
    {
        board[row][5] = board[row][7];
        board[row][7] = EMPTY;
    }
    else
    {
        board[row][3] = board[row][0];
        board[row][0] = EMPTY;
    }
}

int is_valid_move(int board[8][8], int fr, int fc, int tr, int tc)
{
    int piece;
    int captured;
    int ep_piece;
    int is_ep;
    int color;
    int result;

    piece = board[fr][fc];
    if (piece == EMPTY)
        return 0;
    if (is_castling_move(board, fr, fc, tr, tc))
        return castle_ok(board, fr, fc, tr, tc);
    if (!is_legal_move(board, fr, fc, tr, tc))
        return 0;
    color = (piece > 0) ? 1 : 0;
    is_ep = is_en_passant_capture(board, fr, fc, tr, tc);
    captured = board[tr][tc];
    ep_piece = board[fr][tc];
    board[tr][tc] = piece;
    board[fr][fc] = EMPTY;
    if (is_ep)
        board[fr][tc] = EMPTY;
    result = !is_in_check(board, color);
    board[fr][fc] = piece;
    board[tr][tc] = captured;
    if (is_ep)
        board[fr][tc] = ep_piece;
    return result;
}

int has_any_moves(int board[8][8], int color)
{
    int fr;
    int fc;
    int tr;
    int tc;
    int piece;

    fr = 0;
    while (fr < 8)
    {
        fc = 0;
        while (fc < 8)
        {
            piece = board[fr][fc];
            if (piece != EMPTY && (piece > 0) == (color > 0))
            {
                tr = 0;
                while (tr < 8)
                {
                    tc = 0;
                    while (tc < 8)
                    {
                        if (is_valid_move(board, fr, fc, tr, tc))
                            return 1;
                        tc++;
                    }
                    tr++;
                }
            }
            fc++;
        }
        fr++;
    }
    return 0;
}

void pawn_promotion(int board[8][8], int row, int col, int piece)
{
    if (piece == WHITE_PAWN && row == 0)
        board[row][col] = WHITE_QUEEN;
    else if (piece == BLACK_PAWN && row == 7)
        board[row][col] = BLACK_QUEEN;
}