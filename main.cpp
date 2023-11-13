#pragma once

#include "main.hpp"


void pathFinder::setWindow() {
	window.create(VideoMode(width, height), "pathFinder", Style::None);
	window.setFramerateLimit(60);

	windowGap.x = window.getPosition().x;
	windowGap.y = window.getPosition().y;
}

void pathFinder::objects() {

	penColor = Color::Black;

	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			cube[i][j].setFillColor(Color::White);
			cube[i][j].setSize(Vector2f(cubeSize, cubeSize));
			cube[i][j].setPosition(Vector2f(i * cubeSize, j * cubeSize));
			cube[i][j].setOutlineThickness(-1);
			cube[i][j].setOutlineColor(Color::Black);
		}
	}
}

void pathFinder::events() {
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed) window.close();


		else if (event.type == Event::KeyPressed) {

			if (event.key.code == Keyboard::Escape) {
				window.close();
			}

			else if (event.key.code == Keyboard::R) {
				reset = true;
			}

			//CHANGE PEN COLOR IF PRESSED E
			else if (event.key.code == Keyboard::E) {
				if (penColor == Color::Black) {
					penColor = Color::White;
				}

				else {
					penColor = Color::Black;
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
				oldX = -1;
			}
		}
	}
}

void pathFinder::resetColors() {
	reset = false;
	rightClickCount = 0;

	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			cube[i][j].setFillColor(Color::White);
		}
	}
}

void pathFinder::drawLine() {

	int currentX = 0, currentY = 0;

	int xStart = oldX;
	int yStart = oldY;
	int xEnd = clickedX;
	int yEnd = clickedY;


	float h = abs(xEnd - xStart);
	float v = abs(yEnd - yStart);

	int xSide = 1;
	int ySide = 1;

	if (xEnd - xStart < 0) xSide = -1;
	if (yEnd - yStart < 0) ySide = -1;


	float m = v / h;

	bool xTurn = false;
	int length = v;

	if (h > v) {
		xTurn = true;
		length = h;
	}

	int n = 1;
	while (n != length && length > 1) {
		if (xTurn) {
			currentX = n;
			currentY = currentX * m;
		}

		else {
			currentY = n;
			currentX = currentY / m;
		}

		currentX *= xSide;
		currentY *= ySide;

		cube[xStart + currentX][yStart + currentY].setFillColor(penColor);

		n++;
	}
}

void pathFinder::pathDraw(int currentX, int currentY) {

	int tempX, tempY;

	bool stop = false;
	while (!stop) {

		cube[currentX][currentY].setFillColor(Color::Yellow);

		if ((currentX - 1 <= xStart && currentX + 1 >= xStart) &&
			(currentY - 1 <= yStart && currentY + 1 >= yStart)) {



			stop = true;
			break;
		}

		tempX = parentMarker[currentX][currentY][0];
		tempY = parentMarker[currentX][currentY][1];
		currentX += tempX;
		currentY += tempY;
	}
}

void pathFinder::pathFinderLoop(int& parentX, int& parentY, bool& stop) {
	int subX, subY;
	float addVal = 1;
	bool walkable = false;

	//SET EXPLORED
	if (cube[parentX][parentY].getFillColor() != Color::Blue) {
		cube[parentX][parentY].setFillColor(Color::Cyan);
	}

	//CHANGE VALS AROUND PARENT
	for (int j = 0; j < sideTotal; j++) {

		subX = parentX + side[j][0];
		subY = parentY + side[j][1];


		//CHECK IF WALKABLE
		if ((subX >= 0 && subX < cubeLength && subY >= 0 && subY < cubeLength) &&
			cube[subX][subY].getFillColor() == Color::White) {

			if (j < 4) walkable = true;

			else {
				addVal = 1.41;

				if (cube[parentX + side[j][0]][parentY].getFillColor() == Color::White ||
					cube[parentX][parentY + side[j][1]].getFillColor() == Color::White) {

					walkable = true;
				}
			}
		}


		if (walkable) {
			walkable = false;


			//CHANGE VAL IF LESS
			if (pathVal[subX][subY][0] > pathVal[parentX][parentY][0] + addVal) {

				pathVal[subX][subY][0] = pathVal[parentX][parentY][0] + addVal;

				parentMarker[subX][subY][0] = -side[j][0];
				parentMarker[subX][subY][1] = -side[j][1];
			}


			//IF END
			if ((subX - 1 <= xEnd && subX + 1 >= xEnd) &&
				(subY - 1 <= yEnd && subY + 1 >= yEnd)) {
				pathDraw(subX, subY);

				stop = true;
				j = sideTotal - 1;
				break;
			}
		}
	}

	//FIND MIN NEXT PARENT
	float min = 99999;
	int minI = 0, minJ = 0;
	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			if (cube[i][j].getFillColor() == Color::White) {
				if (min > pathVal[i][j][0]) {
					min = pathVal[i][j][0];
					minI = i;
					minJ = j;
				}
			}
		}
	}



	//IF NO PATH
	if (parentX == minI && parentY == minJ) {
		stop = true;
		cout << "\n NO PATH \n";
	}

	else {
		parentX = minI;
		parentY = minJ;
	}
}

void pathFinder::pathFinderInit() {

	int parentX = xStart;
	int parentY = yStart;

	//RESET VALUE
	for (int i = 0; i < cubeLength; i++) {
		for (int j = 0; j < cubeLength; j++) {
			pathVal[i][j][0] = 99999;
		}
	}
	pathVal[xStart][yStart][0] = 0;

	bool stop = false;
	while (!stop) {
		if (frames % 1 == 0) {
			pathFinderLoop(parentX, parentY, stop);
		}

		displays();

		frames++;
		if (frames == 10000) { frames = 0; }

	}
}

void pathFinder::mouseClick() {

	if (clicked) {
		clickedX = (Mouse::getPosition().x - windowGap.x) / cubeSize;
		clickedY = (Mouse::getPosition().y - windowGap.y) / cubeSize;

		cout << "\n x: " << clickedX << " y: " << clickedY << "\n";

		if (clickedX >= 0 && clickedX < cubeLength && clickedY >= 0 && clickedY < cubeLength) {

			if (leftClicked) {

				cube[clickedX][clickedY].setFillColor(penColor);

				//IF DRAW GAP
				if ((abs(oldX - clickedX) > 1 || abs(oldY - clickedY) > 1) && oldX != -1) {

					drawLine();
				}

				oldX = clickedX;
				oldY = clickedY;
			}


			else if (rightClicked) {
				rightClicked = false;
				clicked = false;

				if (rightClickCount == 0) {
					cube[clickedX][clickedY].setFillColor(Color::Blue);

					xStart = clickedX;
					yStart = clickedY;

					rightClickCount++;
				}

				else {
					cube[clickedX][clickedY].setFillColor(Color::Red);

					xEnd = clickedX;
					yEnd = clickedY;

					rightClickCount--;

					pathFinderInit();
				}
			}
		}
	}
}

pathFinder::pathFinder() {
	objects();
	setWindow();


	while (window.isOpen())
	{
		events();

		if (reset) resetColors();

		mouseClick();

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




int main()
{

	pathFinder pf;

	return 0;
}