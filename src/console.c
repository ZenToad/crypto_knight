#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h> // Include the raygui library
#include <style_terminal.h> // Include the terminal style
                            //
// #define STB_TEXTEDIT_CHARTYPE char
// #define STB_TEXTEDIT_IMPLEMENTATION
// #include <stb_textedit.h> // Include the stb text editing library

char consoleText[256] = "Hello,\nWorld!"; // Define a global variable for the console text
 
void draw_console() {
    // 
    // how do I start.  Because there's a part of me that really wants to
    // implement the hot-reloading so I can fiddle.  but is that the
    // best use of time here?

    // also I'm curious about using the stb lib for text editing
}

int main(void) {  
    // Initialization
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "Console");
    GuiLoadStyleTerminal();
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        
        // Draw
        BeginDrawing();

        ClearBackground(BLACK);

        draw_console();
        // DrawText("Hello, World!", 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

