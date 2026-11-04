#ifndef MOVES_H
# define MOVES_H

extern int last_move[4];

int is_legal_move(int board[8][8], int fr, int fc, int tr, int tc);
int is_en_passant_capture(int board[8][8], int fr, int fc, int tr, int tc);
int find_king(int board[8][8], int color);
int is_in_check(int board[8][8], int color);
int is_valid_move(int board[8][8], int fr, int fc, int tr, int tc);
int has_any_moves(int board[8][8], int color);
void pawn_promotion(int board[8][8], int row, int col, int piece);

#endif