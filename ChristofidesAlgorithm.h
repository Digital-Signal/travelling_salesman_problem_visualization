#pragma once

#include "Graph.h"

#include <queue>
#include <unordered_set>
#include <stack>
#include <unordered_map>
#include <windows.h>

class ChristofidesAlgorithm
{

public:

	ChristofidesAlgorithm(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	bool FINISHED;

    void backtracking(Graph graph, std::vector<sf::CircleShape>& odd, std::vector<std::pair<sf::CircleShape, sf::CircleShape>>& pm, float& minimum_weight, std::vector<std::pair<sf::CircleShape, sf::CircleShape>>& min_weight_pm);

    // For display purposes

    std::queue<sf::VertexArray> q_mst;
    std::queue<sf::CircleShape> q_odd_circle;
    std::queue<sf::VertexArray> q_pm;
    std::queue<sf::VertexArray> q_clear_merge;
    std::queue<sf::CircleShape> q_clear_circle;
    std::queue<sf::VertexArray> q_hamiltonian;
    bool finished = false;

    std::vector<sf::VertexArray> mst;
    std::vector<sf::CircleShape> odd;
    std::vector<sf::VertexArray> matching;
    std::vector<sf::CircleShape> clear_circle;
    std::vector<sf::VertexArray> merge;
    std::vector<sf::VertexArray> hamiltonian;

    int number_of_perfect_matchings(int number_of_odd_degree_nodes);

    int PerfectMatchingCount;
    int NumberOfPerfectMatchings;
    float FinalDistance;
};