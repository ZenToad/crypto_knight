/*
* snake.c
* This is a simple snake game. The snake moves around the screen and eats
* food. The snake grows longer as it eats food. The game is over when the
* snake runs into the wall or into itself.
 */

#include "raylib.h"

#define GRID_SIZE 32
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 1080

typedef struct Snake {
    Vector2 position;
    Vector2 direction;
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
    snake->position.x += snake->direction.x;
    snake->position.y += snake->direction.y;
}


int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Scaled Resolution");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    //
    InitAudioDevice();


    //--------------------------------------------------------------------------------------
    // Load game assets
    // game.sounds[COLLISION] = LoadSound("../prototypes/proto2/collision.wav");

    //--------------------------------------------------------------------------------------
    // Initialize game variables

    // Initialize snake
    Snake snake = { .position = {10, 10}, .direction = {1, 0} };
    float moveTimer = 0.0f;
    const float moveInterval = 0.1f;

    initGameState(SCREEN_WIDTH, SCREEN_HEIGHT);

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // Process Input
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_RIGHT) && snake.direction.x == 0) snake.direction = (Vector2){1, 0};
        if (IsKeyPressed(KEY_LEFT) && snake.direction.x == 0) snake.direction = (Vector2){-1, 0};
        if (IsKeyPressed(KEY_UP) && snake.direction.y == 0) snake.direction = (Vector2){0, -1};
        if (IsKeyPressed(KEY_DOWN) && snake.direction.y == 0) snake.direction = (Vector2){0, 1};
        
        // Update game objects
        //----------------------------------------------------------------------------------

        moveTimer += GetFrameTime();
        if (moveTimer >= moveInterval) {
            moveTimer -= moveInterval;
            MoveSnake(&snake);
        };

        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        // BeginMode2D(camera);

        // Draw grid
        for (int i = 0; i < SCREEN_WIDTH / GRID_SIZE; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / GRID_SIZE; j++) {
                DrawRectangleLines(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE, GRID_SIZE, LIGHTGRAY);
            }
        }

        // Draw snake
        DrawRectangle(snake.position.x * GRID_SIZE, snake.position.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, DARKGREEN);
        // Draw your game graphics here using virtual resolution coordinates
        DrawText("Hello, World!", 100, 100, 20, RED);

        // EndMode2D();

        EndDrawing();

    }

    return 0;
}

