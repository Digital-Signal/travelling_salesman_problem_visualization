#pragma once

#include <queue>
#include <random>
#include <algorithm>

#include "Graph.h"

class GeneticAlgorithm
{

public:

	GeneticAlgorithm(Graph graph, int population_size = 100, int elite_size = 20, double mutation_rate = 0.01, int number_of_generations = 500);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	bool FINISHED;

	int population_size;
	int elite_size;
	double mutation_rate;
	int number_of_generations;

	std::queue<std::pair<sf::VertexArray, std::tuple <float, int>>> paths;

	struct individual {
		double fitness;
		std::vector<sf::CircleShape> path;
	};

	bool compareIndividual(individual i1, individual i2);
	void displayBestPath(Graph graph, std::vector<individual> population, int generation_num);

	// For drawing purposes
	sf::VertexArray r; // Route
	float distance;
	int generation_num;

};