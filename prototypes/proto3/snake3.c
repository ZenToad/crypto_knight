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

typedef struct game_state_t {
    player_direction_t currDir; 
} game_state_t;
game_state_t G = {0};

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
                eventList[eventListLength++] = (game_event_type_t){im->eventType};
            }
        }
    }
}

void processGameEvents(void) {
    for (int i = 0; i < eventListLength; ++i) {
        game_event_type_t ge = eventList[i];
        switch (ge) {
            case GE_PLAYER_MOVE_UP: {
                TraceLog(LOG_INFO, "Player move up event");
                // what do we even do here?
                // check current player direction.  If it isn't going down,
                // we can go up.
                if (G.currDir != PLAYER_DIR_DOWN) {
                    G.currDir = PLAYER_DIR_UP;
                }
                else {
                    TraceLog(LOG_INFO, "Can't go up while going down");
                }
                break;
            }
            case GE_PLAYER_MOVE_DOWN: {
                TraceLog(LOG_INFO, "Player move down event");
                if (G.currDir != PLAYER_DIR_UP) {
                    G.currDir = PLAYER_DIR_DOWN;
                }
                else {
                    TraceLog(LOG_INFO, "Can't go down while going up");
                }
                break;
            }
            case GE_PLAYER_MOVE_LEFT: {
                TraceLog(LOG_INFO, "Player move left event");
                if (G.currDir != PLAYER_DIR_RIGHT) {
                  G.currDir = PLAYER_DIR_LEFT;
                } else {
                    TraceLog(LOG_INFO, "Can't go left while going right");
                }
                break;
            }
            case GE_PLAYER_MOVE_RIGHT: {
                TraceLog(LOG_INFO, "Player move right event");
                if (G.currDir != PLAYER_DIR_LEFT) {
                    G.currDir = PLAYER_DIR_RIGHT;
                } else {
                    TraceLog(LOG_INFO, "Can't go right while going left");
                }
                break;
            }
            default: {
                assert(0);
            }
        }
        TraceLog(LOG_INFO, "Player Direction: %d", G.currDir);
    }
}

void updateGameObjects(void) {
    // ok so we have the player direction
    // but then we what?  when do we check for collisions?
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


