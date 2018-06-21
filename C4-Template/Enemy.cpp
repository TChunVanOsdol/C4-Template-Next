#include "Enemy.h"



Enemy::Enemy(sf::Vector2f pos, sf::Vector2f size) : Character(pos, size)
{
	playershape.setFillColor(sf::Color::Red);
	speed = 240;
}

void Enemy::update(float dt)
{
	// AI code to set up movement input vector
	movementInput.x = 0;
	movementInput.y = 0;

	if (followTarget != NULL) {
		//Direct line to target moved to else
		sf::Vector2f dir;
		sf::Vector2f pixelPos;
		//dir = followTarget->position - position;
		//Search algorithm for pathfinding
		gridcoord enemy_location = std::make_pair(int(position.x) + 1, int(position.y) + 1);
		gridcoord astar_target = std::make_pair(int(followTarget->position.x), int(followTarget->position.y));
		pathfinder = mapref->astar(enemy_location, astar_target);
		if (pathfinder.size() > 1) {
			pixelPos = mapref->getPixelPos(pathfinder[1].first, pathfinder[1].second);
			dir = sf::Vector2f(pixelPos.x - int(enemy_location.first), pixelPos.y - int(enemy_location.second));
		}
		//Fix the above equation so that pathfinder and enemylocation match grid spaces (tile vs pixel)
		else {
			dir = followTarget->position - position;
		}
		
		movementInput = dir;
	}

	// Normalize vector
	float mag = std::sqrt(std::pow(movementInput.x, 2) + std::pow(movementInput.y, 2));
	if (mag != 0) {
		movementInput /= mag;
	}
	//State Machine for Alert State
	if (mag > 100 && alert_state == 1) {
		alert_state = 0;
	}
	else if (mag <= 100 && alert_state == 0) {
		alert_state = 1;
	}
	if (alert_state == 0) { //Idle if alert state is 0
		//movementInput = sf::Vector2f(0, 0);
	}
	
	
	// Character update applies movement input and checks collisions
	Character::update(dt);
}
