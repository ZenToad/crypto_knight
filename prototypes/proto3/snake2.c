#include "raylib.h"
#include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const int SNAKE_FOOD = 43;

typedef enum direction_t {
    DIRECTION_NONE  = 0x0,
    DIRECTION_UP    = 0x1,
    DIRECTION_DOWN  = 0x2,
    DIRECTION_LEFT  = 0x4,
    DIRECTION_RIGHT = 0x8,
} direction_t;

typedef enum player_state_t {
    PLAYER_ALIVE,
    PLAYER_CHOMP,
    PLAYER_DEAD,
} player_state_t;

typedef struct game_state_t {
    int grid[GRID_SIZE][GRID_SIZE];
    int snakeHead;
    direction_t dir;
    player_state_t player_state;
    Vector2 position;
    bool spawnFood;
} game_state_t;
game_state_t game_state = {0};


bool isCellEmpty(int x, int y) {
    return game_state.grid[x][y] == 0;
}

// need to randomly spawn food
// make sure that food doesn't spawn on the snake
void spawnFood(void) {
    int x = GetRandomValue(0, GRID_SIZE - 1);
    int y = GetRandomValue(0, GRID_SIZE - 1);
    if (isCellEmpty(x, y)) {
        TraceLog(LOG_INFO, "Spawning food at %d, %d", x, y);
        game_state.grid[y][x] = SNAKE_FOOD;
    } else {
        spawnFood();
    }
}

// The next step here is to draw the number value of each grid cell
// in the cell for debugging purposes
void drawDebugGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // void DrawText(const char *text, int posX, int posY, int fontSize, Color color)
            DrawText(TextFormat("%d", game_state.grid[i][j]), i * SCREEN_WIDTH / GRID_SIZE, j * SCREEN_HEIGHT / GRID_SIZE, 48, DARKGREEN);
        }
    }
}

// should we maybe process the input and make a note, but
// don't actually do any logic there.
// updated the world in another function
void handleInput(void) {

    game_state.dir = DIRECTION_NONE;
    game_state.spawnFood = false;

    if (IsKeyPressed(KEY_RIGHT)) {
        game_state.dir = DIRECTION_RIGHT;
    }

    if (IsKeyPressed(KEY_LEFT)) {
        game_state.dir = DIRECTION_LEFT;
    }

    if (IsKeyPressed(KEY_UP)) {
        game_state.dir = DIRECTION_UP;
    }

    if (IsKeyPressed(KEY_DOWN)) {
        game_state.dir = DIRECTION_DOWN;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        game_state.spawnFood = true;
    }

}

void updateWorld(void) {
    // if the snake is alive, we need to move the snake
    // and update the grid
    if (game_state.player_state == PLAYER_ALIVE) {
        // we need to move the snake
        // we need to update the grid
        // we need to check if the snake is dead
        // we need to check if the snake has eaten
        // we need to check if the snake has hit the wall
    }
    // if the snake is dead, we need to reset the game
    if (game_state.player_state == PLAYER_DEAD) {
        // we need to reset the game
    }
    // if the snake has eaten, we need to grow the snake
    if (game_state.player_state == PLAYER_CHOMP) {
        // we need to grow the snake
        // we need to update the grid
    }
}

void initializeGame(void) {
    game_state.snakeHead = 2;
    game_state.dir = DIRECTION_NONE;
    game_state.player_state = PLAYER_ALIVE;
    game_state.position.x = 2;
    game_state.position.y = 2;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            game_state.grid[i][j] = 0;
        }
    }
    game_state.grid[(int)game_state.position.x][(int)game_state.position.y] = game_state.snakeHead;
    spawnFood();
}

// void archiveUpdate(void) {
//     if (IsKeyPressed(KEY_RIGHT)) {
//         if (position.x + 1 < GRID_SIZE) {
//             grid[(int)position.x][(int)position.y] -= 1;
//             position.x += 1;
//             if (grid[(int)position.x][(int)position.y] != 0) {
//                 player_state = PLAYER_DEAD;
//             } else {
//                 snakeHead += 1;
//                 grid[(int)position.x][(int)position.y] = snakeHead;
//                 player_state = PLAYER_ALIVE;
//             }
//         } else {
//             player_state = PLAYER_DEAD;
//         }
//     }
//
//     if (IsKeyPressed(KEY_LEFT)) {
//         if (position.x - 1 >= 0) {
//             grid[(int)position.x][(int)position.y] -= 1;
//             position.x -= 1;
//             if (grid[(int)position.x][(int)position.y] != 0) {
//                 player_state = PLAYER_DEAD;
//             } else {
//                 snakeHead += 1;
//                 grid[(int)position.x][(int)position.y] = snakeHead;
//                 player_state = PLAYER_ALIVE;
//             }
//         } else {
//             player_state = PLAYER_DEAD;
//         }
//     }
//
//     if (IsKeyPressed(KEY_UP)) {
//         if (position.y - 1 >= 0) {
//             grid[(int)position.x][(int)position.y] -= 1;
//             position.y -= 1;
//             if (grid[(int)position.x][(int)position.y] != 0 && grid[(int)position.x][(int)position.y] != SNAKE_FOOD) {
//                 player_state = PLAYER_DEAD;
//             } else {
//                 snakeHead+= 1;
//                 grid[(int)position.x][(int)position.y] = snakeHead;
//                 player_state = PLAYER_ALIVE;
//             }
//         } else {
//             player_state = PLAYER_DEAD;
//         }
//
//     }
//
//     if (IsKeyPressed(KEY_DOWN)) {
//         if (position.y + 1 < GRID_SIZE) {
//             grid[(int)position.x][(int)position.y] -= 1;
//             position.y += 1;
//             if (grid[(int)position.x][(int)position.y] != 0 && grid[(int)position.x][(int)position.y] != SNAKE_FOOD) {
//                 player_state = PLAYER_DEAD;
//             } else {
//                 snakeHead += 1;
//                 grid[(int)position.x][(int)position.y] = snakeHead;
//                 player_state = PLAYER_ALIVE;
//             }
//         } else {
//             player_state = PLAYER_DEAD;
//         }
//
//     }
//
//     if (IsKeyPressed(KEY_SPACE)) {
//         spawnFood(grid, GRID_SIZE, GRID_SIZE);
//     }
//
// }

void drawGame(void) {
    // Draw grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            DrawRectangleLines(i * SCREEN_WIDTH / GRID_SIZE, j * SCREEN_HEIGHT / GRID_SIZE, SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE, LIGHTGRAY);
        }
    }

    // draw snake
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (game_state.grid[i][j] == game_state.snakeHead) {
                DrawRectangle(
                        i * SCREEN_WIDTH / GRID_SIZE, 
                        j * SCREEN_HEIGHT / GRID_SIZE, 
                        SCREEN_WIDTH / GRID_SIZE, 
                        SCREEN_HEIGHT / GRID_SIZE, 
                        game_state.player_state == PLAYER_DEAD ? RED : GREEN
                        );
            } else if (game_state.grid[i][j] > 0) {
                DrawRectangle(
                        i * SCREEN_WIDTH / GRID_SIZE, 
                        j * SCREEN_HEIGHT / GRID_SIZE, 
                        SCREEN_WIDTH / GRID_SIZE, 
                        SCREEN_HEIGHT / GRID_SIZE, 
                        game_state.player_state == PLAYER_DEAD ? RED : GREEN
                        );
            }
        }
    }



    // draw food
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (game_state.grid[i][j] == SNAKE_FOOD) {
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

}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    InitAudioDevice();

    initializeGame();

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // Process Input
        //----------------------------------------------------------------------------------
        handleInput();

        // Update game objects
        //----------------------------------------------------------------------------------
        updateWorld();

        // if (player_state == PLAYER_ALIVE) {
        //     for (int i = 0; i < GRID_SIZE; i++) {
        //         for (int j = 0; j < GRID_SIZE; j++) {
        //             if (grid[i][j] != snakeHead && grid[i][j] != SNAKE_FOOD) {
        //                 if (grid[i][j] > 0) {
        //                     grid[i][j] -= 1;
        //                 }
        //             }
        //         }
        //     }
        // }
        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        drawGame();

        drawDebugGrid();

        EndDrawing();

    }

    return 0;
}


