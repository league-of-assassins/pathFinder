
//app.hpp
#ifndef APP_HPP
#define APP_HPP

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "pathFinder.hpp"


class App {
private:

	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "App", sf::Style::None);


	bool paused = false;


	std::unique_ptr<PF> pf = std::make_unique<PF>();



	void events();

	void displays();

public:

	App();
};


#endif //APP_HPP