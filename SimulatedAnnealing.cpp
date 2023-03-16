#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(Graph graph, int num_of_iterations, double temperature, double alpha) : graph(graph)
{

    this->num_of_iterations = num_of_iterations;
    this->temperature = temperature;
    this->alpha = alpha;

    NUM_OF_CITIES = graph.getNumberOfNodes();
    FINISHED = false;

    //---------------------------------------------------------

    best_distance = 0;
}

void SimulatedAnnealing::computePath()
{
    srand(time(0));

    std::vector<sf::CircleShape> best_path = graph.getNodes();

    sf::VertexArray route(sf::LinesStrip, best_path.size() + 1);
    for (int i = 0; i <= best_path.size(); i++) {
        route[i].position = sf::Vector2f(best_path[i % best_path.size()].getPosition().x + 10, best_path[i % best_path.size()].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }

    double err = graph.PathDistance(best_path);
    int iter = 0;

    // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability
    std::tuple <DisplayOptions, int, double, double, double, double> stats;
    std::get<0>(stats) = ShorterDistance;
    std::get<1>(stats) = iter;
    std::get<2>(stats) = err;
    std::get<3>(stats) = temperature;
    std::get<4>(stats) = -1;
    std::get<5>(stats) = -1;

    paths.push({ route, stats });

    while (iter < num_of_iterations) {

        if (FINISHED)
            return;

        std::vector<sf::CircleShape> new_path = candidate(best_path);
        double new_err = graph.PathDistance(new_path);

        if (new_err <= err) {

            best_path = new_path;
            err = new_err;

            for (int i = 0; i <= best_path.size(); i++) {
                route[i].position = sf::Vector2f(best_path[i % best_path.size()].getPosition().x + 10, best_path[i % best_path.size()].getPosition().y + 10);
                route[i].color = sf::Color(0, 255, 0);
            }

            // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability
            std::tuple <DisplayOptions, int, double, double, double, double> stats;
            std::get<0>(stats) = ShorterDistance;
            std::get<1>(stats) = iter;
            std::get<2>(stats) = err;
            std::get<3>(stats) = temperature;
            std::get<4>(stats) = -1;
            std::get<5>(stats) = -1;

            paths.push({ route, stats });
        }
        else {

            double accept_probability = exp((err - new_err) / temperature);
            double probability = ((double)rand()) / RAND_MAX; // [0,1]

            if (probability < accept_probability) {

                best_path = new_path;
                err = new_err;

                for (int i = 0; i <= best_path.size(); i++) {
                    route[i].position = sf::Vector2f(best_path[i % best_path.size()].getPosition().x + 10, best_path[i % best_path.size()].getPosition().y + 10);
                    route[i].color = sf::Color(0, 255, 0);
                }

                // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability
                std::tuple <DisplayOptions, int, double, double, double, double> stats;
                std::get<0>(stats) = LongerDistanceAndAccepted;
                std::get<1>(stats) = iter;
                std::get<2>(stats) = err;
                std::get<3>(stats) = temperature;
                std::get<4>(stats) = accept_probability;
                std::get<5>(stats) = probability;

                paths.push({ route, stats });

            }
            else {

                // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability
                std::tuple <DisplayOptions, int, double, double, double, double> stats;

                std::get<0>(stats) = LongerDistanceAndNotAccepted;
                std::get<1>(stats) = iter;
                std::get<2>(stats) = new_err;
                std::get<3>(stats) = temperature;
                std::get<4>(stats) = accept_probability;
                std::get<5>(stats) = probability;

                paths.push({ route, stats });

            }

        }
        if (temperature < 0.00001)
            temperature = 0.00001;
        else
            temperature *= alpha;

        iter += 1;
    }

    for (int i = 0; i <= best_path.size(); i++) {
        route[i].position = sf::Vector2f(best_path[i % best_path.size()].getPosition().x + 10, best_path[i % best_path.size()].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }

    // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability
    std::tuple <DisplayOptions, int, double, double, double, double> stat;

    std::get<0>(stat) = Finished;
    std::get<1>(stat) = iter;
    std::get<2>(stat) = graph.PathDistance(best_path);;
    std::get<3>(stat) = temperature;
    std::get<4>(stat) = -1;
    std::get<5>(stat) = -1;

    paths.push({ route, stat });

}

void SimulatedAnnealing::display(sf::RenderWindow& window)
{

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_SimulatedAnnealing;
    text_SimulatedAnnealing.setFont(font);
    text_SimulatedAnnealing.setCharacterSize(12);
    text_SimulatedAnnealing.setFillColor(sf::Color::Cyan);
    text_SimulatedAnnealing.setPosition(810, 445);

    if (!paths.empty()) {

        std::pair<sf::VertexArray, std::tuple <DisplayOptions, int, double, double, double, double>> pair = paths.front();

        p = pair.first; // The path

        std::tuple <DisplayOptions, int, double, double, double, double> stats = pair.second;
        // DisplayOptions, Iteration #, Current Distance, Current Temperature, Acceptance Probability, Probability

        DisplayOptions display_option = std::get<0>(stats);
        int iteration_number = std::get<1>(stats);
        double current_distance = std::get<2>(stats);
        double current_temperature = std::get<3>(stats);
        double acceptance_probability = std::get<4>(stats);
        double probability = std::get<5>(stats);

        if (display_option == ShorterDistance) {

            best_distance = current_distance;

            std::string s;
           
            s = "[Iteration #] / [Total # Of Iterations]\n" + std::to_string(iteration_number) + " / " + std::to_string(num_of_iterations);
            s += "\n[Current Distance] / [Best Distance]\n" + std::to_string(current_distance) + " / " + std::to_string(best_distance);
            s += "\n[Current Temperature]\n" + std::to_string(current_temperature);
            s += "\n[Acceptance Probability]\n";
            s += "\n[Random Number Generator]\n";

            text_SimulatedAnnealing.setString(s);
            window.draw(text_SimulatedAnnealing);

        }
        else if (display_option == LongerDistanceAndAccepted) {

            best_distance = current_distance;

            std::string s;

            s = "[Iteration #] / [Total # Of Iterations]\n" + std::to_string(iteration_number) + " / " + std::to_string(num_of_iterations);
            s += "\n[Current Distance] / [Best Distance]\n" + std::to_string(current_distance) + " / " + std::to_string(best_distance);
            s += "\n[Current Temperature]\n" + std::to_string(current_temperature);
            s += "\n[Acceptance Probability]\n" + std::to_string(acceptance_probability);
            s += "\n[Random Number Generator]\n" + std::to_string(probability);

            text_SimulatedAnnealing.setString(s);
            window.draw(text_SimulatedAnnealing);
        }
        else if (display_option == LongerDistanceAndNotAccepted) {

            std::string s;

            s = "[Iteration #] / [Total # Of Iterations]\n" + std::to_string(iteration_number) + " / " + std::to_string(num_of_iterations);
            s += "\n[Current Distance] / [Best Distance]\n" + std::to_string(current_distance) + " / " + std::to_string(best_distance);
            s += "\n[Current Temperature]\n" + std::to_string(current_temperature);
            s += "\n[Acceptance Probability]\n" + std::to_string(acceptance_probability);
            s += "\n[Random Number Generator]\n" + std::to_string(probability);

            text_SimulatedAnnealing.setString(s);
            window.draw(text_SimulatedAnnealing);
        }
        else if (display_option == Finished) {

            best_distance = current_distance;

            std::string s;

            s = "[Iteration #] / [Total # Of Iterations]\n" + std::to_string(iteration_number) + " / " + std::to_string(num_of_iterations);
            s += "\n[Current Distance] / [Best Distance]\n" + std::to_string(current_distance) + " / " + std::to_string(best_distance);
            s += "\n[Current Temperature]\n" + std::to_string(current_temperature);
            s += "\n[Acceptance Probability]\n";
            s += "\n[Random Number Generator]\n";

            text_SimulatedAnnealing.setString(s);
            window.draw(text_SimulatedAnnealing);
        }

        window.draw(p);
        paths.pop();

    }
    else {

        std::string s;

        s = "[Iteration #] / [Total # Of Iterations]\n" + std::to_string(num_of_iterations) + " / " + std::to_string(num_of_iterations);
        s += "\n[Best Distance]\n" + std::to_string(best_distance);
        s += "\n[Current Temperature]\n" + std::to_string(temperature);
        s += "\n[Acceptance Probability]\n";
        s += "\n[Random Number Generator]\n";

        text_SimulatedAnnealing.setString(s);
        window.draw(text_SimulatedAnnealing);

        window.draw(p);
    }

}

// Terminates computePath() -> If computePath() is invoked by a thread and terminate() is called, then the thread will end
void SimulatedAnnealing::terminate()
{
    FINISHED = true;
}

std::vector<sf::CircleShape> SimulatedAnnealing::candidate(std::vector<sf::CircleShape> Path)
{
    
    int i = rand() % Path.size();
    int j = rand() % Path.size();

    if (i <= j)
        std::reverse(Path.begin() + i, Path.begin() + j + 1); // 2-opt 
    else
        std::reverse(Path.begin() + j, Path.begin() + i + 1); // 2-opt 

    return Path;

}