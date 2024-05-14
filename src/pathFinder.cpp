
#include "pathFinder.hpp"


PF::PF() {

	//cellShape.setOutlineThickness(-1);
	cellShape.setOutlineColor(sf::Color::Black);
}

void PF::draw(sf::RenderWindow& window) {

	for (int x = 0; x < cellTotalX; x++) {
		for (int y = 0; y < cellTotalY; y++) {
			cellShape.setFillColor(getColor(cell[x][y].status));
			cellShape.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
			window.draw(cellShape);
		}
	}
}

void PF::events(sf::Event& event) {

	if (event.type == sf::Event::KeyPressed) {

		if (event.key.code == sf::Keyboard::R) {
			reset = true;
		}

		else if (event.key.code == sf::Keyboard::E) {
			togglePen = true;
		}
	}

	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			leftClicking = true;
		}

		else if (event.mouseButton.button == sf::Mouse::Right) {
			rightClicked = true;
		}
	}

	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			leftClicking = false;
			mouseOldPos.x = -1;
		}
	}
}


sf::Color PF::getColor(const CellStatus& status) {
	switch (status) {
		case EMPTY: return sf::Color::Black;
		case BLOCKED: return sf::Color::White;
		case CHECKED: return sf::Color::Cyan;
		case PATH: return sf::Color::Yellow;
		case START: return sf::Color::Blue;
		case END: return sf::Color::Red;
	}
}

void PF::resetCells() {

	reset = false;
	started = false;
	pathLoop = false;
	pathDrawing = false;

	for (auto& x : cell) {
		for (auto& y : x) {
			y.status = EMPTY;
		}
	}
}

void PF::togglePenColor() {

	togglePen = false;

	penColor = (penColor) ? EMPTY : BLOCKED;
}


void PF::fillGap(const sf::Vector2i& start, const sf::Vector2i& end) {

	const float h = abs(end.x - start.x);
	const float v = abs(end.y - start.y);

	const float m = (h) ? (v / h) : 0;

	const int length = (h > v) ? h : v;

	sf::Vector2i side;
	side.x = (end.x > start.x) ? 1 : -1;
	side.y = (end.y > start.y) ? 1 : -1;

	for (int n = 1; n < length; n++) {

		const sf::Vector2i pos = (h > v) ? sf::Vector2i(n, n * m) : (m) ? sf::Vector2i(n / m, n) : sf::Vector2i(0, n);

		cell[start.x + pos.x * side.x][start.y + pos.y * side.y].status = penColor;
	}
}

void PF::convertPos(sf::Vector2i& pos) {

	pos.x /= cellSize;
	pos.y /= cellSize;

	if (pos.x < 0) pos.x = 0;
	else if (pos.x >= cellTotalX) pos.x = cellTotalX - 1;

	if (pos.y < 0) pos.y = 0;
	else if (pos.y >= cellTotalX) pos.y = cellTotalY - 1;
}

bool PF::insideBorder(const sf::Vector2i& pos) {
	return !(pos.x < 0 || pos.x >= cellTotalX || pos.y < 0 || pos.y >= cellTotalY);
}

bool PF::walkableCell(const CellStatus& status) {
	return (status == EMPTY || status == END);
}

void PF::pathDraw() {

	parent += cell[parent.x][parent.y].parent;

	if (cell[parent.x][parent.y].status == START) pathDrawing = false;

	else cell[parent.x][parent.y].status = PATH;
}

void PF::pathFinderInit() {

	pathLoop = true;

	for (auto& x : cell) {
		for (auto& y : x) {
			y.val = MAX_VAL;
		}
	}

	cell[parent.x][parent.y].val = 0;
}


void PF::pathFinderLoop() {

	//SET EXPLORED
	if (cell[parent.x][parent.y].status == EMPTY) {

		cell[parent.x][parent.y].status = CHECKED;
	}

	//CHANGE VALS AROUND PARENT
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {

			if (!x && !y) y++;

			const sf::Vector2i sub(parent.x + x, parent.y + y);

			bool walkable = false;

			float newVal;

			//CHECK IF WALKABLE
			if (insideBorder(sub) && walkableCell(cell[sub.x][sub.y].status)) {

				//FLAT
				if (!x || !y) {
					walkable = true;
					newVal = 1;
				}

				//CROSS
				else if (walkableCell(cell[sub.x][parent.y].status) || walkableCell(cell[parent.x][sub.y].status)) {
					walkable = true;
					newVal = 1.41;
				}
			}


			if (walkable) {

				//CHANGE VAL IF LESS
				if (cell[sub.x][sub.y].val > cell[parent.x][parent.y].val + newVal) {

					cell[sub.x][sub.y].val = cell[parent.x][parent.y].val + newVal;

					cell[sub.x][sub.y].parent = sf::Vector2i(-x, -y);
				}
			}
		}
	}
	

	//FIND MIN NEXT PARENT
	float minVal = MAX_VAL;
	sf::Vector2i minPos;

	for (int x = 0; x < cellTotalX; x++) {
		for (int y = 0; y < cellTotalY; y++) {

			if (walkableCell(cell[x][y].status) && cell[x][y].val < minVal) {

				minVal = cell[x][y].val;

				minPos.x = x;
				minPos.y = y;
			}
		}
	}

	if (minVal != MAX_VAL) parent = minPos;

	//NO PATH
	else pathLoop = false;

	//REACHED END
	if (cell[parent.x][parent.y].status == END) {
		pathLoop = false;
		pathDrawing = true;
	}
}

void PF::keyActions() {

	if (reset) resetCells();

	if (togglePen) togglePenColor();
}

void PF::mouseActions(sf::Vector2i& mousePos) {

	convertPos(mousePos);

	if (leftClicking) {
		cell[mousePos.x][mousePos.y].status = penColor;

		if ((abs(mouseOldPos.x - mousePos.x) > 1 || abs(mouseOldPos.y - mousePos.y) > 1) && mouseOldPos.x != -1) {

			fillGap(mouseOldPos, mousePos);
		}
		mouseOldPos = mousePos;
	}

	else if (rightClicked) {

		if (!started) {
			cell[mousePos.x][mousePos.y].status = START;
			parent = mousePos;
		}

		else {
			cell[mousePos.x][mousePos.y].status = END;
			pathFinderInit();
		}

		started = !started;
	}
}




void PF::update(sf::Vector2i mousePos) {

	keyActions();

	if (pathDrawing) pathDraw();

	else if (pathLoop) pathFinderLoop();

	else if (leftClicking || rightClicked) mouseActions(mousePos);

	rightClicked = false;
}