#include "raylib.h"

// Using this to figure out how to handle input while moving
//

#define screenWidth 800
#define screenHeight 450

void updateGame(void) {
    // TraceLog(LOG_INFO, "updating game");
}

void renderGame(void) {
    // TraceLog(LOG_INFO, "rendering game");
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "idk figure out");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        BeginDrawing();
        ClearBackground(BLACK);
        renderGame();
        EndDrawing();

    }

    return 0;
}
