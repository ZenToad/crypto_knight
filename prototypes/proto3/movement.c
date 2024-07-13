/*
 * the smooth movement seems do be fine.  Although, it lags behind the other
 * methods. 
 *
 * The issue then becomes with input coming in but the direction should not change
 * unless the player is has entered a new grid square.  The player can do something like
 * press left, and then while moving left, quickly press down then right, and the right,
 * which shouldnt' work because the player cannot move over themself, but because at that
 * point the player direction is down, this works fine.
 *
 * So what we need is a way to only change the direction once the player has completely 
 * moved over to the next grid square.  
 *
 * How do we know when the player has completely moved over to the next grid square?
 * can we just check the player position as an int, floor(pos) should work right?
 * and then when floor(pos) != floor(prev) we have moved to a new grid square.  we
 * can then process and input that has been captured while moving, and just 
 * don't process anything at that point that is an illegal move
 *
 * So wait, aren't we already doing this?  We have two squares drawn already for the
 * smooth movement, so we can just check if the player has moved over to the next grid
 * How can we sync up the smooth movement to hit the grid square at the same time as
 * the ones that snap?
 */


#include "raylib.h"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

#define GRID_SIZE 8 

#define SCREEN_WIDTH 2*800
#define SCREEN_HEIGHT 2*800

#define FIXED_TIMESTEP 1.0

struct {
    Vector2 pos;
    Vector2 dir;
    double accumulator;
} simple_loop;

struct {
    Vector2 currPos;
    Vector2 prevPos;
    Vector2 dir;
    double currentTime;
    double accumulator;
} continuous_loop;

struct {
    Vector2 pos;
    Vector2 dir;
    double accumulator;
} hybrid_loop;

double time;
double frameTime;


void initGame() {
    time = GetTime();
    simple_loop.pos = (Vector2) {0, 1};
    simple_loop.dir = (Vector2) {1, 0};

    continuous_loop.currPos = (Vector2) {0, 3};
    continuous_loop.prevPos = (Vector2) {0, 3};
    continuous_loop.dir = (Vector2) {1, 0};
    continuous_loop.currentTime = GetTime();
    continuous_loop.accumulator = 0;

    hybrid_loop.pos = (Vector2) {0, 5};
    hybrid_loop.dir = (Vector2) {1, 0};
    hybrid_loop.accumulator = 0;
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
    simple_loop.accumulator += frameTime;
    if (simple_loop.accumulator > FIXED_TIMESTEP) {
        simple_loop.accumulator -= FIXED_TIMESTEP;
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
    if ( frameTime > FIXED_TIMESTEP )
        frameTime = FIXED_TIMESTEP;
    continuous_loop.currentTime = newTime;
    continuous_loop.accumulator += frameTime;

    while (continuous_loop.accumulator >= FIXED_TIMESTEP) {
        continuous_loop.prevPos = continuous_loop.currPos;
        continuous_loop.currPos.x += continuous_loop.dir.x;
        continuous_loop.currPos.y += continuous_loop.dir.y;
        continuous_loop.accumulator -= FIXED_TIMESTEP;
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
       
    double alpha = continuous_loop.accumulator / FIXED_TIMESTEP;
    Vector2 pos = Vector2Lerp(continuous_loop.prevPos, continuous_loop.currPos, alpha);
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    DrawRectangle(pos.x * square_size , pos.y * square_size, square_size, square_size, RED);
}

void hybridUpdateGame(void) {

    hybrid_loop.accumulator += frameTime;

    while (hybrid_loop.accumulator >= FIXED_TIMESTEP) {
        hybrid_loop.pos.x += hybrid_loop.dir.x;
        hybrid_loop.pos.y += hybrid_loop.dir.y;
        hybrid_loop.accumulator -= FIXED_TIMESTEP;
        if (hybrid_loop.pos.x <= 0) {
            hybrid_loop.dir.x *= -1;
            hybrid_loop.pos.x = 0;
        } else if (hybrid_loop.pos.x >= GRID_SIZE - 1) {
            hybrid_loop.dir.x *= -1;
            hybrid_loop.pos.x = GRID_SIZE - 1;
        }
    }
}

void hybridRenderGame(void) {
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    DrawRectangle(hybrid_loop.pos.x * square_size , hybrid_loop.pos.y * square_size, square_size, square_size, BLUE);
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Movement test");
    SetTargetFPS(60);
                                    
    InitAudioDevice();
    initGame();

    double time = GetTime();
    // hang out and boot up bitches
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();
        if (GetTime() - time > FIXED_TIMESTEP) {
            break;
        }
       
    }


    while (!WindowShouldClose()) {   // Detect window close button or ESC key
                                     
        double newTime = GetTime();
        frameTime = newTime - continuous_loop.currentTime;
        if ( frameTime > FIXED_TIMESTEP )
            frameTime = FIXED_TIMESTEP;
        time = newTime;

        simpleUpdateGame();
        continuousUpdateGame();
        hybridUpdateGame();

        BeginDrawing();
        ClearBackground(BLACK);
        simpleRenderGame();
        continuousRenderGame();
        hybridRenderGame();

        drawGrid();
        // draw FPS
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);
        EndDrawing();

    }

    return 0;
}


