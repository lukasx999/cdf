#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "raylib [core] example - basic window");

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();
    return EXIT_SUCCESS;
}
