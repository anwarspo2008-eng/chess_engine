#ifndef MOVES_H
# define MOVES_H


int is_legal_move(int board[8][8], int from_row, int from_col, int to_row, int to_col);
void pawn_promotion(int board[8][8], int row, int col, int new_piece);
int find_king(int board[8][8], int color);
int is_in_check(int board[8][8], int color);
int is_valid_move(int board[8][8], int fr, int fc, int tr, int tc);
int has_any_moves(int board[8][8], int color);

#endif