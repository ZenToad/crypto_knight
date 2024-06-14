#include "raylib.h"
#include <stdio.h>

void sandbox1(void) {
    // Initialize the window and print the available monitors
    int monitorCount = GetMonitorCount();
    printf("Available monitors: %d\n", monitorCount);

    for (int i = 0; i < monitorCount; i++) {
        printf("Monitor %d: %s (%d x %d)\n", i, GetMonitorName(i), GetMonitorWidth(i), GetMonitorHeight(i));
    }

    // Assuming you want to use the primary monitor (index 0)
    int monitorIndex = 0;

    if (monitorIndex < 0 || monitorIndex >= monitorCount) {
        printf("Invalid monitor index. Using primary monitor instead.\n");
        monitorIndex = 0;
    }

    // Get the size of the selected monitor
    int screenWidth = GetMonitorWidth(monitorIndex);
    int screenHeight = GetMonitorHeight(monitorIndex);

    // Initialize the window with the size of the selected monitor
    InitWindow(screenWidth, screenHeight, "Raylib - Monitor Size Example");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display the size of the selected monitor
        DrawText(TextFormat("Selected Monitor Size: %d x %d", screenWidth, screenHeight), 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

}

void sandbox2(void) {
    int monitor = GetCurrentMonitor();

}

int main(void) {
    sandbox2();
    return 0;
}

