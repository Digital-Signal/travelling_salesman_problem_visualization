#include "Graph.h"
#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <stdlib.h>    
#include <time.h>       
#include <math.h>       
#include <limits>

Graph::Graph() {
    numberOfNodes = 0;
    srand(static_cast<unsigned int>(time(NULL)));
}

void Graph::NewGraph(int numberOfNodes)
{
    nodes.clear();
    adjacencyMatrix.clear();
    edges.clear();

    this->numberOfNodes = numberOfNodes;
    
    CreateNodes();
    CreateAdjacencyMatrix();
    //PrintAdjacencyMatrix();
    ComputeAllEdges();
}

void Graph::Clear()
{
    nodes.clear();
    adjacencyMatrix.clear();
    edges.clear();
    numberOfNodes = 0;
}

void Graph::drawNodes(sf::RenderWindow& window)
{
    for (auto &c : getNodes())
        window.draw(c);

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Cyan);
    text.setPosition(5, 570);
    text.setString("# of nodes:  " + std::to_string(numberOfNodes));
    window.draw(text);
}

void Graph::drawEdges(sf::RenderWindow& window)
{
    for (auto &l : getEdges()) {
        window.draw(l);
    }
}

void Graph::addNode(float x, float y)
{
    // Graph is empty
    if (numberOfNodes == 0) {

        if (x >= 10 && y >= 10 && x <= 790 && y <= 590) {

            sf::CircleShape circle(10);
            circle.setFillColor(sf::Color(0, 0, 0)); 
            circle.setOutlineThickness(-2);
            circle.setOutlineColor(sf::Color(0, 255, 0)); // Start node
            circle.setPosition(x - 10, y - 10);
            
            nodes.push_back(circle);
            numberOfNodes++;

            adjacencyMatrix.clear();
            edges.clear();

            CreateAdjacencyMatrix();
            //PrintAdjacencyMatrix();
            ComputeAllEdges();

        }
        else
            return;

    }
    // Graph is not empty
    else {
    
        if (x <= 790 && y <= 590 && x >= 10 && y >= 10) {

            for (auto &n : nodes) {

                // Get a node's position (+10 offset to get the centre coordinate)
                float x2 = n.getPosition().x + 10;
                float y2 = n.getPosition().y + 10;

                // If the new node overlaps with a previous node, then return
                if (sqrt(pow(x2 - x, 2) + pow(y2 - y, 2)) < 20)
                    return;

            }

            sf::CircleShape circle(10);
            circle.setFillColor(sf::Color(0, 0, 0));
            circle.setOutlineThickness(-2);
            circle.setOutlineColor(sf::Color(255, 0, 0));
            circle.setPosition(x - 10, y - 10);

            nodes.push_back(circle);
            numberOfNodes++;

            adjacencyMatrix.clear();
            edges.clear();

            CreateAdjacencyMatrix();
            //PrintAdjacencyMatrix();
            ComputeAllEdges();

        }
        else
            return;

    }

}
   
Graph::Graph(const Graph &graph)
{
    numberOfNodes = graph.numberOfNodes;
    nodes = graph.nodes;
    adjacencyMatrix = graph.adjacencyMatrix;
    edges = graph.edges;
}

void Graph::CreateNodes()
{
    // Create the nodes
    for (int i = 0; i < numberOfNodes; i++) {

        // Each node has a radius of 10
        sf::CircleShape circle(10);

        if (i == 0) {
            circle.setFillColor(sf::Color(0, 0, 0)); // Start node
            circle.setOutlineThickness(-2);
            circle.setOutlineColor(sf::Color(0, 255, 0));
        }
        else {
            circle.setFillColor(sf::Color(0, 0, 0)); // Colour the node black
            circle.setOutlineThickness(-2);
            circle.setOutlineColor(sf::Color(255, 0, 0));
        }

        // Set the node's position
        while (true) {

        NEW_COORDINATE:

            // Generate a random position for the node (+10 gets the centre coordinate)
            float x1 = static_cast<float>(rand() % 780 + 10);
            float y1 = static_cast<float>(rand() % 580 + 10);

            // Check that the generated coordinate does not overlap with any other node
            // For each node in the vector
            for (auto &n : nodes) {

                // Get a node's position (+10 offset to get the centre coordinate)
                float x2 = n.getPosition().x + 10;
                float y2 = n.getPosition().y + 10;

                // If the new node overlaps with a previous node, then generate a new position for the new node
                if (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) < 20)
                    goto NEW_COORDINATE;
            }

            // The new node does not overlap with any previous node
            // Set the new nodes position (-10 to remove the previous offset)
            circle.setPosition(x1 - 10, y1 - 10);
            // Break out of the infinite loop
            break;
        }

        // Push the node into the vector
        nodes.push_back(circle);
    }

}

void Graph::CreateAdjacencyMatrix()
{
    // Build the adjacency matrix
    adjacencyMatrix = std::vector<std::vector<float>>(numberOfNodes, std::vector<float>(numberOfNodes, 0));

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {

            float x1 = nodes[i].getPosition().x + 10;
            float y1 = nodes[i].getPosition().y + 10;

            float x2 = nodes[j].getPosition().x + 10;
            float y2 = nodes[j].getPosition().y + 10;


            adjacencyMatrix[i][j] = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
            adjacencyMatrix[j][i] = static_cast<float>(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));

        }
    }
}

void Graph::PrintAdjacencyMatrix()
{
    // Print the adjacency matrix
    std::cout << "\t";
    for (int i = 0; i < numberOfNodes; i++)
        std::cout << i << "\t";
    std::cout << std::endl;

    for (int i = 0; i < numberOfNodes; i++) {
        std::cout << i << "\t";
        for (int j = 0; j < numberOfNodes; j++) {

            std::cout << adjacencyMatrix[i][j] << "\t";

        }
        std::cout << std::endl;
    }
}

void Graph::ComputeAllEdges()
{
    // Display Edges

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {

            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(nodes[i].getPosition().x + 10, nodes[i].getPosition().y + 10);
            line[0].color = sf::Color(255, 255, 255);
            line[1].position = sf::Vector2f(nodes[j].getPosition().x + 10, nodes[j].getPosition().y + 10);
            line[1].color = sf::Color(255, 255, 255);

            edges.push_back(line);

        }
    }
}

float Graph::Distance(sf::CircleShape point1, sf::CircleShape point2)
{
    float x1 = point1.getPosition().x;
    float y1 = point1.getPosition().y;

    float x2 = point2.getPosition().x;
    float y2 = point2.getPosition().y;

    return static_cast<float>(std::sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

float Graph::PathDistance(std::vector<sf::CircleShape> Path)
{
    float length = 0;
    for (int i = 0; i < Path.size(); i++)
        length += Distance(Path[i], Path[(i + 1) % Path.size()]);

    return length;
}

float Graph::PathLength(std::vector<int> Path)
{
    float len = 0;

    for (int i = 1; i < Path.size(); i++)
        len += adjacencyMatrix[Path[i - 1]][Path[i]];

    len += adjacencyMatrix[Path[Path.size() - 1]][Path[0]];

    return len;
}

bool Graph::NodeInPath(sf::CircleShape node, std::vector<sf::CircleShape> path)
{
    for (int i = 0; i < path.size(); i++) {

        if (node.getPosition().x == path[i].getPosition().x && node.getPosition().y == path[i].getPosition().y)
            return true;

    }
    return false;
}

int Graph::getNumberOfNodes() const
{
    return numberOfNodes;
}

std::vector<sf::CircleShape> Graph::getNodes() const
{
    return nodes;
}

std::vector<std::vector<float>> Graph::getAdjacencyMatrix() const
{
    return adjacencyMatrix;
}

std::vector<sf::VertexArray> Graph::getEdges() const
{
    return edges;
}