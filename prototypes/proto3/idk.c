#include "raylib.h"

#define screenWidth 800
#define screenHeight 450

double previousTime;	
double currentTime;	
double elapsed;	
double lag;	
double time;
double moveSpeed;
// double t = 0.0;
// double dt = 0.01;
//
// double currentTime = hires_time_in_seconds();
// double accumulator = 0.0;
//
// State previous;
// State current;
//
// while ( !quit )
// {
//     double newTime = time();
//     double frameTime = newTime - currentTime;
//     if ( frameTime > 0.25 )
//         frameTime = 0.25;
//     currentTime = newTime;
//
//     accumulator += frameTime;
//
//     while ( accumulator >= dt )
//     {
//         previousState = currentState;
//         integrate( currentState, t, dt );
//         t += dt;
//         accumulator -= dt;
//     }
//
//     const double alpha = accumulator / dt;
//
//     State state = currentState * alpha +
//         previousState * ( 1.0 - alpha );
//
//     render( state );
// }
void updateGame() {
    TraceLog(LOG_INFO, "updating game");
}

void renderGame(double alpha) {
    TraceLog(LOG_INFO, "rendering game: alpha: %f", alpha);
}

void advanceTime(void) {
    time += 0.15;
    TraceLog(LOG_INFO, "time: %f", time);
}

void doTick() {
    previousTime = currentTime;	
    currentTime = time;
    elapsed = currentTime - previousTime;	
    lag += elapsed;
    TraceLog(LOG_INFO, "prev: %f, curr: %f, elapsed: %f, lag: %f", previousTime, currentTime, elapsed, lag);

    if (lag >= moveSpeed) {
        lag -= moveSpeed;
        TraceLog(LOG_INFO, "lag: %f", lag);
        updateGame();
    }

    renderGame(lag / moveSpeed);
}

int main(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "idk figure out");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
                                    
    time = 0.0;
    previousTime = time;	
    currentTime = previousTime;	
    elapsed = currentTime - previousTime;	
    lag = 0.0;	
    moveSpeed = 0.1;


    // thinking if I set shit up, then use the space bar to do a 'tick' 
    //

    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        if (IsKeyPressed(KEY_SPACE)) {
            doTick();
        }
        if (IsKeyPressed(KEY_D)) {
            advanceTime();
        }
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();

    }

    return 0;
}
