#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <stdlib.h>     
#include <time.h>     
#include <math.h>     
#include <limits>

class Graph
{

private:

	int numberOfNodes;
	std::vector<sf::CircleShape> nodes;
	std::vector<std::vector<float>> adjacencyMatrix;
	std::vector<sf::VertexArray> edges;

	void CreateNodes();
	void CreateAdjacencyMatrix();
	void PrintAdjacencyMatrix();
	void ComputeAllEdges();

public:

	Graph();
	Graph(const Graph& graph);

	void NewGraph(int numberOfNodes);
	void Clear();

	void drawNodes(sf::RenderWindow& window);
	void drawEdges(sf::RenderWindow& window);

	void addNode(float x, float y);

	float Distance(sf::CircleShape point1, sf::CircleShape point2);
	float PathDistance(std::vector<sf::CircleShape> Path); // Uses sf::CircleShape
	float PathLength(std::vector<int> Path); // Uses indices
	bool NodeInPath(sf::CircleShape node, std::vector<sf::CircleShape> path);

	int getNumberOfNodes() const;
	std::vector<sf::CircleShape> getNodes() const;
	std::vector<std::vector<float>> getAdjacencyMatrix() const;
	std::vector<sf::VertexArray> getEdges() const;

};