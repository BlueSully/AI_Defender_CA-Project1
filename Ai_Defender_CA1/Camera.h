#pragma once
#include "Player.h"
class Camera
{
private:
	Player m_targetPlayer;
public:
	Camera();
	~Camera();

	void SetupCamera();
	void setTargetPlayer(Player * player);
	void UpdateView();
	sf::View getView() const;
};

