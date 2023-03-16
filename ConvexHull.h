#pragma once

#include <queue>

#include "Graph.h"

class ConvexHull
{

public:

	ConvexHull(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	float final_distance;
	bool FINISHED;

	std::queue<std::pair<sf::VertexArray, int>> q; // For drawing lines 

	// For drawing purposes
	sf::VertexArray pointOnHull_to_endpoint;
	std::vector<sf::VertexArray> hull;
	sf::VertexArray tour;

};