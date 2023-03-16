#pragma once

#include <queue>
#include <random>

#include "Graph.h"

class Random
{
public:

	Random(Graph graph, int count);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	int number_of_shuffles;
	int shuffle_number;
	bool FINISHED;
	float shortestPathLength;

	std::queue<std::pair<sf::VertexArray, std::tuple <float, float, int>>> paths;
	sf::VertexArray solution;

};

