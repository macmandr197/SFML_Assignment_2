#pragma region step 2
#include <Book/Player.hpp>
#include <Book/CommandQueue.hpp>
#include <map>
#include <string>
#include <algorithm>
#include <Book/Doodle.hpp>
#include <iostream>


struct DoodleMover
{
	DoodleMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Doodle& doodle, sf::Time) const
	{
		doodle.setVelocity(velocity);
		//test
	}
	
		

	sf::Vector2f velocity;
};

Player::Player()
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::A] = MoveLeft;

	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::Up] = MoveUp;
	mKeyBinding[sf::Keyboard::W] = MoveUp;

	mKeyBinding[sf::Keyboard::Down] = MoveDown;
	mKeyBinding[sf::Keyboard::S] = MoveDown;


	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's aircraft
	for(auto& pair : mActionBinding)
		pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
	
	
}

void Player::handleRealtimeInput(CommandQueue& commands)
{

	// Traverse all assigned keys and check if they are pressed
	for(auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);		
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{

	for(auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;

	mActionBinding[MoveLeft].action = derivedAction<Doodle>(DoodleMover(-playerSpeed, 0.f));
	mActionBinding[MoveRight].action = derivedAction<Doodle>(DoodleMover(+playerSpeed, 0.f));


	mActionBinding[MoveUp].action = derivedAction<Doodle>(DoodleMover(0.f, -playerSpeed));
	mActionBinding[MoveDown].action = derivedAction<Doodle>(DoodleMover(0.f, +playerSpeed));

}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case MoveDown:
	case MoveUp:
		return true;

	default:

		return false;
	}
}

#pragma endregion
