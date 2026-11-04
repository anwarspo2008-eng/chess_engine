#include "board.h"
#include "pieces.h"
#include "raylib.h"
#include "moves.h"

int board[8][8];
int selected_row = -1;
int selected_col = -1;
int white_turn = 1;
int game_over = 0;

void start_game(void)
{
    InitWindow(800, 800, "Chess");
    SetTargetFPS(60);

    init_board();

    while (!WindowShouldClose())
    {
        handle_input();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        draw_board();
        draw_selection();
        draw_pieces(board);
        if (game_over == 1)
            draw_message("CHECKMATE!", 20, RED);
        else if (game_over == 2)
            draw_message("STALEMATE!", 20, ORANGE);
        else if (is_in_check(board, white_turn))
            draw_message("CHECK!", 20, RED);

        EndDrawing();
    }

    CloseWindow();
}

void draw_board(void)
{
    int row;
    int col;
    Color color;

    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            if ((row + col) % 2 == 0)
                color = BEIGE;
            else
                color = BROWN;

            DrawRectangle(
                BOARD_X + col * SQUARE_SIZE,
                BOARD_Y + row * SQUARE_SIZE,
                SQUARE_SIZE,
                SQUARE_SIZE,
                color
            );

            col++;
        }
        row++;
    }
}

void init_board(void)
{
    int row;
    int col;

    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            board[row][col] = EMPTY;
            col++;
        }
        row++;
    }

    init_pieces(board);
}

void draw_message(const char *text, int y, Color color)
{
    int text_w;

    text_w = MeasureText(text, 40);
    DrawText(text, (800 - text_w) / 2, y, 40, color);
}

static int is_own_piece(int piece)
{
    if (piece == EMPTY)
        return 0;
    if (white_turn && piece > 0)
        return 1;
    if (!white_turn && piece < 0)
        return 1;
    return 0;
}

void draw_selection(void)
{
    int row;
    int col;

    if (selected_row == -1)
        return ;
    DrawRectangle(
        BOARD_X + selected_col * SQUARE_SIZE,
        BOARD_Y + selected_row * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE,
        Fade(YELLOW, 0.5f)
    );
    row = 0;
    while (row < 8)
    {
        col = 0;
        while (col < 8)
        {
            if (is_valid_move(board, selected_row, selected_col, row, col))
                DrawCircle(
                    BOARD_X + col * SQUARE_SIZE + SQUARE_SIZE / 2,
                    BOARD_Y + row * SQUARE_SIZE + SQUARE_SIZE / 2,
                    10,
                    Fade(DARKGREEN, 0.6f)
                );
            col++;
        }
        row++;
    }
}

void handle_input(void)
{
    Vector2 mouse;
    int row;
    int col;

    if (game_over != 0)
        return ;
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return ;
    mouse = GetMousePosition();
    if (mouse.x < BOARD_X || mouse.y < BOARD_Y
        || mouse.x >= BOARD_X + BOARD_SIZE
        || mouse.y >= BOARD_Y + BOARD_SIZE)
        return ;
    col = (int)(mouse.x - BOARD_X) / SQUARE_SIZE;
    row = (int)(mouse.y - BOARD_Y) / SQUARE_SIZE;
    if (selected_row == -1)
    {
        if (is_own_piece(board[row][col]))
        {
            selected_row = row;
            selected_col = col;
        }
        return ;
    }
    if (row == selected_row && col == selected_col)
    {
        selected_row = -1;
        selected_col = -1;
    }
    else if (is_own_piece(board[row][col]))
    {
        selected_row = row;
        selected_col = col;
    }
    else if (is_valid_move(board, selected_row, selected_col, row, col))
    {
        if (is_en_passant_capture(board, selected_row, selected_col, row, col))
            board[selected_row][col] = EMPTY;
        board[row][col] = board[selected_row][selected_col];
        board[selected_row][selected_col] = EMPTY;
        last_move[0] = selected_row;
        last_move[1] = selected_col;
        last_move[2] = row;
        last_move[3] = col;
        pawn_promotion(board, row, col, board[row][col]);
        selected_row = -1;
        selected_col = -1;
        white_turn = !white_turn;
        if (!has_any_moves(board, white_turn))
        {
            if (is_in_check(board, white_turn))
                game_over = 1;
            else
                game_over = 2;
        }
    }
}
