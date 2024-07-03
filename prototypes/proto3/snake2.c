#include "raylib.h"
#include <assert.h>
// #include "ck_acme.h"

#define GRID_SIZE 8
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_EVENTS 10

// TODO: these could change if grid is way larger
// Not sure what to do
const int SNAKE_FOOD = 99;
const int SNAKE_HEAD = 98;

typedef enum game_event_t {
    GAME_EVENT_NONE,
    GAME_EVENT_PLAYER_UP,
    GAME_EVENT_PLAYER_DOWN,
    GAME_EVENT_PLAYER_LEFT,
    GAME_EVENT_PLAYER_RIGHT,
    GAME_EVENT_SPAWN_FOOD,
    GAME_EVENT_RESET_GAME,
    GAME_EVENT_DEBUG_PRINT,
} game_event_t;

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
    game_event_t event[MAX_EVENTS];
    int eventLength;
    int grid[GRID_SIZE][GRID_SIZE];
    Vector2 searchGrid[GRID_SIZE * GRID_SIZE];
    int input [GRID_SIZE];
    int inputLength;
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
            game_state.searchGrid[row * GRID_SIZE + col] = (Vector2){0,0};
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
    int len = 0;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (game_state.grid[row][col] == 0) {
                game_state.searchGrid[len] = (Vector2){col, row};
                len++;
            }
        }
    }
    int idx = GetRandomValue(0, len - 1);
    game_state.grid[(int)game_state.searchGrid[idx].y][(int)game_state.searchGrid[idx].x] = SNAKE_FOOD;
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

    game_state_t *g = &game_state;
    g->inputLength = 0;

    if (IsKeyPressed(KEY_RIGHT)) {
        g->input[g->inputLength++] = KEY_RIGHT;
        TraceLog(LOG_INFO, "Right key pressed");
    }

    if (IsKeyPressed(KEY_LEFT)) {
        g->input[g->inputLength++] = KEY_LEFT;
        TraceLog(LOG_INFO, "Left key pressed");
    }

    if (IsKeyPressed(KEY_UP)) {
        g->input[g->inputLength++] = KEY_UP;
        TraceLog(LOG_INFO, "Up key pressed");
    }

    if (IsKeyPressed(KEY_DOWN)) {
        g->input[g->inputLength++] = KEY_DOWN;
        TraceLog(LOG_INFO, "Down key pressed");
    }

    if (IsKeyPressed(KEY_SPACE)) {
        g->input[g->inputLength++] = KEY_SPACE;
    }

    if (IsKeyPressed(KEY_R)) {
        g->input[g->inputLength++] = KEY_R;
    }

    if (IsKeyPressed(KEY_P)) {
        g->input[g->inputLength++] = KEY_P;
    }

}

void processInputEvents(void) {
    // this is where we could map any input key to any game event
    game_state.eventLength = 0;

    for (int i = 0; i < game_state.inputLength; i++) {
        switch (game_state.input[i]) {
            case KEY_RIGHT:
                game_state.event[game_state.eventLength++] = GAME_EVENT_PLAYER_RIGHT;
                break;
            case KEY_LEFT:
                game_state.event[game_state.eventLength++] = GAME_EVENT_PLAYER_LEFT;
                break;
            case KEY_UP:
                game_state.event[game_state.eventLength++] = GAME_EVENT_PLAYER_UP;
                break;
            case KEY_DOWN:
                game_state.event[game_state.eventLength++] = GAME_EVENT_PLAYER_DOWN;
                break;
            case KEY_SPACE:
                game_state.event[game_state.eventLength++] = GAME_EVENT_SPAWN_FOOD;
                break;
            case KEY_R:
                game_state.event[game_state.eventLength++] = GAME_EVENT_RESET_GAME;
                break;
            case KEY_P:
                game_state.event[game_state.eventLength++] = GAME_EVENT_DEBUG_PRINT;
                break;
        }
    }
}

void processGameEvents(void) {
    for (int i = 0; i < game_state.eventLength; i++) {
        switch (game_state.event[i]) {
            case GAME_EVENT_PLAYER_RIGHT:
                if (game_state.dir != DIRECTION_LEFT) {
                    game_state.dir = DIRECTION_RIGHT;
                }
                break;
            case GAME_EVENT_PLAYER_LEFT:
                if (game_state.dir != DIRECTION_RIGHT) {
                    game_state.dir = DIRECTION_LEFT;
                }
                break;
            case GAME_EVENT_PLAYER_UP:
                if (game_state.dir != DIRECTION_DOWN) {
                    game_state.dir = DIRECTION_UP;
                }
                break;
            case GAME_EVENT_PLAYER_DOWN:
                if (game_state.dir != DIRECTION_UP) {
                    game_state.dir = DIRECTION_DOWN;
                }
                break;
            case GAME_EVENT_SPAWN_FOOD:
                spawnFood();
                break;
            case GAME_EVENT_RESET_GAME:
                initializeGame();
                break;
            case GAME_EVENT_DEBUG_PRINT:
                printGameState();
                break;
            case GAME_EVENT_NONE:
                break;
        }
    }
}

void updateWorld(void) {

    processInputEvents();

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


