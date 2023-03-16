#pragma once

#include <queue>

#include "Graph.h"

class NearestInsertion
{
public:

	NearestInsertion(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	bool FINISHED;

	std::queue<std::pair<sf::VertexArray, float>> paths;

	// For drawing purposes
	sf::VertexArray r;
	float current_distance;

	void displayBestPath(std::vector<sf::CircleShape> path);

};