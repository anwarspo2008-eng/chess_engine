#include "board.h"
#include "pieces.h"
#include "raylib.h"

int board[8][8];
int selected_row = -1;
int selected_col = -1;
int white_turn = 1;

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
    if (selected_row == -1)
        return ;
    DrawRectangle(
        BOARD_X + selected_col * SQUARE_SIZE,
        BOARD_Y + selected_row * SQUARE_SIZE,
        SQUARE_SIZE,
        SQUARE_SIZE,
        Fade(YELLOW, 0.5f)
    );
}

void handle_input(void)
{
    Vector2 mouse;
    int row;
    int col;

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
    else
    {
        board[row][col] = board[selected_row][selected_col];
        board[selected_row][selected_col] = EMPTY;
        selected_row = -1;
        selected_col = -1;
        white_turn = !white_turn;
    }
}