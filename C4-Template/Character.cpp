#include "Character.h"

Character::Character() {}

Character::Character(sf::Vector2f pos, sf::Vector2f size) : Actor(pos, size)
{
	playershape = sf::CircleShape(size.x / 2);
	playershape.setPosition(pos);
	playershape.setFillColor(sf::Color::Blue);
}

void Character::update(float dt)
{
	auto current_pos = position;
	position += (movementInput * speed * dt);
	bool collide = false;
	sf::Vector2f wall_dir = { 0, 0 };
	
	// Test tile collision
	if (mapref) {
		collide = mapref->checkTileCollision(*this);
		wall_dir = mapref->wallLocation(*this);
	
		// Test out of bounds
		/*auto mapbounds = sf::FloatRect(sf::Vector2f(0, 0), mapref->getPixelPos(mapref->mapWidth, mapref->mapHeight));
		auto actorbounds = sf::FloatRect(position, size);

		mapbounds.*/

	}

	if (collide) {
		// Don't move if we hit something.
		//position = current_pos;
		
	}
	position -= wall_dir * speed * dt;
	//if (wall_dir.x != 0 && wall_dir.y != 0) {}
	
}

void Character::draw(sf::RenderWindow & window)
{
	playershape.setPosition(position);
	window.draw(playershape);

}
