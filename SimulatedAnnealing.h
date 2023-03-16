#pragma once

#include <queue>

#include "Graph.h"

class SimulatedAnnealing
{

public:

	SimulatedAnnealing(Graph graph, int num_of_iterations, double temperature, double alpha);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	bool FINISHED;

	int num_of_iterations;
	double temperature;
	double alpha;

	// For display purposes
	enum DisplayOptions { ShorterDistance, LongerDistanceAndAccepted, LongerDistanceAndNotAccepted, Finished };

	std::queue<std::pair<sf::VertexArray, std::tuple <DisplayOptions, int, double, double, double, double>>> paths;

	// For display purposes
	sf::VertexArray p; // path

	// For display purposes
	double best_distance;

	std::vector<sf::CircleShape> candidate(std::vector<sf::CircleShape> Path);

};