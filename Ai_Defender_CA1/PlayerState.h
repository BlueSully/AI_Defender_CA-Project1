#pragma once

//! PlayerStates Class.
/*!
	An enum class that will hold the actions that the player can do within the game
*/
enum PlayerStates 
{
	IDLE,
	MOVING_LEFT,
	MOVING_RIGHT,
	MOVING_UP,
	MOVING_DOWN,
	WARPING
};