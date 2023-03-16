#include "TwoOpt.h"

TwoOpt::TwoOpt(Graph graph) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    FINISHED = false;

    //----------------------------
    swap_num = 0;
    best_dist = 0;

}

void TwoOpt::computePath()
{
    int swap_number = 0;
    std::vector<sf::CircleShape> randomPath = graph.getNodes();
    
    sf::VertexArray route(sf::LinesStrip, randomPath.size() + 1);
    for (int i = 0; i <= randomPath.size(); i++) {
        route[i].position = sf::Vector2f(randomPath[i % randomPath.size()].getPosition().x + 10, randomPath[i % randomPath.size()].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }

    float shortest = graph.PathDistance(randomPath);
    
    // Swap number, best distance
    std::tuple <int, float> stats;
    std::get<0>(stats) = swap_number;
    std::get<1>(stats) = shortest;

    paths.push({ route, stats });

    shorter_route_found:

    swap_number++;

    if (FINISHED)
        return;

    for (int i = 0; i < randomPath.size() - 1; i++) {
        for (int j = i + 1; j < randomPath.size(); j++) {

            std::vector<sf::CircleShape> new_route = two_opt_swap(randomPath, i, j);
            float new_distance = graph.PathDistance(new_route);

            if (new_distance < shortest) {

                randomPath = new_route;
                shortest = new_distance;

                for (int i = 0; i <= randomPath.size(); i++) {
                    route[i].position = sf::Vector2f(randomPath[i % randomPath.size()].getPosition().x + 10, randomPath[i % randomPath.size()].getPosition().y + 10);
                    route[i].color = sf::Color(0, 255, 0);
                }

                // Swap number, best distance
                std::tuple <int, float> stats;
                std::get<0>(stats) = swap_number;
                std::get<1>(stats) = shortest;

                paths.push({ route, stats });

                goto shorter_route_found;

            }

        }
    }

}

void TwoOpt::display(sf::RenderWindow& window)
{

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_TwoOpt;
    text_TwoOpt.setFont(font);
    text_TwoOpt.setCharacterSize(20);
    text_TwoOpt.setFillColor(sf::Color::Cyan);
    text_TwoOpt.setStyle(sf::Text::Bold);
    text_TwoOpt.setPosition(810, 450);

    if (!paths.empty()) {

        std::pair<sf::VertexArray, std::tuple <int, float>> pair = paths.front();

        p = pair.first;

        // Swap number, best distance
        std::tuple <int, float> stats = pair.second;
        swap_num = std::get<0>(stats);
        best_dist = std::get<1>(stats);

        std::string s1 = "[Swap #]\n" + std::to_string(swap_num);
        std::string s2 = "[Best Distance]\n" + std::to_string(best_dist);
        text_TwoOpt.setString(s1 + "\n\n" + s2);
        window.draw(text_TwoOpt);
        
        window.draw(p);
        paths.pop();

    }
    else {
        std::string s1 = "[Swap #]\n" + std::to_string(swap_num);
        std::string s2 = "[Best Distance]\n" + std::to_string(best_dist);
        text_TwoOpt.setString(s1 + "\n\n" + s2);
        window.draw(text_TwoOpt);
        window.draw(p);
    }

}

// Terminates computePath() -> If computePath() is invoked by a thread and terminate() is called, then the thread will end
void TwoOpt::terminate()
{
    FINISHED = true;
}

std::vector<sf::CircleShape> TwoOpt::two_opt_swap(std::vector<sf::CircleShape> Path, int V1, int V2)
{
    std::reverse(Path.begin() + V1 + 1, Path.begin() + V2 + 1);
    return Path;
}