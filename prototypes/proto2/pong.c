#include "raylib.h"

enum GameState {
    GAME_ATTRACT,
    GAME_GET_READY,
    GAME_COUNTDOWN,
    GAME_PLAYING,
    GAME_OVER, 
};

enum SoundType {
    COLLISION,
    COUNTDOWN,
    GAME_START,
    OUT_OF_BOUNDS,
    MAX_SOUNDS,
};

typedef struct game_t {
    enum GameState state;
    int score;
    int countdown;
    float elapsedTime;
    Sound sounds[MAX_SOUNDS];
} game_t;
game_t game;

typedef struct player_t {
    Vector2 position;
    Vector2 size;
    Color color;
    int score;
} player_t;
player_t playerLeft;
player_t playerRight;

typedef struct ball_t {
    Vector2 position;
    Vector2 speed;
    int radius;
    Color color;
} ball_t;
ball_t ball;

void drawRectangle(Vector2 position, Vector2 size, Color color) {
    DrawRectangle(position.x, position.y, size.x, size.y, color);
}

void drawCircle(Vector2 position, int radius, Color color) {
    DrawCircle(position.x, position.y, radius, color);
}

bool checkOutOfBoundsLeft(ball_t *ball, int screenWidth) {
    return ball->position.x <= 0;
}

bool checkOutOfBoundsRight(ball_t *ball, int screenWidth) {
    return ball->position.x >= screenWidth;
}

void handleTopBotCollision(ball_t *ball, int screenWidth, int screenHeight) {
    if ((ball->position.y >= screenHeight - ball->radius) || (ball->position.y <= ball->radius)) {
        ball->speed.y *= -1;
        PlaySound(game.sounds[COLLISION]);
    }
}

void handleCollisionBallPlayer(ball_t *ball, player_t *player) {
    if (CheckCollisionCircleRec(ball->position, ball->radius, (Rectangle){player->position.x, player->position.y, player->size.x, player->size.y})) {
        ball->speed.x *= -1;
        PlaySound(game.sounds[COLLISION]);
    }
}

void printPlayer(player_t player) {
    printf("Paddle position: (%f, %f)\n", player.position.x, player.position.y);
    printf("Paddle size: (%f, %f)\n", player.size.x, player.size.y);
}

void printBall(ball_t ball) {
    printf("Ball position: (%f, %f)\n", ball.position.x, ball.position.y);
    printf("Ball speed: (%f, %f)\n", ball.speed.x, ball.speed.y);
    printf("Ball radius: %d\n", ball.radius);
}

void drawScore(player_t playerLeft, player_t playerRight) {
    DrawText(TextFormat("%i", playerLeft.score), 190, 20, 20, WHITE);
    DrawText(TextFormat("%i", playerRight.score), 600, 20, 20, WHITE);
}

void drawCountdown(int countdown, int screenWidth, int screenHeight) {
    int textWidth = MeasureText(TextFormat("%i", countdown), 84);
    DrawText(TextFormat("%i", countdown), screenWidth/2 - textWidth/2, screenHeight/4, 84, WHITE);

    textWidth = MeasureText("GET READY", 20); 
    DrawText("GET READY!!!", screenWidth/2 - textWidth/2, 20, 20, WHITE);
}

void resetPlayingField(int screenWidth, int screenHeight) {
    playerLeft.position = (Vector2){20, screenHeight / 2};
    playerRight.position = (Vector2){screenWidth - 20, screenHeight / 2};
    ball.position = (Vector2){screenWidth / 2, screenHeight / 2};
    ball.speed = (Vector2){5, 5};
}

void resetGameState(int screenWidth, int screenHeight) {

    game.state = GAME_GET_READY;
    game.score = 0;
    game.countdown = 3;
    game.elapsedTime = 0.0f;

    playerLeft.position = (Vector2){20, screenHeight / 2};
    playerLeft.score = 0;

    playerRight.position = (Vector2){screenWidth - 20, screenHeight / 2};
    playerRight.score = 0;

    ball.position = (Vector2){screenWidth / 2, screenHeight / 2};

}

void initGameState(int screenWidth, int screenHeight) {

    game.state = GAME_ATTRACT;

    game.countdown = 3;
    game.elapsedTime = 0.0f;

    playerLeft.position = (Vector2){20, screenHeight / 2};
    playerLeft.size = (Vector2){10, 100};
    playerLeft.color = RED;
    playerLeft.score = 0;

    playerRight.position = (Vector2){screenWidth - 20, screenHeight / 2};
    playerRight.size = (Vector2){10, 100};
    playerRight.color = BLUE;
    playerRight.score = 0;

    ball.position = (Vector2){screenWidth / 2, screenHeight / 2};
    ball.speed = (Vector2){5, 5};
    ball.radius = 10;
    ball.color = WHITE;
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
    game.sounds[COLLISION] = LoadSound("../prototypes/proto2/collision.wav");
    game.sounds[COUNTDOWN] = LoadSound("../prototypes/proto2/countdown.wav");
    game.sounds[GAME_START] = LoadSound("../prototypes/proto2/game_start.wav");
    game.sounds[OUT_OF_BOUNDS] = LoadSound("../prototypes/proto2/out_of_bounds.wav");

    //--------------------------------------------------------------------------------------
    // Initialize game variables
    initGameState(screenWidth, screenHeight);

    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose()) {   // Detect window close button or ESC key

        // Process Input
        //----------------------------------------------------------------------------------
        if (game.state == GAME_PLAYING || game.state == GAME_COUNTDOWN || game.state == GAME_GET_READY) {

            if (IsKeyDown(KEY_W)) {
                playerLeft.position.y -= 5;
                if (playerLeft.position.y <= 0) {
                    playerLeft.position.y = 0;
                }
            }
            if  (IsKeyDown(KEY_S)) {
                playerLeft.position.y += 5;
                if (playerLeft.position.y >= screenHeight - playerLeft.size.y) {
                    playerLeft.position.y = screenHeight - playerLeft.size.y;
                }
            }
            if (IsKeyDown(KEY_UP)) {
                playerRight.position.y -= 5;
                if (playerRight.position.y <= 0) {
                    playerRight.position.y = 0;
                }
            }
            if  (IsKeyDown(KEY_DOWN)) {
                playerRight.position.y += 5;
                if (playerRight.position.y >= screenHeight - playerRight.size.y) {
                    playerRight.position.y = screenHeight - playerRight.size.y;
                }
            }

        } else if (game.state == GAME_OVER || game.state == GAME_ATTRACT) {

            if (IsKeyPressed(KEY_ENTER)) {
                resetGameState(screenWidth, screenHeight);
            }

        } 

        // Update game objects
        //----------------------------------------------------------------------------------

        if (game.state == GAME_PLAYING) {

            // Move ball
            ball.position.x += ball.speed.x;
            ball.position.y += ball.speed.y;

            handleCollisionBallPlayer(&ball, &playerLeft);
            handleCollisionBallPlayer(&ball, &playerRight);

            if (checkOutOfBoundsLeft(&ball, screenWidth)) {
                playerRight.score++;
                resetPlayingField(screenWidth, screenHeight);
                game.state = GAME_GET_READY;
                game.countdown = 3;
                game.elapsedTime = 0.0f;
                PlaySound(game.sounds[OUT_OF_BOUNDS]);
            } else if (checkOutOfBoundsRight(&ball, screenWidth)) {
                playerLeft.score++;
                resetPlayingField(screenWidth, screenHeight);
                game.state = GAME_GET_READY;
                game.countdown = 3;
                game.elapsedTime = 0.0f;
                PlaySound(game.sounds[OUT_OF_BOUNDS]);
            }

            handleTopBotCollision(&ball, screenWidth, screenHeight);

            if (playerLeft.score >= 5) {

                game.state = GAME_OVER;

            } else if (playerRight.score >= 5) {

                game.state = GAME_OVER;
            }

        } else if (game.state == GAME_GET_READY) {

            game.elapsedTime += GetFrameTime();

            if (game.elapsedTime > 0.75f) {
                game.state = GAME_COUNTDOWN;
                game.elapsedTime = 0.0f;
                PlaySound(game.sounds[COUNTDOWN]);
            }

        } else if (game.state == GAME_COUNTDOWN) {

            // update timer
            game.elapsedTime += GetFrameTime();

            if (game.countdown == 0) {

                game.state = GAME_PLAYING;

                PlaySound(game.sounds[GAME_START]);

            } else if (game.elapsedTime >= 1.0f) {

                game.countdown--;
                game.elapsedTime = 0.0f;

                if (game.countdown > 0) {

                    PlaySound(game.sounds[COUNTDOWN]);
                }
            }
        }


        // Render
        // ---------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);
        if (game.state == GAME_ATTRACT) {

            int w = MeasureText("Press Enter to start", 20);
            DrawText("Press Enter to start", screenWidth/2 - w/2, screenHeight/2 - 20/2, 20, WHITE);

        } else if (game.state == GAME_GET_READY) {

            drawRectangle(playerLeft.position, playerLeft.size, playerLeft.color);
            drawRectangle(playerRight.position, playerRight.size, playerRight.color);
            drawCircle(ball.position, ball.radius, ball.color);
            drawScore(playerLeft, playerRight);
            drawCountdown(game.countdown, screenWidth, screenHeight);

        } else if (game.state == GAME_COUNTDOWN) {

            drawRectangle(playerLeft.position, playerLeft.size, playerLeft.color);
            drawRectangle(playerRight.position, playerRight.size, playerRight.color);
            drawCircle(ball.position, ball.radius, ball.color);
            drawScore(playerLeft, playerRight);
            if (game.countdown > 0) {
                drawCountdown(game.countdown, screenWidth, screenHeight);
            }

        } else if (game.state == GAME_OVER) {

            int w = MeasureText("Game Over", 20);
            DrawText("Game Over", screenWidth/2 - w/2, screenHeight/2 - 20, 20, WHITE);
            w = MeasureText("Press Enter to restart", 20);
            DrawText("Press Enter to restart", screenWidth/2 - w/2, screenHeight/2 + 20, 20, WHITE);

        } else if (game.state == GAME_PLAYING) {

            drawRectangle(playerLeft.position, playerLeft.size, playerLeft.color);
            drawRectangle(playerRight.position, playerRight.size, playerRight.color);
            drawCircle(ball.position, ball.radius, ball.color);
            drawScore(playerLeft, playerRight);

        }

        EndDrawing();

    }

    return 0;
}
