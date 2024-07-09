#include "raylib.h"

#define GRID_SIZE 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

struct {
    Vector2 pos;
    Vector2 dir;
    double time;
    double dt;
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
    simple_loop.pos = (Vector2) {0, 1};
    simple_loop.dir = (Vector2) {1, 0};
    simple_loop.time = 0;
    simple_loop.dt = 0;

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
    simple_loop.dt = GetFrameTime();
    simple_loop.time += simple_loop.dt;
    if (simple_loop.time > 0.2) {
        simple_loop.time -= 0.2;
        simple_loop.pos.x += simple_loop.dir.x;
        simple_loop.pos.y += simple_loop.dir.y;
        if (simple_loop.pos.x <= 0) {
            simple_loop.dir.x *= -1;
            simple_loop.pos.x = 0;
        } else if (simple_loop.pos.x >= GRID_SIZE - 1) {
            simple_loop.dir.x *= -1;
            simple_loop.pos.x = GRID_SIZE - 1;
        }
    }
}

// draw the squares, but snap them to the grid
void simpleRenderGame() {
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    DrawRectangle(simple_loop.pos.x * square_size , simple_loop.pos.y * square_size, square_size, square_size, GREEN);
}

void simpleGameLoop() {
    simpleUpdateGame();
    simpleRenderGame();
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Movement test");
    SetTargetFPS(60);
                                    
    InitAudioDevice();

    initGame();

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        simpleGameLoop();
        BeginDrawing();
        ClearBackground(BLACK);

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();

    }

    return 0;
}


