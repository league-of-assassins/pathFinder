#include "main.hpp"



///////////////////// ESSENTIALS /////////////////////



void pathFinder::setWindow() {
	window.create(VideoMode(width, height), "pathFinder", Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}



void pathFinder::frames() {
	frame++;
	if (frame == 10000) frame = 0;
}



void pathFinder::general() {

	if (pathLoop) {
		if (frame % 1 == 0) pathFinderLoop();
	}

	else {
		if (reset) resetColors();

		mouseLogic();
	}

	frames();
}



pathFinder::pathFinder() {
	objects();
	setWindow();


	while (window.isOpen())
	{
		events();

		general();

		displays();
	}
}



void pathFinder::displays() {

	window.clear();


	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			window.draw(cube[i][j]);
		}
	}


	window.display();
}



void pathFinder::events() {
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) window.close();


		else if (event.type == Event::KeyPressed) {

			if (event.key.code == Keyboard::Escape) {
				window.close();
			}

			//RESET BOARD
			else if (event.key.code == Keyboard::R) {
				reset = true;
			}

			//CHANGE PEN COLOR
			else if (event.key.code == Keyboard::E) {
				if (penColor == cubeColor) {
					penColor = Color::White;
				}

				else {
					penColor = cubeColor;
				}
			}
		}


		else if (event.type == Event::MouseButtonPressed) {
			if (event.key.code == Mouse::Left) {
				leftClicked = true;
				clicked = true;
			}

			else if (event.key.code == Mouse::Right) {
				rightClicked = true;
				clicked = true;
			}
		}


		else if (event.type == Event::MouseButtonReleased) {
			if (event.key.code == Mouse::Left) {
				leftClicked = false;
				clicked = false;
				mouseOldPos.x = -1;
			}
		}
	}
}



void pathFinder::objects() {

	mouseOldPos.x = -1;

	penColor = Color::White;

	cubeColor = Color::Black;

	pathStartColor = Color::Blue;
	pathEndColor = Color::Red;

	exploredColor = Color::Cyan;

	pathColor = Color::Magenta;


	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			cube[i][j].setFillColor(cubeColor);
			cube[i][j].setSize(Vector2f(cubeSize, cubeSize));
			cube[i][j].setPosition(Vector2f(i * cubeSize, j * cubeSize));
		}
	}
}



///////////////////// PATH FINDING /////////////////////



void pathFinder::resetColors() {
	reset = false;
	pathStartMarked = false;

	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			cube[i][j].setFillColor(cubeColor);
		}
	}
}



void pathFinder::drawLine(Vector2i start, Vector2i end) {

	Vector2i pos;

	float h = abs(start.x - end.x);
	float v = abs(start.y - end.y);

	float m = v / h;


	Vector2i side(1, 1);

	if (end.x < start.x) side.x = -1;
	if (end.y < start.y) side.y = -1;


	bool xTurn = false;
	int length = v;

	if (h > v) {
		xTurn = true;
		length = h;
	}

	int n = 1;
	while (n != length && length > 1) {

		if (xTurn) {
			pos.x = n;
			pos.y = pos.x * m;
		}

		else {
			pos.y = n;
			pos.x = pos.y / m;
		}

		pos.x *= side.x;
		pos.y *= side.y;

		cube[start.x + pos.x][start.y + pos.y].setFillColor(penColor);

		n++;
	}
}



void pathFinder::pathDraw(Vector2i pos) {

	Vector2i temp;

	bool stop = false;
	while (!stop) {

		temp.x = parentMarker[pos.x][pos.y][0];
		temp.y = parentMarker[pos.x][pos.y][1];

		pos += temp;

		if (pos == pathStart) {

			stop = true;
			break;
		}

		cube[pos.x][pos.y].setFillColor(pathColor);
	}
}



bool pathFinder::insideBorder(int x, int y) {

	if (x >= 0 && x < cubeLength &&
		y >= 0 && y < cubeLength) {

		return true;
	}

	return false;
}



bool pathFinder::walkableColor(int x, int y) {

	if (cube[x][y].getFillColor() == cubeColor ||
		cube[x][y].getFillColor() == pathEndColor) {

		return true;
	}

	return false;
}



void pathFinder::pathFinderLoop() {

	Vector2i sub;

	float addVal = 1;

	bool walkable = false;


	//SET EXPLORED
	if (cube[parent.x][parent.y].getFillColor() == cubeColor) {

		cube[parent.x][parent.y].setFillColor(exploredColor);
	}

	//CHANGE VALS AROUND PARENT
	for (int i = 0; i < sideTotal; i++) {

		sub.x = parent.x + side[i][0];
		sub.y = parent.y + side[i][1];


		//CHECK IF WALKABLE
		if (insideBorder(sub.x, sub.y) &&
			walkableColor(sub.x, sub.y)) {

			if (i < 4) walkable = true;

			else {
				addVal = 1.41;


				if (walkableColor(sub.x, parent.y) ||
					walkableColor(parent.x, sub.y)) {

					walkable = true;
				}
			}
		}


		if (walkable) {

			walkable = false;


			//CHANGE VAL IF LESS
			if (pathVal[sub.x][sub.y][0] > pathVal[parent.x][parent.y][0] + addVal) {

				pathVal[sub.x][sub.y][0] = pathVal[parent.x][parent.y][0] + addVal;

				parentMarker[sub.x][sub.y][0] = -side[i][0];
				parentMarker[sub.x][sub.y][1] = -side[i][1];
			}
		}
	}

	//FIND MIN NEXT PARENT
	float minVal = 99999;
	Vector2i minPos;

	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {

			if (walkableColor(i, j) &&
				pathVal[i][j][0] < minVal) {

				minVal = pathVal[i][j][0];

				minPos.x = i;
				minPos.y = j;
			}
		}
	}

	//IF NO PATH
	if (minVal == 99999) {

		pathLoop = false;
		cout << "\n NO PATH \n";
	}

	else {
		parent = minPos;
	}


	//IF END
	if (parent == pathEnd) {

		pathLoop = false;

		pathDraw(parent);
	}
}



void pathFinder::pathFinderInit() {

	pathLoop = true;

	parent = pathStart;

	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			pathVal[i][j][0] = 99999;
		}
	}

	pathVal[pathStart.x][pathStart.y][0] = 0;
}



void pathFinder::leftMouseLogic() {


	cube[mousePos.x][mousePos.y].setFillColor(penColor);


	//IF DRAW GAP
	if ((abs(mouseOldPos.x - mousePos.x) > 1 || abs(mouseOldPos.y - mousePos.y) > 1) && mouseOldPos.x != -1) {

		drawLine(mouseOldPos, mousePos);
	}

	mouseOldPos = mousePos;
}



void pathFinder::rightMouseLogic() {

	rightClicked = false;
	clicked = false;

	if (!pathStartMarked) {

		pathStartMarked = true;

		cube[mousePos.x][mousePos.y].setFillColor(pathStartColor);

		pathStart = mousePos;
	}

	else {

		pathStartMarked = false;

		cube[mousePos.x][mousePos.y].setFillColor(pathEndColor);

		pathEnd = mousePos;

		pathFinderInit();
	}
}



void pathFinder::mouseLogic() {

	if (clicked) {

		mousePos.x = (Mouse::getPosition().x - windowGap.x) / cubeSize;
		mousePos.y = (Mouse::getPosition().y - windowGap.y) / cubeSize;


	if (insideBorder(mousePos.x, mousePos.y)) {

			if (leftClicked) {
				leftMouseLogic();
			}


			else if (rightClicked) {
				rightMouseLogic();
			}
		}
	}
}




int main() {

	pathFinder pathFinder;

	return 0;
}
