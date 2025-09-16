#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <string>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "Conway's Game of Life";
	}

public:
	int gridSize = 10;
	int gridWidth = 0;
	int gridHeight = 0;
	int* currentGen = NULL;
	int* nextGen = NULL;
	int generation = 0;

	void setScreenSize(int width, int height) {
		gridWidth = width / gridSize;
		gridHeight = height / gridSize;
		
		if (currentGen != NULL) free(currentGen);
		if (nextGen != NULL) free(nextGen);
		
		int totalCells = gridWidth * gridHeight;
		currentGen = (int*) calloc(totalCells, sizeof(int));
		nextGen = (int*) calloc(totalCells, sizeof(int));
	}
	
	bool isAlive(int x, int y) {
		if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return false;
		return currentGen[y * gridWidth + x] == 1;
	}
	
	void setCell(int x, int y, bool alive) {
		if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return;
		currentGen[y * gridWidth + x] = alive ? 1 : 0;
	}
	
	int countNeighbors(int x, int y) {
		int count = 0;
		for (int dy = -1; dy <= 1; dy++) {
			for (int dx = -1; dx <= 1; dx++) {
				if (dx == 0 && dy == 0) continue;
				if (isAlive(x + dx, y + dy)) count++;
			}
		}
		return count;
	}
	
	void seedRandom() {
		srand(time(NULL));
		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				setCell(x, y, rand() % 4 == 0);
			}
		}
	}
	
	void seedGlider() {
		int cx = gridWidth / 2;
		int cy = gridHeight / 2;
		setCell(cx + 1, cy, true);
		setCell(cx + 2, cy + 1, true);
		setCell(cx, cy + 2, true);
		setCell(cx + 1, cy + 2, true);
		setCell(cx + 2, cy + 2, true);
	}
	
	void seedGosperGliderGun() {
		int cx = gridWidth / 2 - 18;
		int cy = gridHeight / 2 - 5;
		
		// Left block
		setCell(cx, cy + 4, true);
		setCell(cx, cy + 5, true);
		setCell(cx + 1, cy + 4, true);
		setCell(cx + 1, cy + 5, true);
		
		// Left curved part
		setCell(cx + 10, cy + 4, true);
		setCell(cx + 10, cy + 5, true);
		setCell(cx + 10, cy + 6, true);
		setCell(cx + 11, cy + 3, true);
		setCell(cx + 11, cy + 7, true);
		setCell(cx + 12, cy + 2, true);
		setCell(cx + 12, cy + 8, true);
		setCell(cx + 13, cy + 2, true);
		setCell(cx + 13, cy + 8, true);
		setCell(cx + 14, cy + 5, true);
		setCell(cx + 15, cy + 3, true);
		setCell(cx + 15, cy + 7, true);
		setCell(cx + 16, cy + 4, true);
		setCell(cx + 16, cy + 5, true);
		setCell(cx + 16, cy + 6, true);
		setCell(cx + 17, cy + 5, true);
		
		// Right curved part
		setCell(cx + 20, cy + 2, true);
		setCell(cx + 20, cy + 3, true);
		setCell(cx + 20, cy + 4, true);
		setCell(cx + 21, cy + 2, true);
		setCell(cx + 21, cy + 3, true);
		setCell(cx + 21, cy + 4, true);
		setCell(cx + 22, cy + 1, true);
		setCell(cx + 22, cy + 5, true);
		setCell(cx + 24, cy, true);
		setCell(cx + 24, cy + 1, true);
		setCell(cx + 24, cy + 5, true);
		setCell(cx + 24, cy + 6, true);
		
		// Right block
		setCell(cx + 34, cy + 2, true);
		setCell(cx + 34, cy + 3, true);
		setCell(cx + 35, cy + 2, true);
		setCell(cx + 35, cy + 3, true);
	}
	
	void initializeSeed(const char* seedType) {
		// Clear grid first
		for (int i = 0; i < gridWidth * gridHeight; i++) {
			currentGen[i] = 0;
		}
		
		generation = 0;
		
		if (strcmp(seedType, "random") == 0) {
			seedRandom();
		} else if (strcmp(seedType, "glider") == 0) {
			seedGlider();
		} else if (strcmp(seedType, "gun") == 0) {
			seedGosperGliderGun();
		} else {
			// Default to glider
			seedGlider();
		}
		
		updateTitle();
	}
	
	void updateGeneration()
	{
		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				int neighbors = countNeighbors(x, y);
				bool alive = isAlive(x, y);
				
				if (alive) {
					// Live cell rules
					if (neighbors < 2) {
						nextGen[y * gridWidth + x] = 0; // Dies by underpopulation
					} else if (neighbors == 2 || neighbors == 3) {
						nextGen[y * gridWidth + x] = 1; // Survives
					} else {
						nextGen[y * gridWidth + x] = 0; // Dies by overpopulation
					}
				} else {
					// Dead cell rules
					if (neighbors == 3) {
						nextGen[y * gridWidth + x] = 1; // Becomes alive by reproduction
					} else {
						nextGen[y * gridWidth + x] = 0; // Stays dead
					}
				}
			}
		}
		
		// Swap buffers
		int* temp = currentGen;
		currentGen = nextGen;
		nextGen = temp;
		
		generation++;
		updateTitle();
	}
	
	void updateTitle() {
		char title[256];
		snprintf(title, sizeof(title), "Conway's Game of Life - Generation %d", generation);
		sAppName = std::string(title);
	}
	
	void drawGeneration()
	{
		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				if (isAlive(x, y)) {
					FillRect(x * gridSize, y * gridSize, gridSize - 1, gridSize - 1, olc::WHITE);
				}
			}
		}
	}
	
	void drawGrid(int gridSize = 10)
	{
		for (int x = gridSize; x < ScreenWidth(); x += gridSize) {
			DrawLine(x, 0, x, ScreenHeight(), olc::DARK_GREY);
		}
		
		for (int y = gridSize; y < ScreenHeight(); y += gridSize) {
			DrawLine(0, y, ScreenWidth(), y, olc::DARK_GREY);
		}
	}
	
	bool OnUserCreate() override
	{
		updateTitle();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		static float timer = 0.0f;
		timer += fElapsedTime;
		
		// Update generation every 0.1 seconds
		if (timer >= 0.1f) {
			updateGeneration();
			timer = 0.0f;
		}
		
		// Handle user input
		if(GetKey(olc::Key::ESCAPE).bPressed)
		{
			return false; // Exit cleanly
		}
		
		// Erase previous frame
		Clear(olc::BLACK);
		drawGrid();
		drawGeneration();

		return true;
	}
};

int main(int argc, char* argv[])
{
	GameOfLife life;
	
	int width = 1920;
	int height = 1080;
	const char* seedType = "glider";
	
	if (argc >= 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}
	
	if (argc >= 4) {
		seedType = argv[3];
	}
	
	life.setScreenSize(width, height);
	life.sAppName = "Conway's Game of Life - Generation 0";
	if (life.Construct(width, height, 1, 1, false)) {
		life.initializeSeed(seedType);
		life.Start();
	}
	return 0;
}
