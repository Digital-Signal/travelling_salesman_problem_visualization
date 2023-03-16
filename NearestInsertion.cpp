#include "NearestInsertion.h"

NearestInsertion::NearestInsertion(Graph graph) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    FINISHED = false;
    current_distance = 0;
}

void NearestInsertion::computePath()
{

    // Get a vector of nodes (cities)
    std::vector<sf::CircleShape> nodes = graph.getNodes();

    //--------------------------------------------------------------------------------------------------------------

    std::vector<sf::CircleShape> sub_tour;
    sub_tour.push_back(nodes[0]);

    float shortest_distance = graph.Distance(sub_tour[0], nodes[1]);
    sf::CircleShape nearest_circle = nodes[1];

    for (int i = 1; i < nodes.size(); i++) {

        if (graph.Distance(sub_tour[0], nodes[i]) < shortest_distance) {
            shortest_distance = graph.Distance(sub_tour[0], nodes[i]);
            nearest_circle = nodes[i];
        }

    }
    sub_tour.push_back(nearest_circle);

    //---------------------------------------------------------------------------------
    while (sub_tour.size() != nodes.size()) {

        if (FINISHED)
            return;

        float min_length = std::numeric_limits<float>::max();;
        sf::CircleShape closest_circle;

        for (int i = 0; i < sub_tour.size(); i++) {
            for (int j = 0; j < nodes.size(); j++) {

                // If the node is in the sub-tour, then try the next node
                if (graph.NodeInPath(nodes[j], sub_tour))
                    continue;

                // The node is not in the sub-tour
                if (graph.Distance(sub_tour[i], nodes[j]) < min_length) {
                    min_length = graph.Distance(sub_tour[i], nodes[j]);
                    closest_circle = nodes[j];
                }

            }
        }

        int i = 0;
        int j = 0;
        float min_cir_crj_cij = std::numeric_limits<float>::max();;

        for (int index = 0; index < sub_tour.size() - 1; index++) {

            if (graph.Distance(sub_tour[index], closest_circle) + graph.Distance(closest_circle, sub_tour[index + 1]) - graph.Distance(sub_tour[index], sub_tour[index + 1]) < min_cir_crj_cij) {
                min_cir_crj_cij = graph.Distance(sub_tour[index], closest_circle) + graph.Distance(closest_circle, sub_tour[index + 1]) - graph.Distance(sub_tour[index], sub_tour[index + 1]);
                i = index;
                j = index + 1;
            }

        }

        sub_tour.insert(sub_tour.begin() + i + 1, closest_circle);
        displayBestPath(sub_tour);

    }

    displayBestPath(sub_tour);

}

void NearestInsertion::display(sf::RenderWindow& window)
{
    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_NearestInsertion;
    text_NearestInsertion.setFont(font);
    text_NearestInsertion.setCharacterSize(20);
    text_NearestInsertion.setFillColor(sf::Color::Cyan);
    text_NearestInsertion.setStyle(sf::Text::Bold);
    text_NearestInsertion.setPosition(810, 450);

    if (!paths.empty()) {

        std::pair<sf::VertexArray, float> pair = paths.front();

        r = pair.first;

        current_distance = pair.second;
        std::string s1 = "[Current Distance]\n" + std::to_string(current_distance);
        text_NearestInsertion.setString(s1);

        window.draw(r);
        window.draw(text_NearestInsertion);

        paths.pop();

    }
    else {
        std::string s1 = "[Current Distance]\n" + std::to_string(current_distance);
        text_NearestInsertion.setString(s1);

        window.draw(r);
        window.draw(text_NearestInsertion);
    }

}

// Terminates computePath() -> If computePath() is invoked by a thread and terminate() is called, then the thread will end
void NearestInsertion::terminate()
{
    FINISHED = true;
}

void NearestInsertion::displayBestPath(std::vector<sf::CircleShape> path)
{
    float curr_dist = graph.PathDistance(path);
  
    sf::VertexArray route(sf::LinesStrip, path.size() + 1);
    for (int i = 0; i <= path.size(); i++) {
        route[i].position = sf::Vector2f(path[i % path.size()].getPosition().x + 10, path[i % path.size()].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }
    paths.push({ route,curr_dist });
}