/*
* snake.c
* This is a simple snake game. The snake moves around the screen and eats
* food. The snake grows longer as it eats food. The game is over when the
* snake runs into the wall or into itself.
 */

#include "raylib.h"

#define GRID_SIZE 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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


int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - Scaled Resolution");
    // SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    //
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
    const float moveInterval = 0.1f;

    initGameState(SCREEN_WIDTH, SCREEN_HEIGHT);

    double previousTime = GetTime();	
    double currentTime = previousTime;	
    double lag = 0.0;	

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
        };

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

        // BeginMode2D(camera);

        // Draw grid
        for (int i = 0; i < SCREEN_WIDTH / GRID_SIZE; i++) {
            for (int j = 0; j < SCREEN_HEIGHT / GRID_SIZE; j++) {
                // DrawRectangleLines(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE, GRID_SIZE, LIGHTGRAY);
            }
        }

        // Draw snake
        DrawRectangle(interpolatedPosition.x * GRID_SIZE, interpolatedPosition.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, DARKGREEN);

        // EndMode2D();

        EndDrawing();

    }

    return 0;
}

