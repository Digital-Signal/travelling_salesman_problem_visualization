#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(Graph graph, int population_size, int elite_size, double mutation_rate, int number_of_generations) : graph(graph)
{
	this->population_size = population_size;
	this->elite_size = elite_size;
	this->mutation_rate = mutation_rate;
	this->number_of_generations = number_of_generations;

    NUM_OF_CITIES = graph.getNumberOfNodes();
    FINISHED = false;

    // --------------------------------------------------------------

    distance = 0;
    generation_num = 0;

}

void GeneticAlgorithm::computePath()
{
    srand(time(0));

    // Get a vector of nodes (cities)
    std::vector<sf::CircleShape> nodes = graph.getNodes();

    // Create Initial Population  
    std::vector<individual> population;
    for (int i = 0; i < population_size; i++) {

        individual r;
        std::shuffle(nodes.begin(), nodes.end(), std::random_device());
        r.path = nodes;
        r.fitness = 0;
        population.push_back(r);

    }

    //--------------------------------------------------------------------------------------------------------------
    displayBestPath(graph, population, 0);
    //--------------------------------------------------------------------------------------------------------------

    // Create Next Generation
    int next;
    for (next = 1; next <= number_of_generations; next++) {

        if (FINISHED)
            return;

        // -----------------------------------------------------------------------------------------------------
        // Fitness (Compute the fitness of each individual)
        for (int i = 0; i < population_size; i++) {
            population[i].fitness = 1 / graph.PathDistance(population[i].path);
        }

        // -----------------------------------------------------------------------------------------------------
        // Selection (using roulette wheel selection)
        std::vector<individual> children;

        // Sort the population according to the fitness in decreasing order
        sort(population.begin(), population.end(), [](const individual& i1, const individual& i2)
            {
                return (i1.fitness > i2.fitness);
            });

        // Save the best routes in the children vector (Elitism)
        for (int i = 0; i < elite_size; i++) {
            children.push_back(population[i]);
        }

        std::vector<individual> parents;
        int num_of_parents = (population_size - elite_size) * 2;
        //----------------------------------------------------------------------------------------------------------------
        double summation_of_fitness = 0;
        for (int i = 0; i < population.size(); i++) {
            summation_of_fitness += population[i].fitness;
        }

        double lower_bound = 0;
        double upper_bound = summation_of_fitness;
        std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
        std::default_random_engine re;

        for (int c = 0; c < num_of_parents; c++) {
            double a_random_double = unif(re);

            int i = 0;
            while (a_random_double > 0) {
                a_random_double = a_random_double - population[i].fitness;
                i++;
            }

            parents.push_back(population[i - 1]);
        }

        //----------------------------------------------------------------------------------------------------------------
        // Crossover

        sf::CircleShape null;
        null.setPosition(-1.f, -1.f);

        for (int i = 0; i < parents.size(); i = i + 2) {

            std::vector<sf::CircleShape> p1 = parents[i].path;
            std::vector<sf::CircleShape> p2 = parents[i + 1].path;

            int child_size = p1.size();
            std::vector<sf::CircleShape> child(child_size, null);

            int startIndex = rand() % child_size;
            int endIndex = rand() % child_size;
            startIndex = std::min(startIndex, endIndex);
            endIndex = std::max(startIndex, endIndex);

            for (int index = startIndex; index <= endIndex; index++) {
                child[index] = p1[index];
            }

            int index_p2 = (endIndex + 1) % p2.size();
            int index_child = (endIndex + 1) % child.size();

            while (index_child != startIndex) {

                if (graph.NodeInPath(p2[index_p2], child)) {

                    index_p2 = (index_p2 + 1) % p2.size();

                }
                else {

                    child[index_child] = p2[index_p2];

                    index_child = (index_child + 1) % child.size();
                    index_p2 = (index_p2 + 1) % p2.size();

                }

            }

            individual c;
            c.path = child;
            children.push_back(c);

        }
        // -----------------------------------------------------------------------------------------------------
        // Mutation

        std::vector<individual> next_generation;
        for (int i = 0; i < children.size(); i++) {

            std::vector<sf::CircleShape> c = children[i].path;

            // Swap Mutation
            for (int index = 0; index < c.size(); index++) {

                double r = ((double)rand()) / RAND_MAX;

                if (r < mutation_rate) {

                    int swap_index = rand() % c.size();

                    sf::CircleShape temp = c[index];
                    c[index] = c[swap_index];
                    c[swap_index] = temp;

                }

            }

            individual n;
            n.path = c;
            next_generation.push_back(n);

        }

        population = next_generation;

        //-----------------------------------------------------------------------------------------------
        displayBestPath(graph, population, next);
        //-----------------------------------------------------------------------------------------------

    }

    //-----------------------------------------------------------------------------------------------------------------
    displayBestPath(graph, population,--next);
    //-----------------------------------------------------------------------------------------------------------------

    // Return Shortest Route

}

void GeneticAlgorithm::display(sf::RenderWindow& window)
{

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_GeneticAlgorithm;
    text_GeneticAlgorithm.setFont(font);
    text_GeneticAlgorithm.setCharacterSize(13);
    text_GeneticAlgorithm.setFillColor(sf::Color::Cyan);
    text_GeneticAlgorithm.setPosition(810, 442);

    if (!paths.empty()) {

        std::pair<sf::VertexArray, std::tuple <float, int>> pair = paths.front();

        r = pair.first; // The path

        std::tuple <float, int> stats = pair.second;

        // Best path, Generation number
        distance = std::get<0>(stats);
        generation_num = std::get<1>(stats);

        std::string s;
        s += "[Shortest Path In Population]\n" + std::to_string(distance);
        s += "\n[Population Size]\n" + std::to_string(population_size);
        s += "\n[Elite Size]\n" + std::to_string(elite_size);
        s += "\n[Mutation Rate]\n" + std::to_string((float)mutation_rate*100) + "%";
        s += "\n[Generation #] / [# Of Generations]\n" + std::to_string(generation_num) + " / " + std::to_string(number_of_generations);

        text_GeneticAlgorithm.setString(s);
        window.draw(text_GeneticAlgorithm);
        window.draw(r);
        paths.pop();

    }
    else {

        std::string s;
        s += "[Shortest Path In Population]\n" + std::to_string(distance);
        s += "\n[Population Size]\n" + std::to_string(population_size);
        s += "\n[Elite Size]\n" + std::to_string(elite_size);
        s += "\n[Mutation Rate]\n" + std::to_string((float)mutation_rate * 100) + "%";
        s += "\n[Generation #] / [# Of Generations]\n" + std::to_string(generation_num) + " / " + std::to_string(number_of_generations);

        text_GeneticAlgorithm.setString(s);
        window.draw(text_GeneticAlgorithm);
        window.draw(r);
    }

}

void GeneticAlgorithm::terminate()
{
    FINISHED = true;
}

void GeneticAlgorithm::displayBestPath(Graph graph, std::vector<individual> population, int generation_num)
{

    float shortest = graph.PathDistance(population[0].path);
    std::vector<sf::CircleShape> best_path = population[0].path;
    for (int i = 1; i < population.size(); i++) {

        if (graph.PathDistance(population[i].path) < shortest) {

            shortest = graph.PathDistance(population[i].path);
            best_path = population[i].path;
        }

    }

    // Best path, Generation number
    std::tuple <float, int> stats;
    std::get<0>(stats) = shortest;
    std::get<1>(stats) = generation_num;

    sf::VertexArray route(sf::LinesStrip, best_path.size() + 1);
    for (int i = 0; i <= best_path.size(); i++) {
        route[i].position = sf::Vector2f(best_path[i % best_path.size()].getPosition().x + 10, best_path[i % best_path.size()].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }
    paths.push({ route, stats });

}