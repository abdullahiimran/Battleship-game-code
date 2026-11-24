#include "raylib.h"
#include<iostream>
void Grid(int Grid_X, int Grid_Y, int grid[10][10], bool hide = false) {
    int cell_size = 60;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10;j++) {
            int X = Grid_X + i * cell_size;
            int Y = Grid_Y + j * cell_size;
            if (j == 0) {
                char Letter = 'A' + i;
                int Letter_X = X + (cell_size / 2) - 5;
                int Letter_Y = Grid_Y - 30;
                DrawText(TextFormat("%c", Letter), Letter_X, Letter_Y, 28, WHITE);
            }
            if (i == 0) {
                int Number_X = Grid_X - 35;
                int Number_Y = Y + (cell_size / 2) - 10;
                DrawText(TextFormat("%d", j + 1), Number_X, Number_Y, 28, WHITE);
            }
            Color cell_color = WHITE;
            if (hide) {
                if (grid[i][j] == 2) {
                    cell_color = RED;
                }
                else if (grid[i][j] == 3) {
                    cell_color = BLACK;
                }
                else {
                    cell_color = BLUE;
                }
            }
            else {
                if (grid[i][j] == 1) {
                    cell_color = GRAY;
                }
            }

            DrawRectangle(X, Y, cell_size, cell_size, cell_color);
            DrawRectangleLines(X, Y, cell_size, cell_size, BLACK);
        }
    }
}
void values(int GridX, int GridY, int grid[10][10], int value, int& count) {
    const int cell = 60;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int i = (mouseX - GridX) / cell;
        int j = (mouseY - GridY) / cell;
        if (i >= 0 && i < 10 && j < 10 && j >= 0) {
            if (grid[i][j] == 0) {
                grid[i][j] = value;
                count++;
            }
        }
    }
}
bool values2(int GridX, int GridY, int grid[10][10], int& hits) {
    const int cell = 60;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        int i = (mouseX - GridX) / cell;
        int j = (mouseY - GridY) / cell;
        if (i >= 0 && i < 10 && j < 10 && j >= 0) {
            if (grid[i][j] == 0) {
                grid[i][j] = 3;
                return true;

            }
            else if (grid[i][j] == 1) {
                grid[i][j] = 2;
                hits++;
                return true;
            }
        }
    }
    return false;
}
int main()
{
    Color green = { 20,160,133,255 };
    InitWindow(1920, 1080, "Battleship Game");
    ToggleFullscreen();
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    Texture2D Screen0 = LoadTexture("screen0.png");
    Texture2D Screen1 = LoadTexture("screen1.png");
    Texture2D Screen2 = LoadTexture("screen2.png");
    Texture2D Screen3 = LoadTexture("screen3.png");
    int gridX = 660;
    int gridY = 240;
    SetTargetFPS(120);
    int player_grid1[10][10] = { 0 };
    int player_grid2[10][10] = { 0 };
    int input_count = 0;
    int max_inputs = 5;
    int p1_hits = 0;
    int p2_hits = 0;
    int win_hits = 5;
    int winner = 0;
    int screen = 0;
    bool hide_ship = false;
    int turn = 1;

    while (WindowShouldClose() == 0)
    {
        if (screen == 0) {
            int value = 1;
            values(gridX, gridY, player_grid1, value, input_count);
            if (input_count >= max_inputs) {
                input_count = 0;
                screen = 1;
            }
        }
        else if (screen == 1) {
            int value = 1;
            values(gridX, gridY, player_grid2, value, input_count);
            if (input_count >= max_inputs) {
                input_count = 0;
                screen = 2;
            }
        }
        else if (screen == 2) {
            if (turn == 1) {
                if (values2(1060, 260, player_grid2, p1_hits)) {
                    if (p1_hits < win_hits) {
                        turn = 2;
                    }
                }
            }
            else if (turn == 2) {
                if (values2(260, 260, player_grid1, p2_hits)) {
                    if (p2_hits < win_hits) {
                        turn = 1;
                    }
                }
            }
            if (p1_hits >= win_hits) {
                winner = 1;
                screen = 3;
            }
            else if (p2_hits >= win_hits) {
                winner = 2;
                screen = 3;
            }
        }

        BeginDrawing();
        if (screen == 0) {
            Rectangle source = { 0.0f,0.0f,(float)Screen0.width,(float)Screen0.height };
            Rectangle dest = { 0.0f,0.0f,(float)width,(float)height };
            DrawTexturePro(Screen0, source, dest, { 0.0f,0.0f }, 0.0f, WHITE);
            Grid(gridX, gridY, player_grid1);
        }
        else if (screen == 1) {
            Rectangle source = { 0.0f,0.0f,(float)Screen1.width,(float)Screen1.height };
            Rectangle dest = { 0.0f,0.0f,(float)width,(float)height };
            DrawTexturePro(Screen1, source, dest, { 0.0f,0.0f }, 0.0f, WHITE);
            Grid(gridX, gridY, player_grid2);
        }
        else if (screen == 2) {
            Rectangle source = { 0.0f,0.0f,(float)Screen2.width,(float)Screen2.height };
            Rectangle dest = { 0.0f,0.0f,(float)width,(float)height };
            DrawTexturePro(Screen2, source, dest, { 0.0f,0.0f }, 0.0f, WHITE);
            hide_ship = true;
            Grid(260, 260, player_grid1, hide_ship);
            Grid(1060, 260, player_grid2, hide_ship);
        }
        else if (screen == 3) {
            Rectangle source = { 0.0f,0.0f,(float)Screen3.width,(float)Screen3.height };
            Rectangle dest = { 0.0f,0.0f,(float)width,(float)height };
            DrawTexturePro(Screen3, source, dest, { 0.0f,0.0f }, 0.0f, WHITE);
            DrawText("Game Over", 830, 450, 50, WHITE);
            if (winner == 1) {
                DrawText("Player 1 Wins!", 830, 530, 36, WHITE);
            }
            else if (winner == 2) {
                DrawText("Player 2 Wins!", 830, 530, 36, WHITE);
            }
        }
        EndDrawing();
    }


    UnloadTexture(Screen0);
    UnloadTexture(Screen1);
    UnloadTexture(Screen2);
    UnloadTexture(Screen3);
    CloseWindow();
    return 0;
}