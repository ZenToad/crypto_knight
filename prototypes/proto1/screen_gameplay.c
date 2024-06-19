/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <math.h>
#include <stdbool.h>

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------
typedef struct man_t {
    int xpos;
    int ypos;
    int dx;
    int dy;
    int use_comp;
} man_t;
man_t man;

typedef struct computer_t {
    int xpos;
    int ypos;
    int can_use;
} computer_t;
computer_t comp;

void draw_little_man(void) {

    DrawRectangle(man.xpos, man.ypos, 20, 20, BROWN);
    DrawRectangle(man.xpos, man.ypos + 20, 20, 20, BLACK);
}

void draw_computer(void) {
    // desk
    DrawRectangle(comp.xpos, comp.ypos, 60, 20, GRAY);
    if (comp.can_use) {
        DrawRectangle(comp.xpos + 20, comp.ypos - 20, 20, 20, GREEN);
    } else {
        DrawRectangle(comp.xpos + 20, comp.ypos - 20, 20, 20, RAYWHITE);
    }
}

void draw_terminal_view(void) {

}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    // man = (man_t) { 20, 20, 5, 5 };
    man.ypos = 20;
    man.xpos = 20;
    man.dx = 5;
    man.dy = 5;

    comp = (computer_t) { 200, 200 };
    TraceLog(LOG_INFO, "Gameplay Screen Initialized");
}

bool is_man_near_comp(void) {
    if (abs(man.xpos - comp.xpos) > 20) return false;
    if (abs(man.ypos - comp.ypos) > 20 ) return false;
    return true;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!
    comp.can_use = is_man_near_comp() ? 1 : 0;

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        man.xpos += man.dx;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        man.xpos -= man.dx;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        man.ypos -= man.dy;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        man.ypos += man.dy;
    }
    if (IsKeyDown(KEY_E)) {
        // check if we are near the computer
        if (is_man_near_comp()) {
            man.use_comp = 1;
            TraceLog(LOG_INFO, "Using computer");
        }
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    Vector2 pos = { 20, 10 };
    // DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize*3.0f, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);
    draw_computer();
    draw_little_man();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
