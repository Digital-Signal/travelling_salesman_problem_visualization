#pragma once

#include <queue>
#include<tuple>

#include "Graph.h"

class BruteForce
{

public:

	BruteForce(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	int number_of_routes;
	int route_number;
	bool FINISHED;
	float shortestPathLength;

	std::queue<std::pair<sf::VertexArray, std::tuple <float, float, int>>> paths;
	sf::VertexArray solution;

};

