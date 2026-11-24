#include "raylib.h"
#include <iostream>

int shipSizes[3] = { 2, 3, 4 };
bool firstCellSelected = false;
int firstRow = 0, firstColumn = 0;

void DrawInstructions(int width, int height) {
    Color panelColor = { 10, 30, 42, 245 };
    DrawRectangle(0, 0, width, height, panelColor);

    const char* heading = "=========== HOW TO PLAY ===========";
    int headingSize = 42;
    int headingWidth = MeasureText(heading, headingSize);
    DrawText(heading, width / 2 - headingWidth / 2, height / 2 - 220, headingSize, WHITE);

    const char* line1 = "1. Each player places their ships (sizes 2, 3, 4) in their respective grid.";
    const char* line2 = "2. Take turns attacking the opponent's grid by guessing ship locations.";
    const char* line3 = "3. First player to eliminate all ships of the opponent (9 hits) wins the game.";
    const char* line4 = "Click 'Back to Menu' or press ESC to return.";

    int textSize = 28;
    DrawText(line1, width / 2 - MeasureText(line1, textSize) / 2, height / 2 - 120, textSize, LIGHTGRAY);
    DrawText(line2, width / 2 - MeasureText(line2, textSize) / 2, height / 2 - 65, textSize, LIGHTGRAY);
    DrawText(line3, width / 2 - MeasureText(line3, textSize) / 2, height / 2 - 10, textSize, LIGHTGRAY);
    DrawText(line4, width / 2 - MeasureText(line4, 24) / 2, height / 2 + 170, 24, SKYBLUE);
}

void Grid(int Grid_X, int Grid_Y, int grid[10][10], bool hide = false) {
    int cell_size = 60;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
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

bool insideGrid(int startX, int startY, int cellSize, int& row, int& column) {
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    if ((mouseX >= startX && mouseX < startX + 10 * cellSize) && (mouseY >= startY && mouseY < startY + 10 * cellSize)) {
        column = (mouseX - startX) / cellSize;
        row = (mouseY - startY) / cellSize;
        return true;
    }
    return false;
}

bool placementValidation(int grid[10][10], int row1, int col1, int row2, int col2, int requiredSize) {
    if (row1 < 0 || row1 >= 10 || row2 < 0 || row2 >= 10 || col1 < 0 || col1 >= 10 || col2 < 0 || col2 >= 10)
        return false;
    if (row1 != row2 && col1 != col2)
        return false;

    int length;
    if (row1 == row2) {
        if (col1 > col2)
            length = col1 - col2 + 1;
        else
            length = col2 - col1 + 1;
    }
    else {
        if (row1 > row2)
            length = row1 - row2 + 1;
        else
            length = row2 - row1 + 1;
    }

    if (length != requiredSize)
        return false;

    if (row1 == row2) {
        int start = col1 < col2 ? col1 : col2;
        int end = col1 > col2 ? col1 : col2;
        for (int j = start; j <= end; j++) {
            if (grid[j][row1] != 0)
                return false;
        }
    }
    else {
        int start = row1 < row2 ? row1 : row2;
        int end = row1 > row2 ? row1 : row2;
        for (int i = start; i <= end; i++) {
            if (grid[col1][i] != 0)
                return false;
        }
    }
    return true;
}

void placeShip(int grid[10][10], int row1, int col1, int row2, int col2) {
    if (row1 == row2) {
        int start = col1 < col2 ? col1 : col2;
        int end = col1 > col2 ? col1 : col2;
        for (int j = start; j <= end; j++) {
            grid[j][row1] = 1;
        }
    }
    else {
        int start = row1 < row2 ? row1 : row2;
        int end = row1 > row2 ? row1 : row2;
        for (int i = start; i <= end; i++) {
            grid[col1][i] = 1;
        }
    }
}

void handleShipPlacement(int startX, int startY, int grid[10][10], int& shipsPlacedCount) {
    const int cellSize = 60;
    int row, col;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (insideGrid(startX, startY, cellSize, row, col)) {
            if (firstCellSelected == false) {
                firstRow = row;
                firstColumn = col;
                firstCellSelected = true;
            }
            else {
                int requiredSize = shipSizes[shipsPlacedCount];
                if (placementValidation(grid, firstRow, firstColumn, row, col, requiredSize)) {
                    placeShip(grid, firstRow, firstColumn, row, col);
                    shipsPlacedCount++;
                }
                firstCellSelected = false;
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

void resetBoard(int grid[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grid[i][j] = 0;
        }
    }
}

void restartGame(
    int player_grid1[10][10],
    int player_grid2[10][10],
    int& ships_placed,
    int& p1_hits,
    int& p2_hits,
    int& winner,
    int& turn,
    bool& hide_ship,
    int& screen,
    int targetScreen
) {
    resetBoard(player_grid1);
    resetBoard(player_grid2);
    ships_placed = 0;
    firstCellSelected = false;
    p1_hits = 0;
    p2_hits = 0;
    winner = 0;
    turn = 1;
    hide_ship = false;
    screen = targetScreen;
}

int main() {
    Color green = { 20, 160, 133, 255 };
    Color darkTeal = { 12, 45, 58, 255 };
    InitWindow(1920, 1080, "Battleship Game");
    SetExitKey(KEY_NULL); // Prevent ESC from closing window while viewing instructions
    ToggleFullscreen();
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    Texture2D Screen_1 = LoadTexture("screen_1.png");
    Texture2D Screen0 = LoadTexture("screen0.png");
    Texture2D Screen1 = LoadTexture("screen1.png");
    Texture2D Screen2 = LoadTexture("screen2.png");
    Texture2D Screen3 = LoadTexture("screen3.png");
    int gridX = 660;
    int gridY = 240;
    SetTargetFPS(120);
    int player_grid1[10][10] = { 0 };
    int player_grid2[10][10] = { 0 };
    int ships_placed = 0;
    int max_ships = 3;
    int p1_hits = 0;
    int p2_hits = 0;
    int win_hits = 9;
    int winner = 0;
    int screen = -1;
    bool showInstructions = false;
    bool hide_ship = false;
    int turn = 1;
    bool shouldExit = false;

    float btnW = 360.0f;
    float btnH = 68.0f;
    float btnX = (float)width / 2.0f - btnW / 2.0f;
    Rectangle opt1Btn = { btnX, (float)height / 2.0f - 80.0f, btnW, btnH };
    Rectangle opt2Btn = { btnX, (float)height / 2.0f + 5.0f, btnW, btnH };
    Rectangle opt3Btn = { btnX, (float)height / 2.0f + 90.0f, btnW, btnH };
    Rectangle backBtn = { (float)width / 2.0f - 140.0f, (float)height / 2.0f + 80.0f, 280.0f, 65.0f };

    Rectangle restartBtn = { 760.0f, 600.0f, 200.0f, 60.0f };
    Rectangle gameOverMenuBtn = { 980.0f, 600.0f, 200.0f, 60.0f };

    while (!WindowShouldClose() && !shouldExit) {
        Vector2 mousePos = GetMousePosition();
        bool h1 = CheckCollisionPointRec(mousePos, opt1Btn);
        bool h2 = CheckCollisionPointRec(mousePos, opt2Btn);
        bool h3 = CheckCollisionPointRec(mousePos, opt3Btn);
        bool hBack = CheckCollisionPointRec(mousePos, backBtn);
        bool hRestart = CheckCollisionPointRec(mousePos, restartBtn);
        bool hGameOverMenu = CheckCollisionPointRec(mousePos, gameOverMenuBtn);

        if (screen == -1) {
            if (showInstructions) {
                if (IsKeyPressed(KEY_ESCAPE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && hBack)) {
                    showInstructions = false;
                }
            }
            else {
                bool startGame = IsKeyPressed(KEY_ONE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && h1);
                bool openHelp = IsKeyPressed(KEY_TWO) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && h2);
                bool exitGame = IsKeyPressed(KEY_THREE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && h3);

                if (startGame) {
                    restartGame(player_grid1, player_grid2, ships_placed, p1_hits, p2_hits, winner, turn, hide_ship, screen, 0);
                }
                else if (openHelp) {
                    showInstructions = true;
                }
                else if (exitGame) {
                    shouldExit = true;
                }
            }
        }
        else if (screen == 0) {
            handleShipPlacement(gridX, gridY, player_grid1, ships_placed);
            if (ships_placed >= max_ships) {
                ships_placed = 0;
                firstCellSelected = false;
                screen = 1;
            }
        }
        else if (screen == 1) {
            handleShipPlacement(gridX, gridY, player_grid2, ships_placed);
            if (ships_placed >= max_ships) {
                ships_placed = 0;
                firstCellSelected = false;
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
        else if (screen == 3) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (hRestart) {
                    restartGame(player_grid1, player_grid2, ships_placed, p1_hits, p2_hits, winner, turn, hide_ship, screen, 0);
                }
                else if (hGameOverMenu) {
                    restartGame(player_grid1, player_grid2, ships_placed, p1_hits, p2_hits, winner, turn, hide_ship, screen, -1);
                }
            }
        }

        BeginDrawing();
        if (screen == -1) {
            ClearBackground(darkTeal);
            Rectangle source = { 0.0f, 0.0f, (float)Screen_1.width, (float)Screen_1.height };
            Rectangle dest = { 0.0f, 0.0f, (float)width, (float)height };
            DrawTexturePro(Screen_1, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            if (showInstructions) {
                DrawInstructions(width, height);
                DrawRectangleRec(backBtn, hBack ? SKYBLUE : green);
                DrawRectangleLinesEx(backBtn, 3.0f, WHITE);
                const char* backText = "BACK TO MENU";
                int backTextW = MeasureText(backText, 28);
                DrawText(backText, (int)(backBtn.x + backBtn.width / 2 - backTextW / 2), (int)(backBtn.y + backBtn.height / 2 - 14), 28, WHITE);
            }
            else {
                const char* menuHeader = "=========== MAIN MENU ===========";
                int headerSize = 46;
                DrawText(menuHeader, width / 2 - MeasureText(menuHeader, headerSize) / 2, height / 2 - 200, headerSize, WHITE);

                const char* lbl1 = "1) Play Game";
                const char* lbl2 = "2) How to Play";
                const char* lbl3 = "3) Exit Game";

                DrawRectangleRec(opt1Btn, h1 ? SKYBLUE : green);
                DrawRectangleLinesEx(opt1Btn, 3.0f, WHITE);
                DrawText(lbl1, (int)(opt1Btn.x + opt1Btn.width / 2 - MeasureText(lbl1, 30) / 2), (int)(opt1Btn.y + 19), 30, WHITE);

                DrawRectangleRec(opt2Btn, h2 ? SKYBLUE : DARKBLUE);
                DrawRectangleLinesEx(opt2Btn, 3.0f, WHITE);
                DrawText(lbl2, (int)(opt2Btn.x + opt2Btn.width / 2 - MeasureText(lbl2, 30) / 2), (int)(opt2Btn.y + 19), 30, WHITE);

                DrawRectangleRec(opt3Btn, h3 ? MAROON : DARKGRAY);
                DrawRectangleLinesEx(opt3Btn, 3.0f, WHITE);
                DrawText(lbl3, (int)(opt3Btn.x + opt3Btn.width / 2 - MeasureText(lbl3, 30) / 2), (int)(opt3Btn.y + 19), 30, WHITE);
            }
        }
        else if (screen == 0) {
            Rectangle source = { 0.0f, 0.0f, (float)Screen0.width, (float)Screen0.height };
            Rectangle dest = { 0.0f, 0.0f, (float)width, (float)height };
            DrawTexturePro(Screen0, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            Grid(gridX, gridY, player_grid1);
        }
        else if (screen == 1) {
            Rectangle source = { 0.0f, 0.0f, (float)Screen1.width, (float)Screen1.height };
            Rectangle dest = { 0.0f, 0.0f, (float)width, (float)height };
            DrawTexturePro(Screen1, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            Grid(gridX, gridY, player_grid2);
        }
        else if (screen == 2) {
            Rectangle source = { 0.0f, 0.0f, (float)Screen2.width, (float)Screen2.height };
            Rectangle dest = { 0.0f, 0.0f, (float)width, (float)height };
            DrawTexturePro(Screen2, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            hide_ship = true;
            Grid(260, 260, player_grid1, hide_ship);
            Grid(1060, 260, player_grid2, hide_ship);
        }
        else if (screen == 3) {
            std::cout << '\a';
            Rectangle source = { 0.0f, 0.0f, (float)Screen3.width, (float)Screen3.height };
            Rectangle dest = { 0.0f, 0.0f, (float)width, (float)height };
            DrawTexturePro(Screen3, source, dest, { 0.0f, 0.0f }, 0.0f, WHITE);
            DrawText("Game Over", 830, 450, 50, WHITE);
            if (winner == 1) {
                DrawText("Player 1 Wins!", 830, 530, 36, WHITE);
            }
            else if (winner == 2) {
                DrawText("Player 2 Wins!", 830, 530, 36, WHITE);
            }

            DrawRectangleRec(restartBtn, hRestart ? SKYBLUE : green);
            DrawRectangleLinesEx(restartBtn, 3.0f, WHITE);
            const char* restartText = "RESTART";
            DrawText(restartText, (int)(restartBtn.x + restartBtn.width / 2 - MeasureText(restartText, 26) / 2), (int)(restartBtn.y + 17), 26, WHITE);

            DrawRectangleRec(gameOverMenuBtn, hGameOverMenu ? SKYBLUE : DARKBLUE);
            DrawRectangleLinesEx(gameOverMenuBtn, 3.0f, WHITE);
            const char* menuText = "MAIN MENU";
            DrawText(menuText, (int)(gameOverMenuBtn.x + gameOverMenuBtn.width / 2 - MeasureText(menuText, 26) / 2), (int)(gameOverMenuBtn.y + 17), 26, WHITE);
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