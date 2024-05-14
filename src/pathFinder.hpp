
//pathFinder.hpp
#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

#include "common.hpp"



class PF {
private:

	static constexpr int cellSize = 16;

	static constexpr int cellTotalX = WINDOW_WIDTH / cellSize;
	static constexpr int cellTotalY = WINDOW_HEIGHT / cellSize;

	static constexpr int MAX_VAL = 999999;


	enum CellStatus {
		EMPTY,
		BLOCKED,
		CHECKED,
		PATH,
		START,
		END
	};


	struct Cell {
		CellStatus status = EMPTY;
		float val;
		sf::Vector2i parent;
	};

	Cell cell[cellTotalX][cellTotalY];

	sf::RectangleShape cellShape = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));



	sf::Vector2i parent;

	CellStatus penColor = BLOCKED;

	sf::Vector2i mouseOldPos = sf::Vector2i(-1, -1);



	bool leftClicking = false, rightClicked = false;

	bool reset = false, togglePen = false;

	bool started = false, pathLoop = false, pathDrawing = false;



public:

	PF();

	void update(sf::Vector2i mousePos);

	void draw(sf::RenderWindow& window);

	void events(sf::Event& event);

private:

	sf::Color getColor(const CellStatus& status);

	void resetCells();

	void togglePenColor();

	void fillGap(const sf::Vector2i& start, const sf::Vector2i& end);

	void convertPos(sf::Vector2i& mousePos);

	bool insideBorder(const sf::Vector2i& pos);

	bool walkableCell(const CellStatus& status);

	void pathDraw();

	void pathFinderInit();

	void pathFinderLoop();

	void keyActions();

	void mouseActions(sf::Vector2i& mousePos);
};


#endif // PATHFINDER_HPP

