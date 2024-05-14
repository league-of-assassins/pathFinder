
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "pathFinder.hpp"


int main() { 

	//--INITS--//
	sf::RenderWindow window(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "App", sf::Style::None));
	window.setFramerateLimit(FPS);


	std::unique_ptr<PF> pathFinder(new PF);


	bool paused = false;


	//--WINDOW LOOP--//

	while (window.isOpen())
	{
		//EVENTS
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();

			else if (event.type == sf::Event::KeyPressed) {

				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}

				else if (event.key.code == sf::Keyboard::P) {
					paused = !paused;
				}
			}

			pathFinder->events(event);
		}
		

		//UPDATES
		if (!paused) pathFinder->update(sf::Mouse::getPosition(window));


		//DISPLAY
		window.clear(sf::Color::Black);

		pathFinder->draw(window);

		window.display();
	}
}