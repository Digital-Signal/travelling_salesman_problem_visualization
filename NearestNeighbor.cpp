#include "NearestNeighbor.h"

NearestNeighbor::NearestNeighbor(Graph graph) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    final_distance = 0;
}

void NearestNeighbor::computePath()
{

    sf::CircleShape start = graph.getNodes()[0]; // Starting node
    sf::CircleShape lastNodeInPath = graph.getNodes()[0]; // Last node in current path

    // A vector of unvisited nodes (the starting node has been visited)
    std::vector<sf::CircleShape> unvisited = graph.getNodes();
    unvisited.erase(unvisited.begin());

    // A vector to hold the current path (the starting node is in the path)
    std::vector<sf::CircleShape> path;
    path.push_back(start);

    // While there are unvisited nodes
    while (!unvisited.empty()) {

        float min_dist = INT_MAX; // The shortest distance from the last node in path to some unvisited node
        sf::CircleShape closest; // The closest unvisited node to the last node in the path

        // Find the closest unvisited node to the last node in the path
        for (sf::CircleShape c : unvisited) {

            if (graph.Distance(lastNodeInPath, c) < min_dist) {

                min_dist = graph.Distance(lastNodeInPath, c);
                closest = c;

            }

        }

        // Add the closest unvisited node to the path
        path.push_back(closest);

        // Update the last node in the path
        lastNodeInPath = closest;

        // Draw the path
        sf::VertexArray route(sf::LinesStrip, path.size());
        for (int i = 0; i < path.size(); i++) {
            route[i].position = sf::Vector2f(path[i].getPosition().x + 10, path[i].getPosition().y + 10);
            route[i].color = sf::Color(0, 255, 0);
        }

        // Send the path to the main thread
        paths.push(route);

        // Remove the closest node from the unvisited vector
        for (int i = 0; i < unvisited.size(); i++) {

            if ((unvisited[i].getPosition().x == closest.getPosition().x) && (unvisited[i].getPosition().y == closest.getPosition().y)) {
                unvisited.erase(unvisited.begin() + i);
                break;
            }

        }

    }

    // Connect the last node in the path to the starting node
    sf::VertexArray route(sf::LinesStrip, path.size() + 1);
    for (int i = 0; i < path.size(); i++) {
        route[i].position = sf::Vector2f(path[i].getPosition().x + 10, path[i].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }
    route[path.size()].position = sf::Vector2f(path[0].getPosition().x + 10, path[0].getPosition().y + 10);
    route[path.size()].color = sf::Color(0, 255, 0);
    paths.push(route);

    final_distance = graph.PathDistance(path);

}

void NearestNeighbor::display(sf::RenderWindow& window)
{
   
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_NN;
    text_NN.setFont(font);
    text_NN.setCharacterSize(20);
    text_NN.setFillColor(sf::Color::Cyan);
    text_NN.setStyle(sf::Text::Bold);
    text_NN.setPosition(810, 450);

    if(!paths.empty()) {

        sf::VertexArray p = paths.front();
        
        window.draw(p);
        paths.pop();
        final_route = p;
        
    }
    if(paths.empty()) {

        if (final_distance != 0) {

            text_NN.setString("[Final Distance]\n" + std::to_string(final_distance));
            window.draw(text_NN);

        }

        window.draw(final_route);
    }

}