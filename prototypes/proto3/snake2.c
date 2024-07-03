#include "raylib.h"
#include <assert.h>
// #include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

const int SNAKE_FOOD = 99;
const int SNAKE_HEAD = 98;

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
    direction_t prev_dir;
    player_state_t player_state;
    Vector2 position;
    Vector2 previousPosition;
} game_state_t;
game_state_t game_state = {0};

void spawnFood(void);

void initializeGame(void) {
    game_state.snakeBodyLength = 3;
    game_state.dir = DIRECTION_NONE;
    game_state.player_state = PLAYER_ALIVE;
    game_state.position = (Vector2){2, 2};
    game_state.previousPosition = game_state.position;

    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            game_state.grid[row][col] = 0;
        }
    }
    game_state.grid[(int)game_state.position.y][(int)game_state.position.x] = SNAKE_HEAD;
    spawnFood();
}

void printDirection(direction_t dir, direction_t prev) {
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
    switch (prev) {
        case DIRECTION_NONE:
            TraceLog(LOG_INFO, "Previous Direction: DIRECTION_NONE");
            break;
        case DIRECTION_UP:
            TraceLog(LOG_INFO, "Previous Direction: DIRECTION_UP");
            break;
        case DIRECTION_DOWN:
            TraceLog(LOG_INFO, "Previous Direction: DIRECTION_DOWN");
            break;
        case DIRECTION_LEFT:
            TraceLog(LOG_INFO, "Previous Direction: DIRECTION_LEFT");
            break;
        case DIRECTION_RIGHT:
            TraceLog(LOG_INFO, "Previous Direction: DIRECTION_RIGHT");
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
    printDirection(game_state.dir, game_state.prev_dir);
    TraceLog(LOG_INFO, "Player Snake Length: %d", game_state.snakeBodyLength);
}

bool isCellEmpty(Vector2 pos) {
    return game_state.grid[(int)pos.y][(int)pos.x] == 0;
}

bool containsFood(Vector2 pos) {
    return game_state.grid[(int)pos.y][(int)pos.x] == SNAKE_FOOD;
}

void spawnFood(void) {
    // find an empty cell
    // I reall hate this...
    Vector2 pos = {0, 0};
    int tries = 0;
    do {
        pos.x = GetRandomValue(0, GRID_SIZE - 1);
        pos.y = GetRandomValue(0, GRID_SIZE - 1);
        tries++;
        assert(tries < 100 && "Could not find an empty cell to spawn food");
    } while (!isCellEmpty(pos));     
    game_state.grid[(int)pos.y][(int)pos.x] = SNAKE_FOOD;
}

// The next step here is to draw the number value of each grid cell
// in the cell for debugging purposes
void drawDebugGrid() {
    float dx = (int)(SCREEN_WIDTH / GRID_SIZE);
    float dy = (int)(SCREEN_HEIGHT / GRID_SIZE);

    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            // void DrawText(const char *text, int posX, int posY, int fontSize, Color color)
            DrawText(TextFormat("%d", game_state.grid[row][col]), col * dx, row * dy, 48, DARKGREEN);
        }
    }
}

// should we maybe process the input and make a note, but
// don't actually do any logic there.
// updated the world in another function
void handleInput(void) {

    // not sure what is wrong here but ffs why is this so hard?
    // can't move in the opposite direction

    game_state.dir = DIRECTION_NONE;

    if (IsKeyPressed(KEY_RIGHT) && game_state.prev_dir != KEY_LEFT) {
        game_state.dir = DIRECTION_RIGHT;
        TraceLog(LOG_INFO, "Right key pressed");
    }

    if (IsKeyPressed(KEY_LEFT) && game_state.prev_dir != KEY_RIGHT) {
        game_state.dir = DIRECTION_LEFT;
        TraceLog(LOG_INFO, "Left key pressed");
    }

    if (IsKeyPressed(KEY_UP) && game_state.prev_dir != KEY_DOWN) {
        game_state.dir = DIRECTION_UP;
        TraceLog(LOG_INFO, "Up key pressed");
    }

    if (IsKeyPressed(KEY_DOWN) && game_state.prev_dir != KEY_UP) {
        game_state.dir = DIRECTION_DOWN;
        TraceLog(LOG_INFO, "Down key pressed");
    }

    if (IsKeyPressed(KEY_SPACE)) {
        spawnFood();
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

        game_state.prev_dir = game_state.dir;
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

        // game_state.dir = DIRECTION_NONE;
        game_state.grid[(int)game_state.previousPosition.y][(int)game_state.previousPosition.x] = game_state.snakeBodyLength;
        if (containsFood(game_state.position)) {
            game_state.player_state = PLAYER_CHOMP;
        }
        game_state.grid[(int)game_state.position.y][(int)game_state.position.x] = SNAKE_HEAD;

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
        game_state.snakeBodyLength += 1;
        spawnFood();
        game_state.player_state = PLAYER_ALIVE;
        // we need to update the grid
    }
}

void drawGame(void) {
    // Draw grid
    int dx = SCREEN_WIDTH / GRID_SIZE;
    int dy = SCREEN_HEIGHT / GRID_SIZE;

    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            DrawRectangleLines(col * dx, row * dy, dx, dy, LIGHTGRAY);
        }
    }

    // draw snake
    Color snakeColor = game_state.player_state == PLAYER_DEAD ? RED : GREEN;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (game_state.grid[row][col] == SNAKE_HEAD) {
                DrawRectangle(col * dx, row * dy, dx, dy, snakeColor);
            } 
            else if (game_state.grid[row][col] > 0) {
                DrawRectangle(col * dx, row * dy, dx, dy, snakeColor);
            }
        }
    }

    // draw food
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (game_state.grid[row][col] == SNAKE_FOOD) {
                DrawRectangle(col * dx, row * dy, dx, dy, YELLOW);
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


