#include "board.h"
#include "pieces.h"
#include "raylib.h"

int board[8][8];

void start_game(void)
{
    InitWindow(800, 800, "Chess");
    SetTargetFPS(60);

    init_board();
    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        draw_board();
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
                color = BROWN;
            else
                color = WHITE;

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
            board[row][col] = 0;
            col++;
        }
        row++;
    }

    init_pieces(board);
}

