#include "Game.h"


Game::Game()
{

}

Game::~Game()
{

}

void Game::update()
{
	//sf::Event event; // For Keyboard Controls
}

void Game::render(sf::RenderWindow &renderer)
{
	sf::CircleShape c1(100);
	c1.setFillColor(sf::Color(0, 255, 0, 255));
	c1.setPosition(50, 50);
	renderer.clear(sf::Color(0, 0, 0, 255));
	renderer.draw(c1);
	renderer.display();
}
