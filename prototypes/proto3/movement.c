#include "raylib.h"

#define GRID_SIZE 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

struct {
    Vector2 pos;
    Vector2 dir;
} simple_loop;

struct {
    Vector2 pos;
    Vector2 dir;
} smooth_loop;

struct {
    Vector2 pos;
    Vector2 dir;
} grid_loop;

struct {
    Vector2 pos;
    Vector2 dir;
} grid_smooth_loop;

void initGame() {
    // TraceLog(LOG_INFO, "initializing game");
}

// draw the grid.  split this grid into
// GRID_SIZE x GRID_SIZE squares
void drawGrid() {
    for (int i = 0; i < SCREEN_WIDTH; i += SCREEN_WIDTH / GRID_SIZE) {
        DrawLine(i, 0, i, SCREEN_HEIGHT, GRAY);
    }
    for (int i = 0; i < SCREEN_HEIGHT; i += SCREEN_HEIGHT / GRID_SIZE) {
        DrawLine(0, i, SCREEN_WIDTH, i, GRAY);
    }
}

void simpleUpdateGame() {
    static double time = 0;
    double dt = GetFrameTime();
    time += dt;
    if (time > 0.2) {
        time = 0;
    }
}

// draw the squares, but snap them to the grid
void simpleRenderGame() {
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    // DrawRectangle(horizontal.x * square_size , horizontal.y * square_size, square_size, square_size, GREEN);
    // TraceLog(LOG_INFO, "simple render");
}

void simpleGameLoop() {
    simpleUpdateGame();
    simpleRenderGame();
    // TraceLog(LOG_INFO, "simple loop");
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Movement test");
    SetTargetFPS(60);
                                    
    InitAudioDevice();

    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        BeginDrawing();
        ClearBackground(BLACK);

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();

    }

    return 0;
}


