#include "Random.h"

Random::Random(Graph graph, int count) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    number_of_shuffles = count;
    shuffle_number = 0;
    FINISHED = false;
    shortestPathLength = std::numeric_limits<float>::max();
}

// Terminates computePath() -> If computePath() is invoked by a thread and terminate() is called, then the thread will end
void Random::terminate()
{
    FINISHED = true;
}

void Random::computePath()
{
    std::vector<int> path(NUM_OF_CITIES, 0);
    for (int i = 0; i < NUM_OF_CITIES; i++) {
        path[i] = i;
    }

    std::vector<int> shortestPath = {};

    do {

        if (FINISHED)
            return;

        std::shuffle(path.begin(), path.end(), std::random_device());
        shuffle_number++;

        float currentPathLength = graph.PathLength(path);
        if (currentPathLength < shortestPathLength) {
            shortestPathLength = currentPathLength;
            shortestPath = path;
        }

        // Current Distance, Best Distance, Shuffle #
        std::tuple <float, float, int> stats;
        std::get<0>(stats) = currentPathLength;
        std::get<1>(stats) = shortestPathLength;
        std::get<2>(stats) = shuffle_number;

        if (currentPathLength == shortestPathLength) {
            sf::VertexArray route(sf::LinesStrip, NUM_OF_CITIES + 1);
            for (int i = 0; i < NUM_OF_CITIES; i++) {
                int n = path[i];
                route[i].position = sf::Vector2f(graph.getNodes()[n].getPosition().x + 10, graph.getNodes()[n].getPosition().y + 10);
                route[i].color = sf::Color(0, 255, 0);
            }
            route[NUM_OF_CITIES].position = sf::Vector2f(graph.getNodes()[path[0]].getPosition().x + 10, graph.getNodes()[path[0]].getPosition().y + 10);
            route[NUM_OF_CITIES].color = sf::Color(0, 255, 0);
            paths.push({ route, stats });
        }
        else {
            sf::VertexArray route(sf::LinesStrip, (NUM_OF_CITIES + 1));
            for (int i = 0; i < NUM_OF_CITIES; i++) {
                int n = path[i];
                route[i].position = sf::Vector2f(graph.getNodes()[n].getPosition().x + 10, graph.getNodes()[n].getPosition().y + 10);
                route[i].color = sf::Color(0, 0, 0);
            }
            route[NUM_OF_CITIES].position = sf::Vector2f(graph.getNodes()[path[0]].getPosition().x + 10, graph.getNodes()[path[0]].getPosition().y + 10);
            route[NUM_OF_CITIES].color = sf::Color(0, 0, 0);
            paths.push({ route, stats });
        }

    } while (shuffle_number != number_of_shuffles);

}

void Random::display(sf::RenderWindow& window)
{

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_Random;
    text_Random.setFont(font);
    text_Random.setCharacterSize(20);
    text_Random.setFillColor(sf::Color::Cyan);
    text_Random.setStyle(sf::Text::Bold);
    text_Random.setPosition(810, 450);

    if (!paths.empty()) {
 
        std::pair<sf::VertexArray, std::tuple <float, float, int>> p = paths.front();

        sf::VertexArray r = p.first;

        std::tuple <float, float, int> stats = p.second;
        float currentLength = std::get<0>(stats);
        float shortestLength = std::get<1>(stats);
        int shuffle_num = std::get<2>(stats);
        std::string s1 = "[Current Distance] / [Best Distance]\n" + std::to_string(currentLength) + " / " + std::to_string(shortestLength);
        std::string s2 = "[Shuffle #] / [Total # Of Shuffles]\n" + std::to_string(shuffle_num) + " / " + std::to_string(number_of_shuffles);
        text_Random.setString(s1 + "\n\n" + s2);

        // ERROR Handling
        if (r.getVertexCount() == 0) {
            paths.pop();
            std::cout << "ERROR" << std::endl;
            return;
        }

        // Save the shortest path
        if (r[0].color == sf::Color(0, 255, 0))
            solution = r;

        // Display statistical data
        window.draw(text_Random);
        // Display current path
        window.draw(r);
        // Dequeue
        paths.pop();
        // Display best path
        window.draw(solution);

    }
    else {
        std::string s1 = "[Best Distance]\n" + std::to_string(shortestPathLength);
        std::string s2 = "[Shuffle #] / [Total # Of Shuffles]\n" + std::to_string(shuffle_number) + " / " + std::to_string(number_of_shuffles);
        text_Random.setString(s1 + "\n\n" + s2);

        // Display statistical data
        window.draw(text_Random);
        // Display best path
        window.draw(solution);
    }

}
