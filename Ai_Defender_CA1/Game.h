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


//! Game Class.
/*!
	This holds all the main logic of the program and pieces all the entities and obejct together.
*/

class Game
{
private:
	Player m_playerShip;
	vector<Astronaut *> m_astronauts;
	vector<Abductor *> m_abductors;
	vector<Nest *> m_nests;
	vector<Mutant *> m_mutants;

	sf::RenderWindow * m_windowScreen;/*!<A Render Window used to render images to the screen. */

	Camera * m_camera;/*!<A Camera that will follow the player Character as he moves through the world. */
	sf::View m_minimapView;/*!<A View port that will be used to create a mini-map for the player to use. */

	sf::Event m_event;

	bool m_isGameRunning;
	int m_numOfScreens;/*!<Variable to hold How large the world will become.*/
	int m_screenWidth;/*!<Variable for how large each screen's width is.*/
	int m_score;
	sf::Clock m_clock;
	sf::Time m_deltatime;
	sf::View *viewport;
	sf::Vector2f m_worldSize;
	std::vector<sf::RectangleShape> m_worldBackground;/*!<container holding a Block for the background*/

public:
	Game();
	Game(sf::RenderWindow &window);/**<Game Constructor*/
	~Game();

	bool isGameRunning() const;
	void getInput();//!<Gets user inputs and processes them into actions.

	void createMutant(Abductor* abductor);//!<Method called when a abductor sucessfully collects a human to spawn a mutant at it's location.

	void manageHumans(sf::Time elapsedTime);//!< Updates astronaut movement, death, being abducted.
	void manageNests(sf::Time elapsedTime);//!< Update nests and spawning new abductors.
	void manageMutants(sf::Time elapsedTime);//!< Updates Mutants seeking after player, death.
	void manageAbductors(sf::Time elapsedTime);//!< Updates abductors patrolling and seeking after astronauts and Abducting.

	bool collisionChecker();//!<Will compare enemy lasers and missiles to see if any hit the player.

	void update();/**< game update loop that runs the game*/
	void cameraWorldWrapping();/**< When player hits end of the world either side teleport to opposite side and anything on screen with him*/
	void warpingEntities();/**< Teleports any entities that goes past the world bounds to the opposite side*/
	void render(sf::RenderWindow &renderer);/**< renders entities also handles drawing the mini-map */
	void killAllPowerUp();

	bool playerHit();

};

