/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <iostream>

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
const int screenWidth = 800;
const int screenHeight = 600;
const int cellWidth = 1;                       // length of each partical in a square shape
#define ROWS screenHeight / cellWidth          // number of rows
#define COLS screenWidth / cellWidth           // number of columns
int** grid = nullptr;                          // dynamic array representing the particals
static int numOfParticals = 0;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);              // general update and draw function
static void DrawGrid(int** grid);               // draw the grid
static void UpdateGrid(int** grid);             // update the pyhsics of the partical
int** initGrid(int rows, int cols);             // initialize an empty grid
int hue = 1;                                    // color value of each partical in hsv

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	grid = initGrid(ROWS, COLS);
	InitWindow(screenWidth, screenHeight, "raylib");

	//--------------------------------------------------------------------------------------
	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateDrawFrame();
	}


	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();                  // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
	// Update
	//----------------------------------------------------------------------------------
	UpdateGrid(grid);
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 mousePos = GetMousePosition();
		// check if the mouse is within the frame
		if (mousePos.x < screenWidth && mousePos.x >= 0
			&& mousePos.y >= 0 && mousePos.y < screenHeight) {
			int x = mousePos.x / cellWidth;
			int y = mousePos.y / cellWidth;
			// add the particals in a s by s square box centered at the mouse cursor
			int s = 3;
			for (int i = x - s; i < x + s; ++i) {
				for (int j = y - s; j < y + s; ++j) {
					// add another partical with 50% chance, this can be changed based on @param cellWidth
					if (i >= 0 && i < COLS && j >= 0 && j < ROWS && GetRandomValue(0, 1)) {
						grid[j][i] = hue;
						hue += 1;
						numOfParticals += 1;
					}
				}
			}
		}
	}
	else if (IsKeyDown(KEY_R)) {
		grid = initGrid(ROWS, COLS);
		numOfParticals = 0;
	}
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------
	BeginDrawing();

	ClearBackground({ 0, 0, 0, 1 });
	DrawGrid(grid);
	char numParticals[10];
	itoa(numOfParticals, numParticals, 10);
	DrawText(std::string("Number of Particals: ").append(numParticals).c_str(), 10, 10, 20, { 255, 255, 255, 255 });
	DrawText("(R for reset!)", 10, 50, 20, { 255, 255, 255, 255 });
	DrawFPS(10, 30);

	EndDrawing();
	//----------------------------------------------------------------------------------
}
//TODO: instead of checking for every cell, keep track of each movind partical position on the grid
//TODO: maybe add the gravitational acceleration to the particals
//----------------------------------------------------------------------------------
// Update the partical physics
//----------------------------------------------------------------------------------
static void UpdateGrid(int** grid)
{
	for (int y = ROWS - 2; y >= 0; --y) {
		for (int x = COLS - 1; x >= 0; --x) {
			int state = grid[y][x];
			// meaning there is a partical at that cell
			if (state != 0) {
				int below = grid[y + 1][x];
				// randomly decide whether the partical needs to go right or left
				int r = GetRandomValue(0, 1) ? -1 : 1;
				int belowA = grid[y + 1][x + r];
				int belowB = grid[y + 1][x - r];
				// check below, below left and below right of that partical to see if it needs to go down anymore
				if (below == 0) {
					grid[y][x] = 0;
					grid[y + 1][x] = state;
				}
				else if (belowA == 0) {
					grid[y][x] = 0;
					grid[y + 1][x + r] = state;
				}
				else if (belowB == 0) {
					grid[y][x] = 0;
					grid[y + 1][x - r] = state;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
// Draw the particals
//----------------------------------------------------------------------------------
static void DrawGrid(int** grid)
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			int state = grid[i][j];
			if (state != 0)
				DrawRectangle(j * cellWidth, i * cellWidth, cellWidth, cellWidth, ColorFromHSV(state, 1.0f, 1.0f));
		}
	}
}

int** initGrid(int rows, int cols)
{
	int** newGrid = new int* [rows];
	for (int i = 0; i < rows; ++i)
		newGrid[i] = new int[cols] {0};

	return newGrid;
}