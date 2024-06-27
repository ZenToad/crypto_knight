#include "raylib.h"
#include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const int SNAKE_FOOD = 43;

Vector2 position = {0, 0};

/* 
 *
 * How do we even test this thing?
 * snake moves, but it's body follows, and grows.
 * Interesting problem to solve.
 */

// need to randomly spawn food
// make sure that food doesn't spawn on the snake
void spawnFood(int *grid, int w, int h) {
    int x = GetRandomValue(0, w - 1);
    int y = GetRandomValue(0, h - 1);
    if (grid[x * w + y] == 0) {
        grid[x * w + y] = SNAKE_FOOD;
    } else {
        spawnFood(grid, w, h);
    }
}

int main(void) {

    int snakeHead = 5;

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    InitAudioDevice();
    int grid[GRID_SIZE][GRID_SIZE] = {0};

    position.x = 2;
    position.y = 2;

    grid[(int)position.x][(int)position.y] = snakeHead;

    bool death = false;
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        bool moved = false;
        // Process Input
        //----------------------------------------------------------------------------------
        
        // checking if dir is 0 is just a hack to get the snake
        // to never move back where it came.  if it is going right
        // and we keep pressing right nothing will happend.
        // and if we are going right and press left nothing
        // also happens.
        // we could just have a grid, and as we move the snake
        // through each grid cell we can have a number.
        //
        // for example, if the grid has a 3, then when the snake is moved,
        // we put the head somewhere else, and decrement all other squares,
        // and draw anything in the grid not 0
        //
        // Now, if the snake hits the edge or itself turn it red, just to check if
        // we got that working.  Moving in another direction will go back to green
        if (IsKeyPressed(KEY_RIGHT)) {
            if (position.x + 1 < GRID_SIZE) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.x += 1;
                if (grid[(int)position.x][(int)position.y] != 0) {
                    death = true;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    moved = true;
                    death = false;
                }
            } else {
                death = true;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (position.x - 1 >= 0) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.x -= 1;
                if (grid[(int)position.x][(int)position.y] != 0) {
                    death = true;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    moved = true;
                    death = false;
                }
            } else {
                death = true;
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            if (position.y - 1 >= 0) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.y -= 1;
                if (grid[(int)position.x][(int)position.y] != 0) {
                    death = true;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    moved = true;
                    death = false;
                }
            } else {
                death = true;
            }

        }

        if (IsKeyPressed(KEY_DOWN)) {
            if (position.y + 1 < GRID_SIZE) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.y += 1;
                if (grid[(int)position.x][(int)position.y] != 0) {
                    death = true;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    moved = true;
                    death = false;
                }
            } else {
                death = true;
            }
                    
        }
        if (IsKeyPressed(KEY_SPACE)) {
            snakeHead += 1;
        }

        // Update game objects
        //----------------------------------------------------------------------------------

        if (moved) {
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    if (grid[i][j] != snakeHead && grid[i][j] != SNAKE_FOOD) {
                        if (grid[i][j] > 0) {
                            grid[i][j] -= 1;
                        }
                    }
                }
            }
        }
        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        // Draw grid
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                DrawRectangleLines(i * SCREEN_WIDTH / GRID_SIZE, j * SCREEN_HEIGHT / GRID_SIZE, SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE, LIGHTGRAY);
            }
        }

        // draw snake
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (grid[i][j] == snakeHead) {
                    DrawRectangle(
                        i * SCREEN_WIDTH / GRID_SIZE, 
                        j * SCREEN_HEIGHT / GRID_SIZE, 
                        SCREEN_WIDTH / GRID_SIZE, 
                        SCREEN_HEIGHT / GRID_SIZE, 
                        death ? RED : GREEN
                    );
                } else if (grid[i][j] > 0) {
                    DrawRectangle(
                        i * SCREEN_WIDTH / GRID_SIZE, 
                        j * SCREEN_HEIGHT / GRID_SIZE, 
                        SCREEN_WIDTH / GRID_SIZE, 
                        SCREEN_HEIGHT / GRID_SIZE, 
                        death ? RED : GREEN
                    );
                }
            }
        }


        EndDrawing();

    }

    return 0;
}


