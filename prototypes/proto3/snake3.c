#include "raylib.h"
#include <assert.h>
// #include "ck_acme.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define INPUT_MAP_MAX 100
#define EVENT_MAX 100

/* 
 *  This version we tackle the input handling.  Because that shit ain't right.
 *  we should get the input, and turn that into game events.  Then process the
 *  events and a separate time.
 *
 *  Next Game Frame:
 *  ---------------- 
 *  Get Input key/mouse
 *  Turn input into game events
 *  process game events
 *  update game objects
 *  render game world
 */
typedef enum input_map_type_t {
    IM_KEYPRESS = 0,
    IM_EVENT_COUNT,
} input_map_type_t;

typedef enum game_event_type_t {
    GE_PLAYER_MOVE_UP = 0,
    GE_EVENT_COUNT,
} game_event_type_t;

typedef struct input_map_t {
    input_map_type_t inputType;
    game_event_type_t eventType;
    int data; 
} input_map_t;

input_map_t inputMap[INPUT_MAP_MAX];
int inputMapLength = 0;
game_event_type_t eventList[EVENT_MAX];
int eventListLength = 0;

// We could have a mapping of input to game event, and just do that 
// in one place.  
// Got some input, table lookup, add event,  
// Then we don't even need the input step saved.  We just look at input,
// convert to event, add event to event list.
// What would that look like?
void processInput(void) {
    for (int i = 0; i < inputMapLength; ++i) {
        input_map_t *im = &inputMap[i];
        if (im->inputType == IM_KEYPRESS) {
            if (IsKeyPressed(im->data)) {
                // Add event to event list
                eventList[eventListLength++] = (game_event_type_t){im->eventType};
            }
        }
        // ... ect ...
    }
}

void processGameEvents(void) {
    for (int i = 0; i < eventListLength; ++i) {
        game_event_type_t ge = eventList[i];
        switch (ge) {
            case GE_PLAYER_MOVE_UP: {
                // Move player up
                TraceLog(LOG_INFO, "Player move up event");
                break;
            }
            // ... ect ...
            default: {
                assert(0);
            }
        }
    }
}

void updateGameObjects(void) {

}

void renderGameWorld(void) {

}

void prepareForNextFrame(void) {
    eventListLength = 0;
}

void initializeGame(void) {
    // Setup input map
    inputMapLength = 0;
    inputMap[inputMapLength++] = (input_map_t) {
        .inputType = IM_KEYPRESS, 
        .eventType = GE_PLAYER_MOVE_UP,
        .data = KEY_UP,
    };
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake 3 baby!!!");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    InitAudioDevice();

    initializeGame();

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // reset game objects and stuff
        prepareForNextFrame();

        // Process Input
        //----------------------------------------------------------------------------------
        processInput();

        processGameEvents();

        // Update game objects
        //----------------------------------------------------------------------------------
        updateGameObjects();
        
        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();

    }

    return 0;
}


