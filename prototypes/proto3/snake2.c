#include "raylib.h"
#include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const int SNAKE_FOOD = 43;

typedef enum player_state_t {
    PLAYER_ALIVE,
    PLAYER_CHOMP,
    PLAYER_DEAD,
} player_state_t;
player_state_t player_state = PLAYER_ALIVE;

Vector2 position = {0, 0};

/* 
 *
 * How do we even test this thing?
 * snake moves, but it's body follows, and grows.
 * Interesting problem to solve.
 */

// need to randomly spawn food
// make sure that food doesn't spawn on the snake
void spawnFood(int grid[GRID_SIZE][GRID_SIZE], int w, int h) {
    int x = GetRandomValue(0, w - 1);
    int y = GetRandomValue(0, h - 1);
    if (grid[y][x] == 0) {
        TraceLog(LOG_INFO, "Spawning food at %d, %d", x, y);
        grid[y][x] = SNAKE_FOOD;
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

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

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
                    player_state = PLAYER_DEAD;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    player_state = PLAYER_ALIVE;
                }
            } else {
                player_state = PLAYER_DEAD;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (position.x - 1 >= 0) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.x -= 1;
                if (grid[(int)position.x][(int)position.y] != 0) {
                    player_state = PLAYER_DEAD;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    player_state = PLAYER_ALIVE;
                }
            } else {
                player_state = PLAYER_DEAD;
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            if (position.y - 1 >= 0) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.y -= 1;
                if (grid[(int)position.x][(int)position.y] != 0 && grid[(int)position.x][(int)position.y] != SNAKE_FOOD) {
                    player_state = PLAYER_DEAD;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    player_state = PLAYER_ALIVE;
                }
            } else {
                player_state = PLAYER_DEAD;
            }

        }

        if (IsKeyPressed(KEY_DOWN)) {
            if (position.y + 1 < GRID_SIZE) {
                grid[(int)position.x][(int)position.y] -= 1;
                position.y += 1;
                if (grid[(int)position.x][(int)position.y] != 0 && grid[(int)position.x][(int)position.y] != SNAKE_FOOD) {
                    player_state = PLAYER_DEAD;
                } else {
                    grid[(int)position.x][(int)position.y] = snakeHead;
                    player_state = PLAYER_ALIVE;
                    player_state = PLAYER_DEAD;
                }
            } else {
                player_state = PLAYER_DEAD;
            }
                    
        }

        if (IsKeyPressed(KEY_SPACE)) {
            spawnFood(grid, GRID_SIZE, GRID_SIZE);
        }

        // Update game objects
        //----------------------------------------------------------------------------------

        if (player_state == PLAYER_ALIVE) {
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
                        player_state == PLAYER_DEAD ? RED : GREEN
                    );
                } else if (grid[i][j] > 0) {
                    DrawRectangle(
                        i * SCREEN_WIDTH / GRID_SIZE, 
                        j * SCREEN_HEIGHT / GRID_SIZE, 
                        SCREEN_WIDTH / GRID_SIZE, 
                        SCREEN_HEIGHT / GRID_SIZE, 
                        player_state == PLAYER_DEAD ? RED : GREEN
                    );
                }
            }
        }

        // draw food
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == SNAKE_FOOD) {
                DrawRectangle(
                    i * SCREEN_WIDTH / GRID_SIZE, 
                    j * SCREEN_HEIGHT / GRID_SIZE, 
                    SCREEN_WIDTH / GRID_SIZE, 
                    SCREEN_HEIGHT / GRID_SIZE, 
                    YELLOW
                );
            }
            }
        }

        EndDrawing();

    }

    return 0;
}


