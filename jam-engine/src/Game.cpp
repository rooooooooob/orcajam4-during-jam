#include "Game.hpp"

#include "Level.hpp"
#include "TexManager.hpp"

namespace je
{

Game::Game(int width, int height, int framerate)
	:window(sf::VideoMode(width, height), "")
	,view(sf::Vector2f(width / 2, height / 2), sf::Vector2f(width, height))
	,level(nullptr)
	,input(window)
{
	window.setFramerateLimit(framerate);
}

Game::~Game()
{
	if (level)
		delete level;
}

int Game::execute()
{
	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
        }

        window.clear();

		input.update();

		if (level)
		{
			level->update();

			view.setCenter(level->getCameraPosition());

			window.setView(view);

			level->draw(window);

			window.setView(window.getDefaultView());

			level->drawGUI(window);
		}

        window.display();
    }

    return 0;
}

void Game::setLevel(Level *level)
{
	if (this->level)
		delete this->level;
	this->level = level;
}

void Game::setTitle(const std::string& title)
{
	this->title = title;
	window.setTitle(title);
}

Input& Game::getInput()
{
	return input;
}

TexManager& Game::getTexManager()
{
	return texMan;
}

sf::RenderWindow& Game::getWindow()
{
	return window;
}

}
