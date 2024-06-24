#include "raylib.h"

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

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    InitAudioDevice();

    //--------------------------------------------------------------------------------------
    // Load game assets
    // game.sounds[COLLISION] = LoadSound("../prototypes/proto2/collision.wav");

    //--------------------------------------------------------------------------------------
    // Initialize game variables
    initGameState(screenWidth, screenHeight);

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // Process Input
        //----------------------------------------------------------------------------------
        
        // Update game objects
        //----------------------------------------------------------------------------------

        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();

    }

    return 0;
}

