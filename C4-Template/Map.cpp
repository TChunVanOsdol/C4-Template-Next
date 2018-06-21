#include "Map.h"
#include <iostream>

void GameMap::calcNeighbors(vertex & v)
{
	int x = v.coords.first;
	int y = v.coords.second;

	int coords_to_check[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

	for (auto coord : coords_to_check) {
		TileType t = getTile(x + coord[0], y + coord[1]);

		if (t != TileType::wall) {
			// If the tile is pathable, add to this vertex's connections
			gridcoord g = std::make_pair(coord[0], coord[1]);
			v.adjacent.push_back(g);
		}
	}

}

GameMap::GameMap(sf::RenderWindow& window, int width, int height)
{
	mapWidth = width;
	mapHeight = height;

	int pixelWidth = window.getSize().x;
	int pixelHeight = window.getSize().y;
	// Fit tiles to screen
	tileSize = (float)pixelHeight / (float)mapHeight;

	tiles = std::vector<TileType>(mapWidth * mapHeight);

	ground = sf::RectangleShape(sf::Vector2f(10, 10));
	ground.setFillColor(sf::Color::Green);

	wall = sf::RectangleShape(sf::Vector2f(10, 10));
	wall.setFillColor(sf::Color::White);

	// Load map here
	tiles[90] = TileType::wall;
	tiles[50] = TileType::wall;
	tiles[70] = TileType::wall;
	tiles[51] = TileType::wall;
	tiles[52] = TileType::wall;
	tiles[53] = TileType::wall;
	tiles[54] = TileType::wall;
	//tiles[41] = TileType::wall;
	//tiles[22] = TileType::wall;
	//tiles[23] = TileType::wall;
	//tiles[24] = TileType::wall;
	//tiles[42] = TileType::wall;

	// Build nav grid
	recalculateNav();

}

TileType GameMap::getTile(int x, int y)
{
	if(x >= mapWidth || x < 0){
		return TileType::wall;
	}
	if (y >= mapHeight || y < 0) {
		return TileType::wall;
	}
	return tiles[y * mapWidth + x];
}

int GameMap::getTileN(int x, int y)
{
	if (x >= mapWidth || x < 0) {
		return -1;
	}
	if (y >= mapHeight || y < 0) {
		return -1;
	}
	return (y * mapWidth + x);
}

void GameMap::recalculateNav()
{
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (getTile(x, y) != TileType::wall) {
				// If it's not a wall, it's probably pathable!
				vertex v;
				v.coords = std::make_pair(x, y);
				calcNeighbors(v);
				nav.addVertex(x, y, v);
			}
		}
	}


}

void GameMap::drawMap(sf::RenderWindow& window)
{

	int pixelWidth = window.getSize().x;
	int pixelHeight = window.getSize().y;
	// Fit tiles to screen
	tileSize = (float)pixelHeight / (float)mapHeight;
	ground.setSize(sf::Vector2f(tileSize, tileSize));
	wall.setSize(sf::Vector2f(tileSize, tileSize));


	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {

			float draw_x = x * tileSize;
			float draw_y = y * tileSize;

			switch (getTile(x, y)) {
			case TileType::ground:
				// Draw Ground
				ground.setPosition(sf::Vector2f(draw_x, draw_y));
				window.draw(ground);
				break;
			case TileType::wall:
				// Draw Wall
				wall.setPosition(sf::Vector2f(draw_x, draw_y));
				window.draw(wall);
				break;
			}
		}
	}

}

bool GameMap::checkTileCollision(Actor & actor)
{
	// Find applicable tiles
	sf::Vector2i top_corner = getTilePos(actor.position.x, actor.position.y);
	sf::Vector2i bottom_corner = getTilePos(actor.position.x + actor.size.x, actor.position.y + actor.size.y);

	for (int y = top_corner.y; y <= bottom_corner.y; y++) {
		for (int x = top_corner.x; x <= bottom_corner.x; x++) {
			if (getTile(x, y) == TileType::wall) {
				return true;
			}
		}
	}


	return false;
}

sf::Vector2f GameMap::wallLocation(Actor& actor)
{
	// Find applicable tiles
	sf::Vector2i top_corner = getTilePos(actor.position.x, actor.position.y);
	sf::Vector2i bottom_corner = getTilePos(actor.position.x + actor.size.x, actor.position.y + actor.size.y);
	float wall_x = 0;
	float wall_y = 0;

	for (int y = (top_corner.y); y <= bottom_corner.y; y++) {
		int x = top_corner.x; //Wall is on the left
		if (getTile(x, y) == TileType::wall) {
			wall_x += -1;
		}
	}
	for (int y = (top_corner.y); y <= bottom_corner.y; y++) {
		int x = bottom_corner.x; //Wall is on the right
		if (getTile(x, y) == TileType::wall) {
			wall_x += 1;
		}
	}
	for (int x = (top_corner.x); x <= bottom_corner.x; x++) {
		int y = top_corner.y; //Wall is on the top
		if (getTile(x, y) == TileType::wall) {
			wall_y += -1;
		}
	}
	for (int x = (top_corner.x); x <= bottom_corner.x; x++) {
		int y = bottom_corner.y; //Wall is on the bottom
		if (getTile(x, y) == TileType::wall) {
			wall_y += 1;
		}
	}
	float mag = std::sqrt(std::pow(wall_x, 2) + std::pow(wall_y, 2));
	if (mag != 0) {
		wall_x /= mag;
		wall_y /= mag;
	}
	return sf::Vector2f(wall_x, wall_y);
}

std::vector<gridcoord> GameMap::astar(gridcoord start, gridcoord end) {
	//Takes two tiles by gridcoord as input
	//Returns a vector of tiles by index
	//Maybe return a vector of gridcoords instead?

	std::vector<TileType> nodes = tiles;
	int node_count = nodes.size();
	sf::Vector2i start_tile = getTilePos(start.first, start.second);
	sf::Vector2i end_tile = getTilePos(end.first, end.second);
	int start_node = getTileN(start_tile.x, start_tile.y);
	int end_node = getTileN(end_tile.x, end_tile.y);
	//vector<vector<int>> paths_list;
	//The above variables would need a conversion from NavGraph to fit into original A* code

	std::vector<gridcoord> full_path = {};
	std::vector<int> prior_path (node_count);
	std::vector<int> open_set = { start_node };
	std::vector<int> closed_set;
	//Add all walls to closed set
	for (int i = 0; i < node_count; i++) {
		if (nodes[i] == TileType::wall) {
			closed_set.push_back(i);
		}
	}
	std::vector<double> open_scores;
	std::vector<double> cost;
	std::vector<gridcoord> neighbors;
	double neighbor_check[8][2] = { {-1, -1},{-1, 0},{-1, 1},{0, -1},{0, 1},{1, -1},{1, 0},{1, 1} };
	gridcoord new_neighbor;
	double dist, temp_cost;
	double est_dist;
	std::vector<double> est_list (node_count);
	/*for (int i = 0; i < node_count; i++) {
		sf::Vector2i node = getTilePosFromIndex(i);
		est_dist = std::sqrt(std::pow(node.x - end.first, 2) + std::pow(node.y - end.second, 2));
		est_list[i] = est_dist;
	}*/
	std::vector<double> score;
	for (int i = 0; i < node_count; i++) {
		cost.push_back(INFINITY);
		score.push_back(INFINITY);
	}
	cost[start_node] = 0;
	score[start_node] = std::sqrt(std::pow(start_tile.x - end_tile.x, 2) + std::pow(start_tile.y - end_tile.y, 2));
	std::vector<double>::iterator lowest_score;
	int lowest_node, current_node, neighbor_node;
	sf::Vector2i current_coord;
	while (!open_set.empty()) {
		open_scores = {};
		for (unsigned int i = 0; i < open_set.size(); i++) {
			open_scores.push_back(score[open_set[i]]);
		}
		lowest_score = min_element(open_scores.begin(), open_scores.end());
		lowest_node = distance(open_scores.begin(), lowest_score);
		current_node = open_set[lowest_node];
		std::cout << start_node << std::endl;
		current_coord = getTilePosFromIndex(current_node);
		if (current_node == end_node) {
			//current_coord = getTilePosFromIndex(current_node);
			full_path.push_back(std::make_pair(current_coord.x, current_coord.y)); 
			while (current_node != start_node) {
				current_node = prior_path[current_node];
				current_coord = getTilePosFromIndex(current_node);
				full_path.push_back(std::make_pair(current_coord.x, current_coord.y));
			}
			reverse(full_path.begin(), full_path.end());
			return full_path;
		}
		remove(open_set.begin(), open_set.end(), current_node);
		open_set.pop_back();
		closed_set.push_back(current_node);
		//Create the list of neighbors
		neighbors = {};
		for (int i = 0; i < 8; i++) {
			new_neighbor = std::make_pair(current_coord.x + neighbor_check[i][0], current_coord.y + neighbor_check[i][1]);
			neighbors.push_back(new_neighbor);
			if (getTileN(new_neighbor.first, new_neighbor.second) == -1) {
				neighbors.pop_back();
			}
			else if (getTile(new_neighbor.first, new_neighbor.second) == TileType::wall) {
				closed_set.push_back(getTileN(new_neighbor.first, new_neighbor.second));
			}
		}
		for (unsigned int i = 0; i < neighbors.size(); i++) {
			neighbor_node = getTileN(neighbors[i].first,neighbors[i].second);
			if (count(closed_set.begin(), closed_set.end(), neighbor_node) > 0) {
				continue;
			}
			if (count(open_set.begin(), open_set.end(), neighbor_node) == 0) {
				open_set.push_back(neighbor_node);
			}
			
			est_dist = std::sqrt(std::pow(int(neighbors[i].first) - end_tile.x, 2) + std::pow(int(neighbors[i].second) - end_tile.y, 2));
			est_list[neighbor_node] = est_dist;
			dist = std::sqrt(std::pow(int(neighbors[i].first) - int(current_coord.x), 2) + std::pow(int(neighbors[i].second) - int(current_coord.y), 2));
			temp_cost = cost[current_node] + dist;
			if (temp_cost >= cost[neighbor_node]) {
				continue;
			}
			prior_path[neighbor_node] = current_node;
			cost[neighbor_node] = temp_cost;
			score[neighbor_node] = cost[neighbor_node] + est_list[neighbor_node];
		}
	}
	return full_path; //Returns if open set ends up empty too early
}   //Don't forget this closing bracket!
