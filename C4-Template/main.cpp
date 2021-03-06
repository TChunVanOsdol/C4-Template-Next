#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Character.h"
#include "Enemy.h"
#include "Buff.h"

int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	GameMap map = GameMap(window, 20, 20);

	// Controls
	sf::Keyboard::Key move_left = sf::Keyboard::Key::A;
	sf::Keyboard::Key move_right = sf::Keyboard::Key::D;
	sf::Keyboard::Key move_up = sf::Keyboard::Key::W;
	sf::Keyboard::Key move_down = sf::Keyboard::Key::S;

	bool move_left_pressed = false;
	bool move_right_pressed = false;
	bool move_up_pressed = false;
	bool move_down_pressed = false;

	sf::Vector2f input(0, 0);

	Character player(map.getPixelPos(1, 18), sf::Vector2f(map.tileSize, map.tileSize));
	player.mapref = &map;

	Enemy enemy(map.getPixelPos(5, 5), sf::Vector2f(map.tileSize, map.tileSize));
	enemy.mapref = &map;
	enemy.followTarget = &player;
	//Enemy two
	Enemy enemy2(map.getPixelPos(15, 15), sf::Vector2f(map.tileSize, map.tileSize));
	enemy2.mapref = &map;
	enemy2.followTarget = &player;
	enemy2.speed = 160;

	std::vector<Buff> all_buffs;

	sf::Clock dtClock;

	while (window.isOpen())
	{
		sf::Time dt = dtClock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {

				if (event.key.code == move_left) {
					move_left_pressed = true;
				}
				if (event.key.code == move_right) {
					move_right_pressed = true;
				}
				if (event.key.code == move_up) {
					move_up_pressed = true;
				}
				if (event.key.code == move_down) {
					move_down_pressed = true;
				}

				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}

			}
			if (event.type == sf::Event::KeyReleased) {

				if (event.key.code == move_left) {
					move_left_pressed = false;
				}
				if (event.key.code == move_right) {
					move_right_pressed = false;
				}
				if (event.key.code == move_up) {
					move_up_pressed = false;
				}
				if (event.key.code == move_down) {
					move_down_pressed = false;
				}

			}
		}

		// Process input
		player.movementInput.x = 0;
		player.movementInput.y = 0;
		
		if (move_left_pressed) {
			player.movementInput.x -= 1;
		}
		if (move_right_pressed) {
			player.movementInput.x += 1;
		}
		if (move_up_pressed) {
			player.movementInput.y -= 1;
		}
		if (move_down_pressed) {
			player.movementInput.y += 1;
		}

		// Normalize vector
		float mag = std::sqrt(std::pow(player.movementInput.x, 2) + std::pow(player.movementInput.y, 2));
		if (mag != 0) {
			player.movementInput /= mag;
		}
		
		//Testing the buff
		if (enemy.position.y > 100) {
			Buff white_tint = Buff();
			white_tint.buff_target = &enemy;
			all_buffs.push_back(white_tint);
			enemy.buff_list.push_back(&white_tint);
		}
		// Run updates
		player.update(dt.asSeconds());
		enemy.update(dt.asSeconds());
		enemy2.update(dt.asSeconds());

		//Updating all buffs
		for (int i = 0; i < all_buffs.size(); i++) {
			all_buffs[i].update(dt.asSeconds());
			if (all_buffs[i].duration <= 0) {
				all_buffs.erase(all_buffs.begin() + i);
			}
		}

		window.clear();
		
		map.drawMap(window);

		player.draw(window);
		enemy.draw(window);
		enemy2.draw(window);

		window.display();
	}

	return 0;
}