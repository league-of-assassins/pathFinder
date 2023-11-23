#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;


const int cubeLength = 50;


class pathFinder {
private:

	float width = 800, height = 800;

	RenderWindow window;

	Vector2i windowGap;

	int frame = 0;

	Event event;



	RectangleShape cube[cubeLength][cubeLength];
	int cubeSize = height / cubeLength;

	Color cubeColor;



	int parentMarker[cubeLength][cubeLength][2] = {};
	float pathVal[cubeLength][cubeLength][1] = {};

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

	Vector2i parent;

	Vector2i pathStart, pathEnd;

	Color pathStartColor, pathEndColor;

	bool pathStartMarked = false;

	Color pathColor;
	Color exploredColor;

	bool pathLoop = false;



	Vector2i mousePos;
	Vector2i mouseOldPos;

	Color penColor;

	bool leftClicked = false, rightClicked = false, clicked = false;
	bool reset = false;

public:

	void setWindow();

	void objects();

	void events();

	void resetColors();

	void drawLine(Vector2i start, Vector2i end);

	void pathDraw(Vector2i pos);

	bool insideBorder(int x, int y);

	bool walkableColor(int x, int y);

	void pathFinderLoop();

	void pathFinderInit();

	void leftMouseLogic();

	void rightMouseLogic();

	void mouseLogic();

	void general();

	void displays();

	void frames();

	pathFinder();
};
