#include "raylib.h"

#define GRID_SIZE 10

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

typedef enum game_loop_t {
    SIMPLE,
    SMOOTH,
    GRID,
    GRID_SMOOTH,
} game_loop_t;
game_loop_t game_loop_type = SIMPLE;

struct {
    game_loop_t type;
} simple_loop;

struct {
    game_loop_t type;
} smooth_loop;

struct {
    game_loop_t type;
} grid_loop;

struct {
    game_loop_t type;
} grid_smooth_loop;

typedef struct bounding_box_t {
    Vector2 topleft;
    Vector2 bottomright;
} bounding_box_t;

// want to test different types of movement algorithms
// the grid based movement is nice, but jumpy.
// smooth movement is nice, but not grid based.
// maybe a mix of both?
//
// Start with some squares bouncing around the screen in random
// No we need some grid movement, they don't do any diagonal movement.
// 
bounding_box_t clockwise_bb = { { 1, 1 }, {GRID_SIZE - 2, GRID_SIZE - 2} };
Vector2 clockwise = { 1, 1 };
Vector2 clockwiseDir = { 1, 0 };

bounding_box_t counterClockwise_bb = { { 0, 0 }, {GRID_SIZE - 1, GRID_SIZE - 1} };
Vector2 counterClockwise = { 0, 0 };
Vector2 counterClockwiseDir = { 0, 1 };

Vector2 horizontal = { 0, 5 };
Vector2 horizontalDir = { 1, 0 };

Vector2 vertical = { 4, 0 };
Vector2 verticalDir = { 0, 1 };

void initGame() {
    // TraceLog(LOG_INFO, "initializing game");
    simple_loop.type = SIMPLE;
    smooth_loop.type = SMOOTH;
    grid_loop.type = GRID;
    grid_smooth_loop.type = GRID_SMOOTH;
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
        clockwise.x += clockwiseDir.x;
        clockwise.y += clockwiseDir.y;
        counterClockwise.x += counterClockwiseDir.x;
        counterClockwise.y += counterClockwiseDir.y;
        vertical.x += verticalDir.x;
        vertical.y += verticalDir.y;
        horizontal.x += horizontalDir.x;
        horizontal.y += horizontalDir.y;
    }

    if (horizontal.x >= GRID_SIZE) {
        horizontalDir.x = -1;
        horizontal.x = GRID_SIZE - 2;
    } else if (horizontal.x < 0) {
        horizontalDir.x = 1;
        horizontal.x = 1;
    }

    if (vertical.y >= GRID_SIZE) {
        verticalDir.y = -1;
        vertical.y = GRID_SIZE - 2;
    } else if (vertical.y < 0) {
        verticalDir.y = 1;
        vertical.y = 1;
    }
TODO: this is really cool but really we should have 4 different boxes
moving from left to right at the same speed so we can see the differences
    // clockwise movement
    if (clockwise.x > clockwise_bb.bottomright.x) {
        clockwise.x = clockwise_bb.bottomright.x;
        clockwise.y = clockwise_bb.topleft.y + 1;
        clockwiseDir.x = 0;
        clockwiseDir.y = 1;
    } else if (clockwise.y > clockwise_bb.bottomright.y) {
        clockwise.x = clockwise_bb.bottomright.x - 1;
        clockwise.y = clockwise_bb.bottomright.y;
        clockwiseDir.y = 0;
        clockwiseDir.x = -1;
    } else if (clockwise.x < clockwise_bb.topleft.x) {
        clockwise.x = clockwise_bb.topleft.x;
        clockwise.y = clockwise_bb.bottomright.y - 1;
        clockwiseDir.x = 0;
        clockwiseDir.y = -1;
    } else if (clockwise.y < clockwise_bb.topleft.y) {
        clockwise.x = clockwise_bb.topleft.x + 1;
        clockwise.y = clockwise_bb.topleft.y;
        clockwiseDir.y = 0;
        clockwiseDir.x = 1;
    } 

    // counter clockwise movement
    if (counterClockwise.y > counterClockwise_bb.bottomright.y) {
        counterClockwise.x = counterClockwise_bb.topleft.x + 1;
        counterClockwise.y = counterClockwise_bb.bottomright.y;
        counterClockwiseDir.y = 0;
        counterClockwiseDir.x = 1;
    } else if (counterClockwise.x > counterClockwise_bb.bottomright.x) {
        counterClockwise.x = 
    }
}

// draw the squares, but snap them to the grid
void simpleRenderGame() {
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    // DrawRectangle(clockwise.x * square_size , clockwise.y * square_size, square_size, square_size, RED);
    DrawRectangle(counterClockwise.x * square_size , counterClockwise.y * square_size, square_size, square_size, BLUE);
    // DrawRectangle(horizontal.x * square_size , horizontal.y * square_size, square_size, square_size, GREEN);
    // DrawRectangle(vertical.x * square_size , vertical.y * square_size, square_size, square_size, YELLOW);


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

        if (game_loop_type == SIMPLE) {
            simpleGameLoop();
        } else if (game_loop_type == SMOOTH) {
            // smoothGameLoop();
        } else if (game_loop_type == GRID) {
            // gridGameLoop();
        } else if (game_loop_type == GRID_SMOOTH) {
            // gridSmoothGameLoop();
        }
               
        BeginDrawing();
        ClearBackground(BLACK);

        if (game_loop_type == SIMPLE) {
            simpleRenderGame();
        } else if (game_loop_type == SMOOTH) {
            // smoothRenderGame();
        } else if (game_loop_type == GRID) {
            // gridRenderGame();
        } else if (game_loop_type == GRID_SMOOTH) {
            // gridSmoothRenderGame();
        }

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();

    }

    return 0;
}


