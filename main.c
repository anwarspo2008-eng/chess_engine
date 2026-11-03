#include "raylib.h"
#include "board.h"

int main(void)
{
    InitWindow(800, 800, "Chess");
    SetTargetFPS(60);

    init_board();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        draw_board();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}