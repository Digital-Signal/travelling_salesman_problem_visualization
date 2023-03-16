#pragma once

#include <queue>

#include "Graph.h"

class TwoOpt
{

public:

	TwoOpt(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	bool FINISHED;

	std::queue<std::pair<sf::VertexArray, std::tuple <int, float>>> paths;

	// For display purposes
	sf::VertexArray p;
	int swap_num;
	float best_dist;

	std::vector<sf::CircleShape> two_opt_swap(std::vector<sf::CircleShape> Path, int V1, int V2);

};