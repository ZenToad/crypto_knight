#include "raylib.h"
#include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const int SNAKE_FOOD = 43;
const int SNAKE_HEAD = 42;

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
    int snakeBodyLength;
    direction_t dir;
    player_state_t player_state;
    Vector2 position;
    Vector2 previousPosition;
    bool spawnFood;
} game_state_t;
game_state_t game_state = {0};

void spawnFood(void);

void initializeGame(void) {
    game_state.snakeBodyLength = 3;
    game_state.dir = DIRECTION_NONE;
    game_state.player_state = PLAYER_ALIVE;
    game_state.position = (Vector2){2, 2};
    game_state.previousPosition = game_state.position;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            game_state.grid[i][j] = 0;
        }
    }
    game_state.grid[(int)game_state.position.x][(int)game_state.position.y] = SNAKE_HEAD;
    spawnFood();
}

void printDirection(direction_t dir) {
    switch (dir) {
        case DIRECTION_NONE:
            TraceLog(LOG_INFO, "Player Direction: DIRECTION_NONE");
            break;
        case DIRECTION_UP:
            TraceLog(LOG_INFO, "Player Direction: DIRECTION_UP");
            break;
        case DIRECTION_DOWN:
            TraceLog(LOG_INFO, "Player Direction: DIRECTION_DOWN");
            break;
        case DIRECTION_LEFT:
            TraceLog(LOG_INFO, "Player Direction: DIRECTION_LEFT");
            break;
        case DIRECTION_RIGHT:
            TraceLog(LOG_INFO, "Player Direction: DIRECTION_RIGHT");
            break;
    }
}

void printPlayerState(player_state_t state) {
    switch (state) {
        case PLAYER_ALIVE:
            TraceLog(LOG_INFO, "Player State: PLAYER_ALIVE");
            break;
        case PLAYER_CHOMP:
            TraceLog(LOG_INFO, "Player State: PLAYER_CHOMP");
            break;
        case PLAYER_DEAD:
            TraceLog(LOG_INFO, "Player State: PLAYER_DEAD");
            break;
    }
}

void printGameState(void) {
    printPlayerState(game_state.player_state);
    TraceLog(LOG_INFO, "Player Position: %d, %d", (int)game_state.position.x, (int)game_state.position.y);
    TraceLog(LOG_INFO, "Player Previous Position: %d, %d", (int)game_state.previousPosition.x, (int)game_state.previousPosition.y);
    printDirection(game_state.dir);
    TraceLog(LOG_INFO, "Player Snake Length: %d", game_state.snakeBodyLength);
    TraceLog(LOG_INFO, "Player Spawn Food: %d", game_state.spawnFood);
}

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
        TraceLog(LOG_INFO, "Right key pressed");
    }

    if (IsKeyPressed(KEY_LEFT)) {
        game_state.dir = DIRECTION_LEFT;
        TraceLog(LOG_INFO, "Left key pressed");
    }

    if (IsKeyPressed(KEY_UP)) {
        game_state.dir = DIRECTION_UP;
        TraceLog(LOG_INFO, "Up key pressed");
    }

    if (IsKeyPressed(KEY_DOWN)) {
        game_state.dir = DIRECTION_DOWN;
        TraceLog(LOG_INFO, "Down key pressed");
    }

    if (IsKeyPressed(KEY_SPACE)) {
        game_state.spawnFood = true;
    }

    if (IsKeyPressed(KEY_R)) {
        initializeGame();
    }

    if (IsKeyPressed(KEY_P)) {
        printGameState();
    }

}

void updateWorld(void) {
    // if the snake is alive, we need to move the snake
    // and update the grid
    if (game_state.player_state == PLAYER_ALIVE && game_state.dir != DIRECTION_NONE) {

        game_state.previousPosition = game_state.position;

        // we need to move the snake
        if (game_state.dir == DIRECTION_RIGHT) {
            game_state.position.x += 1;
        }
        if (game_state.dir == DIRECTION_LEFT) {
            game_state.position.x -= 1;
        }
        if (game_state.dir == DIRECTION_UP) {
            game_state.position.y -= 1;
        }
        if (game_state.dir == DIRECTION_DOWN) {
            game_state.position.y += 1;
        }

        game_state.dir = DIRECTION_NONE;
        game_state.grid[(int)game_state.previousPosition.x][(int)game_state.previousPosition.y] = game_state.snakeBodyLength;
        game_state.grid[(int)game_state.position.x][(int)game_state.position.y] = SNAKE_HEAD;
        // we need to update the grid
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                if (game_state.position.x == col && game_state.position.y == row) {
                    continue;
                }
                if (game_state.grid[row][col] != SNAKE_FOOD && game_state.grid[row][col] != SNAKE_HEAD) {
                    if (game_state.grid[row][col] > 0) {
                        game_state.grid[row][col] -= 1;
                    }
                }
            }
        }

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
            if (game_state.grid[i][j] == SNAKE_HEAD) {
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


