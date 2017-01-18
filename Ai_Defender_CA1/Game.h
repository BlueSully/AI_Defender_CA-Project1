#pragma once
#include "Player.h"
#include "Astronaut.h"
#include "Mutant.h"
#include "Camera.h"
#include "Abductor.h"
#include "Nest.h"
#include <vector>
#include "MathFunction.h"


using namespace std;

class Game
{
private:
	Player m_playerShip;
	vector<Astronaut *> m_astronauts;
	vector<Abductor *> m_abductors;
	vector<Nest *> m_nests;
	vector<Mutant *> m_mutants;

	sf::RenderWindow * m_windowScreen;

	sf::RectangleShape screenRect;
	Camera * m_camera;
	sf::View m_minimapView;

	sf::Event m_event;
	float debugTime;
	bool m_isGameRunning;
	int m_numOfScreens;
	int m_screenWidth;
	int m_score;
	sf::Clock m_clock;
	sf::Time m_deltatime;
	sf::View *viewport;
	sf::Vector2f m_worldSize;
	std::vector<sf::RectangleShape> m_worldBackground;

public:
	Game();
	Game(sf::RenderWindow &window);
	~Game();

	//Get Methods and Set methods
	bool isGameRunning() const;
	void getInput();

	void createMutant(Abductor* abductor);

	void manageHumans(sf::Time elapsedTime);
	void manageNests(sf::Time elapsedTime);
	void manageMutants(sf::Time elapsedTime);
	void manageAbductors(sf::Time elapsedTime);

	bool collisionChecker();

	//Methods
	void update();
	void cameraWorldWrapping();
	void render(sf::RenderWindow &renderer);
};

