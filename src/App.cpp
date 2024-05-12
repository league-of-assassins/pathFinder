
#include "App.hpp"




void App::events() {
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

		pf->events(event);
	}
}


void App::displays() {

	window.clear(sf::Color::Black);

	pf->draw(window);

	window.display();
}



App::App() {

	//--INITS--//
	window.setFramerateLimit(FPS);

	//--WINDOW LOOP--//

	while (window.isOpen())
	{
		events();

		if (!paused) pf->logic(sf::Mouse::getPosition(window));


		displays();
	}
}