#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include "Graph.h"

// Held–Karp algorithm
// Dynamic Programming - Top Down Approach - Memoization

// Time Complexity:  O(n*n*2^n)
// Number of subproblems to solve: (n-1)*2^(n-2)
// Max amount of time to solve one subproblem: (n-1)
// Time Complexity = (n-1)*(n-1)*2^(n-2) = O(n*n*2^n)

// Space Complexity:  O(n*2^n)
// Since there are (n-1)*2^(n-2) subproblems, then (n-1)*2^(n-2) subproblem solutions need to be stored in cache.
// Space Complexity = (n-1)*2^(n-2) = O(n*2^n)

class DP_Memoization
{

public:

	DP_Memoization(Graph graph);
	void computePath();
	void display(sf::RenderWindow& window);
	void terminate();

private:

	Graph graph;
	int NUM_OF_CITIES;
	int total_number_of_subproblems;
	int subproblem_count;
	bool FINISHED;
	float best_distance;
	sf::VertexArray solution;

	std::string encode(std::set<int> s);
	float g(int i, std::set<int> s, std::unordered_map<int, std::unordered_map<std::string, float>>& dp, std::unordered_map<int, std::unordered_map<std::string, int>>& p);

};