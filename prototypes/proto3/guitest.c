#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../../src/raygui.h"

#include "../../styles/dark/style_dark.h"

#define CK_GUI_IMPLEMENTATION
#include "../../include/ck_gui.h"

static Vector2 window_position = { 10, 10 };
static Vector2 window_size = { 200, 400 };
static bool minimized = false;
static bool moving = false;
static bool resizing = false;
static Vector2 scroll;


static void DrawContent(Vector2 position, Vector2 scroll) {

    if (GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 50  + scroll.y, 100, 25 }, "Button 1")) {
        printf("Button 1 pressed\n");
    }
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 100 + scroll.y, 100, 25 }, "Button 2");
    GuiButton((Rectangle) { position.x + 20 + scroll.x, position.y + 150 + scroll.y, 100, 25 }, "Button 3");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 200 + scroll.y, 250, 25 }, "A Label");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 250 + scroll.y, 250, 25 }, "Another Label");
    GuiLabel((Rectangle) { position.x + 20 + scroll.x, position.y + 300 + scroll.y, 250, 25 }, "Yet Another Label");

}

int main(void) {
    InitWindow(960, 560, "raygui - floating window example");
    SetTargetFPS(60);
    GuiLoadStyleDark();

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(DARKGREEN);
        GuiWindowFloating(
            &window_position, &window_size, &minimized, &moving, 
            &resizing, &DrawContent, (Vector2) { 140, 320 }, 
            &scroll, "Movable & Scalable Window"
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
