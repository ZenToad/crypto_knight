/*
 * So we have everything we need to get things working.
 * Need to put together all the pieces from all the other
 * prototypes and create the full game.
 *
 * Curious why this is freeking hard for me, but idk.
 */

#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define INPUT_MAP_MAX 100
#define EVENT_MAX 100
#define GRID_SIZE 20

typedef enum input_map_type_t {
    IM_KEYPRESS = 0,
    IM_EVENT_COUNT,
} input_map_type_t;

typedef enum player_direction_t {
    PLAYER_DIR_UP = 0,
    PLAYER_DIR_DOWN,
    PLAYER_DIR_RIGHT,
    PLAYER_DIR_LEFT,
} player_direction_t;

typedef enum game_event_type_t {
    GE_PLAYER_MOVE_UP = 0,
    GE_PLAYER_MOVE_DOWN,
    GE_PLAYER_MOVE_LEFT,
    GE_PLAYER_MOVE_RIGHT,
    GE_EVENT_COUNT,
} game_event_type_t;

typedef struct input_map_t {
    input_map_type_t inputType;
    game_event_type_t eventType;
    int data; 
} input_map_t;

typedef struct snake_t {
    Vector2 position;
    Vector2 direction;
    Vector2 previousPosition;
} snake_t;

typedef struct game_state_t {
    player_direction_t currDir; 
    snake_t snake;
} game_state_t;
game_state_t G = {0};

input_map_t inputMap[INPUT_MAP_MAX];
int inputMapLength = 0;
game_event_type_t eventList[EVENT_MAX];
int eventListLength = 0;

void processInput(void) {
    for (int i = 0; i < inputMapLength; ++i) {
        input_map_t *im = &inputMap[i];
        if (im->inputType == IM_KEYPRESS) {
            if (IsKeyPressed(im->data)) {
                eventList[eventListLength++] = (game_event_type_t){im->eventType};
            }
        }
    }
}

void createWindow(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    InitAudioDevice();
}

void initializeGame(void) {

    G.snake.position = (Vector2){10, 10};
    G.snake.previousPosition = (Vector2){10, 10};
    G.snake.direction = (Vector2){1, 0};
    // Setup input map
    inputMapLength = 0;
    inputMap[inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_UP,
        .data = KEY_UP,
    };
    inputMap[inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_DOWN,
        .data = KEY_DOWN,
    };
    inputMap[inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_LEFT,
        .data = KEY_LEFT,
    };
    inputMap[inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_RIGHT,
        .data = KEY_RIGHT,
    };
}

void handleInput(void) {
}

void updateGame(void) {
    // Update game objects
    G.snake.previousPosition = G.snake.position;
    G.snake.position.x += G.snake.direction.x;
    G.snake.position.y += G.snake.direction.y;
    if (G.snake.position.x < 0) {
        G.snake.direction.x = G.snake.direction.x * -1;
    }
    if (G.snake.position.y < 0) {
        G.snake.direction.y = G.snake.direction.y * -1;
    }
    if (G.snake.position.x >= (int)(SCREEN_WIDTH / GRID_SIZE)) {
        G.snake.direction.x = G.snake.direction.x * -1;
    }
    if (G.snake.position.y >= (int)(SCREEN_HEIGHT / GRID_SIZE)) {
        G.snake.direction.y = G.snake.direction.y * -1;
    }
}

void renderGame(double alpha) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    Vector2 interpolatedPosition = {	
        .x = G.snake.previousPosition.x * (1.0f - alpha) + G.snake.position.x * alpha,	
        .y = G.snake.previousPosition.y * (1.0f - alpha) + G.snake.position.y * alpha	
    };
    // Draw snake
    DrawRectangle(interpolatedPosition.x * GRID_SIZE, interpolatedPosition.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, DARKGREEN);

    EndDrawing();
}

int main(int argc, char **argv) {

    createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");

    initializeGame();

    double previousTime = GetTime();	
    double currentTime = previousTime;	
    double lag = 0.0;	
    const float moveInterval = 0.1f;

    while (!WindowShouldClose()) {

        double currentTime = GetTime();	
        double elapsed = currentTime - previousTime;	
        previousTime = currentTime;	
        lag += elapsed;

        handleInput();

        while (lag >= moveInterval) {
            lag -= moveInterval;
            updateGame();
        }

        renderGame(lag / moveInterval);
            
    }
    return 0;
}
