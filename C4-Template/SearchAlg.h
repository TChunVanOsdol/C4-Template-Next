#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Map.h"

using namespace std;

class SearchAlg
{
public:
	int node_count;
	GameMap* mapref = NULL;
	NavGraph node_map;
	vector<vector<int>> node;
	vector<vector<int>> paths_list;

	//vector<int> closest_node();
	vector<int> astar(GameMap map, unsigned int start, unsigned int end);

	SearchAlg();
};
