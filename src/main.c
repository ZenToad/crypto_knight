#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h> // Include the raygui library
#include <style_terminal.h> // Include the terminal style
  
char consoleText[256] = "Hello,\nWorld!"; // Define a global variable for the console text
 
void draw_console() {
}

int main(void) {  
    // Initialization
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitWindow(screenWidth, screenHeight, "Hello, World!");
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
