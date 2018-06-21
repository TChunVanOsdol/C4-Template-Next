#pragma once
#include "Actor.h"
#include "Map.h"

class Buff;

/*
	A character that takes movement input. Base of Players and Enemies
*/
class Character :
	public Actor
{
public:

	sf::Vector2f movementInput;
	float speed = 480;

	GameMap* mapref = NULL;

	sf::CircleShape playershape;

	std::vector<Buff*> buff_list;

	Character();
	Character(sf::Vector2f pos, sf::Vector2f size);

	virtual void update(float dt) override;

	virtual void draw(sf::RenderWindow& window) override;

};

