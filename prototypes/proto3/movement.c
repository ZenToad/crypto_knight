#include "raylib.h"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

#define GRID_SIZE 20

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

struct {
    Vector2 pos;
    Vector2 dir;
    double time;
    double dt;
} simple_loop;

struct {
    Vector2 currPos;
    Vector2 prevPos;
    Vector2 dir;
    double t;
    double dt;
    double currentTime;
    double accumulator;
} continuous_loop;

void initGame() {
    simple_loop.pos = (Vector2) {0, 1};
    simple_loop.dir = (Vector2) {1, 0};
    simple_loop.time = 0;
    simple_loop.dt = 0;

    continuous_loop.currPos = (Vector2) {0, 3};
    continuous_loop.prevPos = (Vector2) {0, 3};
    continuous_loop.dir = (Vector2) {1, 0};
    continuous_loop.t = 0;
    continuous_loop.dt = 0.2;
    continuous_loop.currentTime = GetTime();
    continuous_loop.accumulator = 0;
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

void continuousUpdateGame(void) {
    double newTime = GetTime();
    double frameTime = newTime - continuous_loop.currentTime;
    if ( frameTime > 0.25 )
        frameTime = 0.25;
    continuous_loop.currentTime = newTime;
    continuous_loop.accumulator += frameTime;

    while (continuous_loop.accumulator >= continuous_loop.dt) {
        continuous_loop.prevPos = continuous_loop.currPos;
        continuous_loop.currPos.x += continuous_loop.dir.x;
        continuous_loop.currPos.y += continuous_loop.dir.y;
        continuous_loop.t += continuous_loop.dt;
        continuous_loop.accumulator -= continuous_loop.dt;
        if (continuous_loop.currPos.x <= 0) {
            continuous_loop.dir.x *= -1;
            continuous_loop.currPos.x = 0;
        } else if (continuous_loop.currPos.x >= GRID_SIZE - 1) {
            continuous_loop.dir.x *= -1;
            continuous_loop.currPos.x = GRID_SIZE - 1;
        }
    }
}

void continuousRenderGame(void) {
    DrawRectangle(continuous_loop.currPos.x * SCREEN_WIDTH / GRID_SIZE, continuous_loop.currPos.y * SCREEN_HEIGHT / GRID_SIZE, SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE, BLUE);
       
    double alpha = continuous_loop.accumulator / continuous_loop.dt;
    Vector2 pos = Vector2Lerp(continuous_loop.prevPos, continuous_loop.currPos, alpha);
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    DrawRectangle(pos.x * square_size , pos.y * square_size, square_size, square_size, RED);
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Movement test");
    SetTargetFPS(60);
                                    
    InitAudioDevice();

    initGame();

    while (!WindowShouldClose()) {   // Detect window close button or ESC key
                                     
        simpleUpdateGame();
        continuousUpdateGame();

        BeginDrawing();
        ClearBackground(BLACK);
        simpleRenderGame();
        continuousRenderGame();

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();

    }

    return 0;
}


