/*
 * So we have everything we need to get things working.
 * Need to put together all the pieces from all the other
 * prototypes and create the full game.
 *
 * Curious why this is freeking hard for me, but idk.
 */

/*
 * TODO: We don't really know why the movement code is doing what it is doing.
 * Why does a movment interval of 1.0 do what it does?
 * Why does the rect not turn until it is on a grid?  It doesn't seem
 * like anything is setup to do that and yet it seems to.
 *
 * So, what we need, even if it takes a week, is a UI property viewer and way
 * to toggle debug views on/off.  This isn't a waste of time.  Because we will use this
 * for all other damn things.
 *
 * Wait, don't we already have a window in a library somewhere?
 * We do have it, so add some shit to do some debugging, toggle the grid, speed up and
 * slow down the movement parameter, and print out stuff to see what in the world
 * is going on.
 */
#include "raylib.h"
#include <assert.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "style_dark.h"

#define CK_GUI_IMPLEMENTATION
#include "ck_gui.h"

static Vector2 window_position = { 10, 10 };
static Vector2 window_size = { 200, 400 };
static bool minimized = false;
static bool moving = false;
static bool resizing = false;
static Vector2 scroll;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define INPUT_MAP_MAX 100
#define EVENT_MAX 100
#define GRID_SIZE 20

typedef enum input_map_type_t {
    IM_KEYPRESS = 0,
    IM_EVENT_COUNT,
} input_map_type_t;

typedef enum game_event_type_t {
    GE_PLAYER_MOVE_UP = 0,
    GE_PLAYER_MOVE_DOWN,
    GE_PLAYER_MOVE_LEFT,
    GE_PLAYER_MOVE_RIGHT,
    GE_SPAWN_FOOD,
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
    snake_t snake;
    input_map_t inputMap[INPUT_MAP_MAX];
    int inputMapLength;
    game_event_type_t eventList[EVENT_MAX];
    int eventListLength;
} game_state_t;
game_state_t G = {0};

typedef struct debug_state_t {
    bool showGrid;
} debug_state_t;
debug_state_t D = {0};


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
    G.inputMapLength = 0;
    G.inputMap[G.inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_UP,
        .data = KEY_UP,
    };
    G.inputMap[G.inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_DOWN,
        .data = KEY_DOWN,
    };
    G.inputMap[G.inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_LEFT,
        .data = KEY_LEFT,
    };
    G.inputMap[G.inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_RIGHT,
        .data = KEY_RIGHT,
    };
    G.inputMap[G.inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_SPAWN_FOOD,
        .data = KEY_SPACE,
    };
}

void DrawContent(Vector2 position, Vector2 scroll) {
    GuiCheckBox((Rectangle){ position.x + 20 + scroll.x, position.y + 50 + scroll.y, 20, 20 }, "Toggle Grid", &D.showGrid);
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 100 + scroll.y, 100, 25 }, "Button 2");
    // GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 150 + scroll.y, 100, 25 }, "Button 3");
    // GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 200 + scroll.y, 250, 25 }, "A Label");
    // GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 250 + scroll.y, 250, 25 }, "Another Label");
    // GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 300 + scroll.y, 250, 25 }, "Yet Another Label");

}

// Just cut/paste this from snake2, gotta work it in
void spawnFood(void) {
    // // assert(!"TODO");
    // int len = 0;
    // for (int row = 0; row < GRID_SIZE; row++) {
    //     for (int col = 0; col < GRID_SIZE; col++) {
    //         if (G.grid[row][col] == 0) {
    //             G.searchGrid[len] = (Vector2){col, row};
    //             len++;
    //         }
    //     }
    // }
    // int idx = GetRandomValue(0, len - 1);
    // G.grid[(int)G.searchGrid[idx].y][(int)G.searchGrid[idx].x] = SNAKE_FOOD;
}

void processInput(void) {
    for (int i = 0; i < G.inputMapLength; ++i) {
        input_map_t *im = &G.inputMap[i];
        if (im->inputType == IM_KEYPRESS) {
            if (IsKeyPressed(im->data)) {
                G.eventList[G.eventListLength++] = (game_event_type_t){im->eventType};
            }
        }
    }
}

void updateGame(void) {

    // events
    for (int i = 0; i < G.eventListLength; ++i) {
        game_event_type_t ge = G.eventList[i];
        switch (ge) {
            case GE_PLAYER_MOVE_UP: {
                if (G.snake.direction.y == 0) {
                    G.snake.direction = (Vector2){0, -1};
                }
                break;
            }
            case GE_PLAYER_MOVE_DOWN: {
                if (G.snake.direction.y == 0) {
                    G.snake.direction = (Vector2){0, 1};
                }
                break;
            }
            case GE_PLAYER_MOVE_LEFT: {
                if (G.snake.direction.x == 0) {
                    G.snake.direction = (Vector2){-1, 0};
                }
                break;
            }
            case GE_PLAYER_MOVE_RIGHT: {
                if (G.snake.direction.x == 0) {
                    G.snake.direction = (Vector2){1, 0};
                }
                break;
            }
            case GE_SPAWN_FOOD: {
                spawnFood();
                break;
            }
                       
            default: {
                assert(0);
            }
        }
    }

    // Update game objects
    G.snake.previousPosition = G.snake.position;
    G.snake.position.x += G.snake.direction.x;
    G.snake.position.y += G.snake.direction.y;
}

void drawWindow(void) {
    GuiWindowFloating(
        &window_position, &window_size, &minimized, &moving,
        &resizing, &DrawContent, (Vector2) { 140, 320 },
        &scroll, "Movable & Scalable Window"
    );
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

    // Draw grid
    if (D.showGrid) {
        for (int i = 0; i < SCREEN_WIDTH / GRID_SIZE; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / GRID_SIZE; j++) {
                DrawRectangleLines(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE, GRID_SIZE, LIGHTGRAY);
            }
        }
    }

    drawWindow();

    EndDrawing();
}

int main(int argc, char **argv) {

    createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");

    initializeGame();

    double previousTime = GetTime();	
    double currentTime = previousTime;	
    double lag = 0.0;	
    const float moveInterval = 1.001f;

    while (!WindowShouldClose()) {

        double currentTime = GetTime();	
        double elapsed = currentTime - previousTime;	
        previousTime = currentTime;	
        lag += elapsed;

        processInput();

        while (lag >= moveInterval) {
            lag -= moveInterval;
            updateGame();
        }

        renderGame(lag / moveInterval);
            
    }
    return 0;
}
