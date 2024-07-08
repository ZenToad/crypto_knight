#include "raylib.h"

#define screenWidth 800
#define screenHeight 450

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    InitAudioDevice();

    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();

    }

    return 0;
}

