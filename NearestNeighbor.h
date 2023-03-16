#pragma once

#include <queue>

#include "Graph.h"

class NearestNeighbor
{

public:

	NearestNeighbor(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);

private:

	Graph graph;
	int NUM_OF_CITIES;
	float final_distance;

	std::queue<sf::VertexArray> paths;
	sf::VertexArray final_route;

};