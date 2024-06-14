#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h> // Include the raygui library
#include <style_terminal.h> // Include the terminal style
  
char consoleText[256] = "Hello,\nWorld!"; // Define a global variable for the console text
 
void draw_console() {
    DrawRectangle(0, 0, 800, 100, BLACK);
    DrawRectangle(0, 100, 800, 1, GRAY);
    DrawText("Console", 10, 10, 20, WHITE);

    // Draw the console text box
    Rectangle bounds = { 10, 30, 780, 60 };
    GuiTextBox(bounds, consoleText, 256, true);
}

int main(void) {  
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

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
