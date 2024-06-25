/*
* snake.c
* This is a simple snake game. The snake moves around the screen and eats
* food. The snake grows longer as it eats food. The game is over when the
* snake runs into the wall or into itself.
 */

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
// #include "../../styles/dark/style_dark.h"
// #include "../../styles/amber/style_amber.h"
// #include "../../styles/bluish/style_bluish.h"
// #include "../../styles/candy/style_candy.h"
// #include "../../styles/cherry/style_cherry.h"
// #include "../../styles/cyber/style_cyber.h"
// #include "../../styles/enefete/style_enefete.h"
// #include "../../styles/jungle/style_jungle.h"
// #include "../../styles/lavanda/style_lavanda.h"
// #include "../../styles/sunny/style_sunny.h"
#include "../../styles/terminal/style_terminal.h"

#define GRID_SIZE 32
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

typedef struct Snake {
    Vector2 position;
    Vector2 direction;
    Vector2 previousPosition;
} Snake;

enum GameState {
    GAME_ATTRACT,
    GAME_GET_READY,
    GAME_OVER, 
};

enum SoundType {
    MAX_SOUNDS,
};

typedef struct game_t {
    enum GameState state;
    Sound sounds[MAX_SOUNDS];
} game_t;
game_t game;

void initGameState(int screenWidth, int screenHeight) {

}

void MoveSnake(Snake *snake) {
    snake->previousPosition = snake->position;
    snake->position.x += snake->direction.x;
    snake->position.y += snake->direction.y;
}


static void DrawContent(Vector2 position, Vector2 scroll) {
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 50  + scroll.y, 100, 25 }, "Button 1");
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 100 + scroll.y, 100, 25 }, "Button 2");
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 150 + scroll.y, 100, 25 }, "Button 3");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 200 + scroll.y, 250, 25 }, "A Label");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 250 + scroll.y, 250, 25 }, "Another Label");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 300 + scroll.y, 250, 25 }, "Yet Another Label");
}

joid GuiWindowFloating(Vector2 *position, Vector2 *size, bool *minimized, bool *moving, bool *resizing, void (*draw_content)(Vector2, Vector2), Vector2 content_size, Vector2 *scroll, const char* title) {
    #if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
        #define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24
    #endif

    #if !defined(RAYGUI_WINDOW_CLOSEBUTTON_SIZE)
        #define RAYGUI_WINDOW_CLOSEBUTTON_SIZE 18
    #endif

    int close_title_size_delta_half = (RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_WINDOW_CLOSEBUTTON_SIZE) / 2;

    // window movement and resize input and collision check
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !*moving && !*resizing) {
        Vector2 mouse_position = GetMousePosition();

        Rectangle title_collision_rect = { position->x, position->y, size->x - (RAYGUI_WINDOW_CLOSEBUTTON_SIZE + close_title_size_delta_half), RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT };
        Rectangle resize_collision_rect = { position->x + size->x - 20, position->y + size->y - 20, 20, 20 };

        if(CheckCollisionPointRec(mouse_position, title_collision_rect)) {
            *moving = true;
        } else if(!*minimized && CheckCollisionPointRec(mouse_position, resize_collision_rect)) {
            *resizing = true;
        }
    }

    // window movement and resize update
    if(*moving) {
        Vector2 mouse_delta = GetMouseDelta();
        position->x += mouse_delta.x;
        position->y += mouse_delta.y;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            *moving = false;

            // clamp window position keep it inside the application area
            if(position->x < 0) position->x = 0;
            else if(position->x > GetScreenWidth() - size->x) position->x = GetScreenWidth() - size->x;
            if(position->y < 0) position->y = 0;
            else if(position->y > GetScreenHeight()) position->y = GetScreenHeight() - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT;
        }

    } else if(*resizing) {
        Vector2 mouse = GetMousePosition();
        if (mouse.x > position->x)
            size->x = mouse.x - position->x;
        if (mouse.y > position->y)
            size->y = mouse.y - position->y;

        // clamp window size to an arbitrary minimum value and the window size as the maximum
        if(size->x < 100) size->x = 100;
        else if(size->x > GetScreenWidth()) size->x = GetScreenWidth();
        if(size->y < 100) size->y = 100;
        else if(size->y > GetScreenHeight()) size->y = GetScreenHeight();

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            *resizing = false;
        }
    }

    // window and content drawing with scissor and scroll area
    if(*minimized) {
        GuiStatusBar((Rectangle){ position->x, position->y, size->x, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT }, title);

        if (GuiButton((Rectangle){ position->x + size->x - RAYGUI_WINDOW_CLOSEBUTTON_SIZE - close_title_size_delta_half,
                                   position->y + close_title_size_delta_half,
                                   RAYGUI_WINDOW_CLOSEBUTTON_SIZE,
                                   RAYGUI_WINDOW_CLOSEBUTTON_SIZE },
                                   "#120#")) {
            *minimized = false;
        }

    } else {
        *minimized = GuiWindowBox((Rectangle) { position->x, position->y, size->x, size->y }, title);

        // scissor and draw content within a scroll panel
        if(draw_content != NULL) {
            Rectangle scissor = { 0 };
            GuiScrollPanel((Rectangle) { position->x, position->y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT, size->x, size->y - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT },
                                         NULL,
                                         (Rectangle) { position->x, position->y, content_size.x, content_size.y },
                                         scroll,
                                         &scissor);

            bool require_scissor = size->x < content_size.x || size->y < content_size.y;

            if(require_scissor) {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            draw_content(*position, *scroll);

            if(require_scissor) {
                EndScissorMode();
            }
        }

        // draw the resize button/icon
        GuiDrawIcon(71, position->x + size->x - 20, position->y + size->y - 20, 1, WHITE);
    }
}
int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Scaled Resolution");
    // SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // GuiLoadStyleDark();
    // GuiLoadStyleAmber();
    // GuiLoadStyleBluish();
    // GuiLoadStyleCandy();
    // GuiLoadStyleCherry();
    // GuiLoadStyleCyber();
    // GuiLoadStyleEnefete();
    // GuiLoadStyleJungle();
    // GuiLoadStyleLavanda();
    // GuiLoadStyleSunny();
    GuiLoadStyleTerminal();
    InitAudioDevice();

    //--------------------------------------------------------------------------------------
    // Load game assets
    // game.sounds[COLLISION] = LoadSound("../prototypes/proto2/collision.wav");

    //--------------------------------------------------------------------------------------
    // Initialize game variables

    // Initialize snake
    Snake snake = { .position = {10, 10}, .direction = {1, 0} };
    snake.previousPosition = snake.position;
    float moveTimer = 0.0f;
    const float moveInterval = 0.5f;


    initGameState(SCREEN_WIDTH, SCREEN_HEIGHT);

    double previousTime = GetTime();
    double currentTime = previousTime;
    double lag = 0.0;

    bool windowOpen = true;
    Rectangle windowBounds = { 200, 200, 300, 200 };

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
                                     
        double currentTime = GetTime();
        double elapsed = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsed;

        // Process Input
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_RIGHT) && snake.direction.x == 0) snake.direction = (Vector2){1, 0};
        if (IsKeyPressed(KEY_LEFT) && snake.direction.x == 0) snake.direction = (Vector2){-1, 0};
        if (IsKeyPressed(KEY_UP) && snake.direction.y == 0) snake.direction = (Vector2){0, -1};
        if (IsKeyPressed(KEY_DOWN) && snake.direction.y == 0) snake.direction = (Vector2){0, 1};
        
        // Update game objects
        //----------------------------------------------------------------------------------

        while (lag >= moveInterval) {
            MoveSnake(&snake);
            lag -= moveInterval;
        }

        // Interpolation
        float alpha = lag / moveInterval;
        Vector2 interpolatedPosition = {
            .x = snake.previousPosition.x * (1.0f - alpha) + snake.position.x * alpha,
            .y = snake.previousPosition.y * (1.0f - alpha) + snake.position.y * alpha
        };

        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
        // ClearBackground(RAYWHITE);

        // BeginMode2D(camera);

        // Draw snake
        DrawRectangle(interpolatedPosition.x * GRID_SIZE, interpolatedPosition.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, DARKGREEN);

        // Draw grid
        for (int i = 0; i < SCREEN_WIDTH / GRID_SIZE; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / GRID_SIZE; j++) {
                DrawRectangleLines(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE, GRID_SIZE, LIGHTGRAY);
            }
        }

        GuiWindowFloating(&window_position, &window_size, &minimized, &moving, &resizing, &DrawContent, (Vector2) { 140, 320 }, &scroll, "Movable & Scalable Window");

        // if (windowOpen) {
        //     GuiWindowBox(windowBounds, "Floating Window");
        //
        //     // Example: Add a button inside the window
        //     if (GuiButton((Rectangle){ windowBounds.x + 10, windowBounds.y + 40, 100, 30 }, "Click Me")) {
        //         // Handle button click
        //         TraceLog(LOG_INFO, "Button clicked!");
        //     }
        // }

        // EndMode2D();

        EndDrawing();

    }

    return 0;
}

