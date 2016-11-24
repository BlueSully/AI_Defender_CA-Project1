#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetupCamera()
{
}

void Camera::setTargetPlayer(Player * player)
{
}

void Camera::UpdateView()
{
	//if(m_targetPlayer->)
}

sf::View Camera::getView() const
{
	return sf::View();
}

//void GameScene::updateView(sf::Time elapsedTime)
//{
//	if (player.getPos().x > camera.getCenter().x && !cameraLocked)
//	{
//		camera.setCenter(camera.getCenter() + sf::Vector2f(125, 0) * elapsedTime.asSeconds());
//	}
//	if (player.getPos().y < camera.getCenter().y && !cameraLocked)
//	{
//		camera.setCenter(camera.getCenter() - sf::Vector2f(0, 125) * elapsedTime.asSeconds());
//	}
//	else if (player.getPos().y > camera.getCenter().y + 100 && camera.getCenter().y < camera.getSize().y / 2)
//	{
//		camera.setCenter(camera.getCenter() + sf::Vector2f(0, 125) * elapsedTime.asSeconds());
//	}
//	else if (camera.getCenter().y > camera.getSize().y / 2)
//	{
//		camera.setCenter(camera.getCenter().x, camera.getSize().y / 2);
//	}
//}
