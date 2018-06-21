#include "SearchAlg.h"

SearchAlg::SearchAlg() {
	
}

//Closest Node
/*vector<int> SearchAlg::closest_node() {
	vector<int> full_path = { 0 };
	int current_node;
	vector<int> neighbors;
	vector<double> distances;
	double dist;
	int neighbor_node;
	vector<double>::iterator lowest_dist;
	int lowest_node, next_node;
	bool building_path = true;
	while (building_path){
		current_node = full_path.back();
	neighbors = paths_list[current_node];
	distances = {};
	for (int i = 0; i < neighbors.size(); i++) {
		neighbor_node = neighbors[i];
		dist = sqrt(pow((node[current_node][0] - node[neighbor_node][0]), 2) + pow((node[current_node][1] - node[neighbor_node][1]), 2));
		distances.push_back(dist);
	}
	lowest_dist = min_element(distances.begin(), distances.end());
	lowest_node = distance(distances.begin(), lowest_dist);
	next_node = neighbors[lowest_node];
	full_path.push_back(next_node);
	if (next_node == (node_count - 1)) {
		building_path = false;
	}
	}
	return full_path;
}*/

vector<int> SearchAlg::astar(GameMap map, unsigned int start, unsigned int end) {
	//Takes two tiles by index as input
	//Returns a vector of tiles by index

	//int node_count;
	vector<TileType> nodes = map.tiles;
	//vector<vector<int>> paths_list;
	//The above variables would need a conversion from NavGraph to fit into original A* code
	
	vector<int> full_path;
	vector<int> prior_path;
	vector<int> open_set = { 0 };
	vector<int> closed_set;
	vector<double> open_scores;
	vector<double> cost = { 0 };
	vector<int> neighbors;
	double dist, temp_cost;
	double est_dist;
	vector<double> est_list;
	for (int i = 0; i < node_count; i++) {
		est_dist = sqrt(pow((node[i][0] - node[node_count - 1][0]), 2) + pow((node[i][1] - node[node_count - 1][1]), 2));
		est_list[i] = est_dist;
	}
	vector<double> score = { est_list[0] };
	for (int i = 0; i < node_count; i++) {
		prior_path.push_back(-1);
		cost.push_back(INFINITY);
		score.push_back(INFINITY);
	}
	vector<double>::iterator lowest_score;
	int lowest_node, current_node, neighbor_node;
	while (!open_set.empty()) {
		open_scores = {};
		for (unsigned int i = 0; i < open_set.size(); i++) {
			open_scores.push_back(score[open_set[i]]);
		}
		lowest_score = min_element(open_scores.begin(), open_scores.end());
		lowest_node = distance(open_scores.begin(), lowest_score);
		current_node = open_set[lowest_node];
		if (current_node == (node_count - 1)) {
			full_path = { current_node };
			while (current_node > 0) {
				current_node = prior_path[current_node];
				full_path.push_back(current_node);
			}
			reverse(full_path.begin(), full_path.end());
			return full_path;
		}
		remove(open_set.begin(), open_set.end(), current_node);
		open_set.pop_back();
		closed_set.push_back(current_node);
		neighbors = paths_list[current_node];
		for (unsigned int i = 0; i < neighbors.size(); i++) {
			neighbor_node = neighbors[i];
			if (count(closed_set.begin(), closed_set.end(), neighbor_node) > 0) {
				continue;
			}
			if (count(open_set.begin(), open_set.end(), neighbor_node) == 0) {
				open_set.push_back(neighbor_node);
			}
			dist = sqrt(pow((node[neighbor_node][0] - node[current_node][0]), 2) + pow((node[neighbor_node][1] - node[current_node][1]), 2));
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
