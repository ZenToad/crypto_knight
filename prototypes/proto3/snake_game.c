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

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

#include "style_terminal.h"

#define CK_GUI_IMPLEMENTATION
#include "ck_gui.h"

static Vector2 window_position = { 10, 10 };
static Vector2 window_size = { 200, 400 };
static bool minimized = false;
static bool moving = false;
static bool resizing = false;
static Vector2 scroll;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define INPUT_MAP_MAX 100
#define EVENT_MAX 100
#define GRID_SIZE 8

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

typedef struct game_loop_t {
    float fixedTimeStep;
    double previousTime;	
    double currentTime;	
    double accumulator;
    double time;
    double elapsed;
    double lag;
} game_loop_t;

typedef struct snake_t {
    Vector2 position;
    Vector2 direction;
    Vector2 previousPosition;
    Vector2 currGrid;
    Vector2 prevGrid;
} snake_t;

typedef struct game_state_t {
    snake_t snake;
    game_loop_t loop;
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


void createWindow(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    InitAudioDevice();
    GuiLoadStyleTerminal();
}

void initializeGame(void) {

    G.loop.fixedTimeStep = 1.0f;
    G.snake.position = (Vector2){0, 1};
    G.snake.previousPosition = (Vector2){0, 1};
    G.snake.currGrid = (Vector2){0, 1};
    G.snake.prevGrid = (Vector2){0, 1};
    G.snake.direction = (Vector2){1, 0};

    D.showGrid = true;

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

void resetGame(void) {
    G.snake.position = (Vector2){0, 1};
    G.snake.previousPosition = (Vector2){0, 1};
    G.snake.direction = (Vector2){1, 0};
}

void DrawContent(Vector2 position, Vector2 scroll) {

    int padding = 10;
    Rectangle rect = {0};
    rect.x = position.x + 20 + scroll.x;
    rect.y = position.y + 32 + scroll.y;
    rect.width = 150;
    rect.height = 20;

    if (GuiButton(rect, "Reset")) {
        resetGame();
    }
    rect.y += rect.height + padding;

    rect.width = 20;
    rect.height = 20;
    GuiCheckBox(rect, "Toggle Grid", &D.showGrid);
    rect.y += rect.height + padding;

    rect.width = 100; rect.height = 25;
    GuiButton(rect, "Button 2");
    rect.y += rect.height + padding;

    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Window Pos: (%.0f, %.0f)", position.x, position.y));
    rect.y += rect.height + padding;

    // Slider to control the speed
// int GuiSliderPro(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue, int sliderWidth)
    rect.width = 100, rect.height = 20;
    GuiSliderPro(rect, "", "", &G.loop.fixedTimeStep, 0.1f, 1.0, 10);
    rect.y += rect.height + padding;

    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Lag: %.2f", G.loop.lag));
    rect.y += rect.height + padding;

    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Elapsed: %.2f", G.loop.elapsed));
    rect.y += rect.height + padding;

    // print out snake data
    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Snake Pos: (%.0f, %.0f)", G.snake.position.x, G.snake.position.y));
    rect.y += rect.height + padding;

    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Snake Dir: (%.0f, %.0f)", G.snake.direction.x, G.snake.direction.y));
    rect.y += rect.height + padding;

    rect.width = 200, rect.height = 20;
    GuiLabel(rect, TextFormat("Snake Prev Pos: (%.0f, %.0f)", G.snake.previousPosition.x, G.snake.previousPosition.y));
    rect.y += rect.height + padding;

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
    // use the continuous movement code from movement.c
    // process events after movement?  We could also
    // delay a frame, not sure.  Guess just see what feels better.
    double newTime = GetTime();
    double frameTime = newTime - G.loop.currentTime;
    if (frameTime > G.loop.fixedTimeStep) {
        frameTime = G.loop.fixedTimeStep;
    }
    G.loop.currentTime = newTime;
    G.loop.accumulator += frameTime;

    while (G.loop.accumulator >= G.loop.fixedTimeStep) {
        G.snake.previousPosition = G.snake.position;
        G.snake.position.x += G.snake.direction.x;
        G.snake.position.y += G.snake.direction.y;
        G.loop.accumulator -= G.loop.fixedTimeStep;
        if (G.snake.position.x <= 0) {
            G.snake.direction.x *= -1;
            G.snake.position.x = 0;
        } else if (G.snake.position.x >= GRID_SIZE - 1) {
            G.snake.direction.x *= -1;
            G.snake.position.x = GRID_SIZE - 1;
        }

        // update the grid position
        G.snake.prevGrid = G.snake.currGrid;
        G.snake.currGrid = (Vector2) {floor(G.snake.position.x), floor(G.snake.position.y)};
        // see if the player has moved to a new grid square
        if (!(G.snake.currGrid.x == G.snake.prevGrid.x && 
              G.snake.currGrid.y == G.snake.prevGrid.y)) {
            TraceLog(LOG_INFO, "Player has moved to a new grid square: %f, %f", 
                G.snake.currGrid.x, G.snake.currGrid.y);
            // this is where we would check for a movement event and set the direction.
            // if there isn't already an input event it will have to wait until the next square
        }
            
    }

    // // Update game objects
    // G.snake.previousPosition = G.snake.position;
    // G.snake.position.x += G.snake.direction.x;
    // G.snake.position.y += G.snake.direction.y;
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
    ClearBackground(BLACK);

    DrawRectangle(G.snake.position.x * SCREEN_WIDTH / GRID_SIZE, G.snake.position.y * SCREEN_HEIGHT / GRID_SIZE, SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE, BLUE);
       
    // double alpha = G.loop.accumulator / G.fixedTimeStep
    Vector2 pos = Vector2Lerp(G.snake.previousPosition, G.snake.position, alpha);
    int square_size = SCREEN_WIDTH / GRID_SIZE;
    DrawRectangle(pos.x * square_size , pos.y * square_size, square_size, square_size, RED);

    // Draw grid
    if (D.showGrid) {
        drawGrid();
    }

    drawWindow();

    EndDrawing();
}

int main(int argc, char **argv) {

    createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");

    initializeGame();

    G.loop.previousTime = GetTime();	
    G.loop.currentTime = G.loop.previousTime;	
    G.loop.elapsed = G.loop.currentTime - G.loop.previousTime;	
    G.loop.lag = 0.0;	

    while (!WindowShouldClose()) {

        double newTime = GetTime();
        double frameTime = newTime - G.loop.currentTime;
        if (frameTime > G.loop.fixedTimeStep) {
            frameTime = G.loop.fixedTimeStep;
        }
        G.loop.time = newTime;

        processInput();

        updateGame();

        renderGame(G.loop.lag / G.loop.fixedTimeStep);
            
    }
    return 0;
}
