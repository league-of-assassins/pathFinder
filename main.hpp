#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class pathFinder {
private:

	float width = 800, height = 800;

	Vector2f windowGap;

	RenderWindow window;

	Event event;

	Color penColor;


	RectangleShape cube[50][50];

	int cubeLength = 50;
	int cubeSize = height / cubeLength;


	int rightClickCount = 0;

	int parentMarker[50][50][2] = {};
	float pathVal[50][50][1] = {};
	int side[8][2] = {
		{0, -1},
		{1, 0},
		{0, 1},
		{-1, 0},
		{-1, -1},
		{1, -1},
		{-1, 1},
		{1, 1}
	};
	int sideTotal = 8;

	int clickedX = 0, clickedY = 0;
	int oldX = -1, oldY;
	int xStart, xEnd, yStart, yEnd;
	bool leftClicked = false, rightClicked = false, clicked = false;
	bool reset = false;

	int frames = 0;

public:

	void setWindow();

	void objects();

	void events();

	void resetColors();

	void drawLine();

	void pathDraw(int currentX, int currentY);

	void pathFinderLoop(int parentX, int parentY, bool& stop);

	void pathFinderInit();

	void mouseClick();

	void displays();

	pathFinder();
};