#include <SFML/Graphics.hpp>

#include <vector>
#include <queue>
#include <iostream>
#include <limits>
#include <thread>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <stdlib.h>    
#include <time.h>      
#include <math.h>  
#include <windows.h>

#include "Graph.h"
#include "Menu.h"

#include "BruteForce.h"
#include "Random.h"
#include "DP_Memoization.h"
#include "NearestNeighbor.h"
#include "ConvexHull.h"
#include "TwoOpt.h"
#include "SimulatedAnnealing.h"
#include "GeneticAlgorithm.h"
#include "NearestInsertion.h"
#include "ChristofidesAlgorithm.h"

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    // create the window
    // 800, 600 -> graph
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Travelling Salesman Problem Visualization", sf::Style::Titlebar | sf::Style::Close, settings);

    Graph graph;
    Menu menu;

    // Which item is selected in the menu.
    int selected = 0;

    bool draw_edges = false;

    // State Machine
    enum State {

        Start_s,
        GenerateRandomNodes_s,
        AddNodes_s,
        DisplayCompleteGraph_s,
        BruteForce_s,
        Random_s,
        DynamicProgramming_s,
        NearestNeighbor_s,
        ConvexHull_s,
        TwoOpt_s,
        SimulatedAnnealing_s,
        GeneticAlgorithm_s,
        NearestInsertion_s,
        ChristofidesAlgorithm_s

    };
    State currentState = Start_s;

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    // For "GenerateRandomNodes_s" state
    // -------------------------------------------------------------------------------------------
    std::string text_message = "Enter number of nodes (3 to 100):  \n";
    std::string input_text;
    
    sf::Text text_GenerateRandomNodes_s;
    text_GenerateRandomNodes_s.setFont(font);
    text_GenerateRandomNodes_s.setCharacterSize(20);
    text_GenerateRandomNodes_s.setFillColor(sf::Color::Black);
    text_GenerateRandomNodes_s.setStyle(sf::Text::Bold);
    text_GenerateRandomNodes_s.setPosition(810, 420);
    // -------------------------------------------------------------------------------------------
    // For "AddNodes_s" state
    sf::Text text_AddNodes_s;
    text_AddNodes_s.setFont(font);
    text_AddNodes_s.setCharacterSize(20);
    text_AddNodes_s.setFillColor(sf::Color::Black);
    text_AddNodes_s.setStyle(sf::Text::Bold);
    text_AddNodes_s.setPosition(810, 420);
    text_AddNodes_s.setString("Add nodes to the graph using your\nmouse (left click).\nPress Enter when your done.");
    // -------------------------------------------------------------------------------------------
    // For "DisplayCompleteGraph_s" state
    sf::Text text_DisplayCompleteGraph_s;
    text_DisplayCompleteGraph_s.setFont(font);
    text_DisplayCompleteGraph_s.setCharacterSize(20);
    text_DisplayCompleteGraph_s.setFillColor(sf::Color::Black);
    text_DisplayCompleteGraph_s.setStyle(sf::Text::Bold);
    text_DisplayCompleteGraph_s.setPosition(810, 420);
    text_DisplayCompleteGraph_s.setString("Press Enter to exit the current mode.");
    // -------------------------------------------------------------------------------------------
    // To terminate algorithm
    sf::Text text_Terminate;
    text_Terminate.setFont(font);
    text_Terminate.setCharacterSize(20);
    text_Terminate.setFillColor(sf::Color::Black);
    text_Terminate.setStyle(sf::Text::Bold);
    text_Terminate.setPosition(810, 420);
    text_Terminate.setString("Press Enter to exit the current mode.");

    BruteForce *brute = nullptr;
    std::thread BruteForceThread;

    Random *rand = nullptr;
    std::thread RandomThread;

    DP_Memoization *dp = nullptr;
    std::thread DPThread;

    NearestNeighbor *nearest_neighbor = nullptr;

    ConvexHull *convex = nullptr;
    std::thread ConvexHullThread;

    TwoOpt *two_opt = nullptr;
    std::thread TwoOptThread;

    SimulatedAnnealing *simulated_annealing = nullptr;
    std::thread SimulatedAnnealingThread;

    GeneticAlgorithm *genetic = nullptr;
    std::thread GeneticAlgorithmThread;

    NearestInsertion *nearest_insertion = nullptr;
    std::thread NearestInsertionThread;

    ChristofidesAlgorithm *christofides = nullptr;
    std::thread ChristofidesAlgorithmThread;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        START:
        switch (currentState) {
            case Start_s: {
                // ---------------------------------------------------------------------------------------------------------------------------------------------
                // check all the window's events that were triggered since the last iteration of the loop
                sf::Event event;
                while (window.pollEvent(event))
                {

                    switch (event.type)
                    {
                    case sf::Event::KeyReleased:
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Up:
                            menu.MoveUp();
                            break;

                        case sf::Keyboard::Down:
                            menu.MoveDown();
                            break;

                        case sf::Keyboard::Return:
                            switch (menu.GetPressedItem())
                            {
                            case 0:
                                std::cout << "Generate Random Nodes" << std::endl;
                                currentState = GenerateRandomNodes_s;
                                goto START;
                                
                                break;
                            case 1:
                                std::cout << "Add Nodes" << std::endl;
                                currentState = AddNodes_s;
                                goto START;

                                break;
                            case 2:
                                std::cout << "Clear Everything / Restart" << std::endl;
                                graph.Clear();

                                break;
                            case 3:
                                std::cout << "Display Complete Graph" << std::endl;
                                currentState = DisplayCompleteGraph_s;
                                goto START;

                                break;
                            case 4:
                                std::cout << "Brute Force (Exhaustive)" << std::endl;
                                currentState = BruteForce_s;
                                goto START;
 
                                break;
                            case 5:
                                std::cout << "Random" << std::endl;
                                currentState = Random_s;
                                goto START;

                                break;
                            case 6:
                                std::cout << "Dynamic Programming (Memoization)" << std::endl;
                                currentState = DynamicProgramming_s;
                                goto START;

                                break;
                            case 7:
                                std::cout << "Nearest Neighbor (Greedy)" << std::endl;
                                currentState = NearestNeighbor_s;
                                goto START;

                                break;
                            case 8:
                                std::cout << "Convex Hull" << std::endl;
                                currentState = ConvexHull_s;
                                goto START;

                                break;
                            case 9:
                                std::cout << "2 opt (Remove Overlap)" << std::endl;
                                currentState = TwoOpt_s;
                                goto START;
                               
                                break;
                            case 10:
                                std::cout << "Simulated Annealing" << std::endl;
                                currentState = SimulatedAnnealing_s;
                                goto START;

                                break;
                            case 11:
                                std::cout << "Genetic Algorithm" << std::endl;
                                currentState = GeneticAlgorithm_s;
                                goto START;

                                break;
                            case 12:
                                std::cout << "Nearest Insertion" << std::endl;
                                currentState = NearestInsertion_s;
                                goto START;

                                break;
                            case 13:
                                std::cout << "Christofides Algorithm" << std::endl;
                                currentState = ChristofidesAlgorithm_s;
                                goto START;

                                break;
                            }
                            break;
                        }
                        break;

                    case sf::Event::Closed:
                        window.close();
                        break;
                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...

                graph.drawNodes(window);
                menu.draw(window);

                // end the current frame
                window.display();
                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case GenerateRandomNodes_s: {

                sf::Event event;
                while (window.pollEvent(event))
                {
                  
                    if (event.type == sf::Event::Closed)
                        window.close();
                    else if (event.type == sf::Event::TextEntered) {
                        if (std::isprint(event.text.unicode))
                            if (isdigit(event.text.unicode))
                                if (input_text.size() <= 2)
                                    input_text += event.text.unicode;
                    }
                    else if (event.type == sf::Event::KeyReleased) {
                        if (event.key.code == sf::Keyboard::BackSpace) {
                            if (!input_text.empty())
                                input_text.pop_back();
                        }
                        if (event.key.code == sf::Keyboard::Return) {
                            
                            if (!input_text.empty() && 3 <= stoi(input_text) && stoi(input_text) <= 100) {
                                currentState = Start_s;
                                graph.NewGraph(stoi(input_text));
                                
                            }
                                
                        }
                    }
                  
                }

                text_GenerateRandomNodes_s.setString(text_message + input_text);

                window.clear(sf::Color(150, 150, 150));
                
                // draw everything here...
                menu.draw(window);
                window.draw(text_GenerateRandomNodes_s);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case AddNodes_s: {

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed)
                        window.close();
                    else if (event.type == sf::Event::MouseButtonPressed)
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            graph.addNode(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                        }

                    }
                    else if (event.type == sf::Event::KeyReleased) {
                        if (event.key.code == sf::Keyboard::Return) 
                            currentState = Start_s;
                    }
                    
                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                window.draw(text_AddNodes_s);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case DisplayCompleteGraph_s: {

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed)
                        window.close();
                    else if (event.type == sf::Event::KeyReleased) {
                        if (event.key.code == sf::Keyboard::Return)
                            currentState = Start_s;
                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                graph.drawEdges(window);
                window.draw(text_DisplayCompleteGraph_s);
               
                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case BruteForce_s: {

                if (graph.getNumberOfNodes() >= 9 || graph.getNumberOfNodes() <= 2) {

                    sf::Text valid_Brute_Force;
                    valid_Brute_Force.setFont(font);
                    valid_Brute_Force.setCharacterSize(20);
                    valid_Brute_Force.setFillColor(sf::Color::Black);
                    valid_Brute_Force.setStyle(sf::Text::Bold);
                    valid_Brute_Force.setPosition(810, 420);

                    valid_Brute_Force.setString("Valid number of nodes in range [3,8].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_Brute_Force);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------

                if (brute == nullptr) {

                    brute = new BruteForce(graph);
                    
                    // brute->computePath() runs in another thread
                    BruteForceThread = std::thread(&BruteForce::computePath, std::ref(*brute));
                    
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                    
                        brute->terminate(); // Terminates brute->computePath() => BruteForceThread ends
                        BruteForceThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {
                  
                            brute->terminate(); // Terminates brute->computePath() => BruteForceThread ends
                            BruteForceThread.join();

                            delete brute;
                            brute = nullptr;
                            currentState = Start_s;
                            goto START;
          
                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                brute->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case Random_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_random;
                    valid_random.setFont(font);
                    valid_random.setCharacterSize(18);
                    valid_random.setFillColor(sf::Color::Black);
                    valid_random.setStyle(sf::Text::Bold);
                    valid_random.setPosition(810, 420);

                    valid_random.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_random);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------

                if (rand == nullptr) {

                    rand = new Random(graph, 5000);

                    // rand->computePath() runs in another thread
                    RandomThread = std::thread(&Random::computePath, std::ref(*rand));

                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {

                        rand->terminate(); // Terminates rand->computePath() => RandomThread ends
                        RandomThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        rand->terminate(); // Terminates rand->computePath() => RandomThread ends
                        RandomThread.join();

                        delete rand;
                        rand = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                rand->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case DynamicProgramming_s: {

                if (graph.getNumberOfNodes() >= 15 || graph.getNumberOfNodes() <= 2) {

                    sf::Text valid_dp;
                    valid_dp.setFont(font);
                    valid_dp.setCharacterSize(18);
                    valid_dp.setFillColor(sf::Color::Black);
                    valid_dp.setStyle(sf::Text::Bold);
                    valid_dp.setPosition(810, 420);

                    valid_dp.setString("Valid number of nodes in range [3,14].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_dp);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------

                if (dp == nullptr) {

                    dp = new DP_Memoization(graph);

                    // dp->computePath() runs in another thread
                    DPThread = std::thread(&DP_Memoization::computePath, std::ref(*dp));

                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        dp->terminate(); // Terminates dp->computePath() => DPThread ends
                        DPThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        dp->terminate(); // Terminates dp->computePath() => DPThread ends
                        DPThread.join();

                        delete dp;
                        dp = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                dp->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case NearestNeighbor_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_nn;
                    valid_nn.setFont(font);
                    valid_nn.setCharacterSize(18);
                    valid_nn.setFillColor(sf::Color::Black);
                    valid_nn.setStyle(sf::Text::Bold);
                    valid_nn.setPosition(810, 420);

                    valid_nn.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_nn);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------

                if (nearest_neighbor == nullptr) {

                    nearest_neighbor = new NearestNeighbor(graph);
                    nearest_neighbor->computePath();
                   
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        delete nearest_neighbor;
                        nearest_neighbor = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                nearest_neighbor->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();
                Sleep(100);
                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case ConvexHull_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_convex_hull;
                    valid_convex_hull.setFont(font);
                    valid_convex_hull.setCharacterSize(18);
                    valid_convex_hull.setFillColor(sf::Color::Black);
                    valid_convex_hull.setStyle(sf::Text::Bold);
                    valid_convex_hull.setPosition(810, 420);

                    valid_convex_hull.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_convex_hull);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------

                if (convex == nullptr)
                {
                    convex = new ConvexHull(graph);

                    // convex->computePath() runs in another thread
                    ConvexHullThread = std::thread(&ConvexHull::computePath, std::ref(*convex));

                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        convex->terminate(); // Terminates convex->computePath() => ConvexHullThread ends
                        ConvexHullThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        convex->terminate(); // Terminates convex->computePath() => ConvexHullThread ends
                        ConvexHullThread.join();

                        delete convex;
                        convex = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                convex->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();
                Sleep(10);
                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case TwoOpt_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_two_opt;
                    valid_two_opt.setFont(font);
                    valid_two_opt.setCharacterSize(18);
                    valid_two_opt.setFillColor(sf::Color::Black);
                    valid_two_opt.setStyle(sf::Text::Bold);
                    valid_two_opt.setPosition(810, 420);

                    valid_two_opt.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_two_opt);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------
                if (two_opt == nullptr)
                {
                    two_opt = new TwoOpt(graph);

                    // two_opt->computePath() runs in another thread
                    TwoOptThread = std::thread(&TwoOpt::computePath, std::ref(*two_opt));
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        two_opt->terminate(); // Terminates two_opt->computePath() => TwoOptThread ends
                        TwoOptThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        two_opt->terminate(); // Terminates two_opt->computePath() => TwoOptThread ends
                        TwoOptThread.join();

                        delete two_opt;
                        two_opt = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                two_opt->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();
             
                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case SimulatedAnnealing_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_simulated_annealing;
                    valid_simulated_annealing.setFont(font);
                    valid_simulated_annealing.setCharacterSize(18);
                    valid_simulated_annealing.setFillColor(sf::Color::Black);
                    valid_simulated_annealing.setStyle(sf::Text::Bold);
                    valid_simulated_annealing.setPosition(810, 420);

                    valid_simulated_annealing.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_simulated_annealing);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------
                if (simulated_annealing == nullptr)
                {
                    simulated_annealing = new SimulatedAnnealing(graph, 7500, 100000, 0.99);

                    // simulated_annealing->computePath() runs in another thread
                    SimulatedAnnealingThread = std::thread(&SimulatedAnnealing::computePath, std::ref(*simulated_annealing));
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        simulated_annealing->terminate(); // Terminates simulated_annealing->computePath() => SimulatedAnnealingThread ends
                        SimulatedAnnealingThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        simulated_annealing->terminate(); // Terminates simulated_annealing->computePath() => SimulatedAnnealingThread ends
                        SimulatedAnnealingThread.join();

                        delete simulated_annealing;
                        simulated_annealing = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                simulated_annealing->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case GeneticAlgorithm_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_GeneticAlgorithm;
                    valid_GeneticAlgorithm.setFont(font);
                    valid_GeneticAlgorithm.setCharacterSize(18);
                    valid_GeneticAlgorithm.setFillColor(sf::Color::Black);
                    valid_GeneticAlgorithm.setStyle(sf::Text::Bold);
                    valid_GeneticAlgorithm.setPosition(810, 420);

                    valid_GeneticAlgorithm.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_GeneticAlgorithm);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------
                if (genetic == nullptr)
                {
                    genetic = new GeneticAlgorithm(graph);

                    // genetic->computePath() runs in another thread
                    GeneticAlgorithmThread = std::thread(&GeneticAlgorithm::computePath, std::ref(*genetic));
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        genetic->terminate(); // Terminates genetic->computePath() => GeneticAlgorithmThread ends
                        GeneticAlgorithmThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        genetic->terminate(); // Terminates genetic->computePath() => GeneticAlgorithmThread ends
                        GeneticAlgorithmThread.join();

                        delete genetic;
                        genetic = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                genetic->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case NearestInsertion_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 101) {

                    sf::Text valid_nearest_insertion;
                    valid_nearest_insertion.setFont(font);
                    valid_nearest_insertion.setCharacterSize(18);
                    valid_nearest_insertion.setFillColor(sf::Color::Black);
                    valid_nearest_insertion.setStyle(sf::Text::Bold);
                    valid_nearest_insertion.setPosition(810, 420);

                    valid_nearest_insertion.setString("Valid number of nodes in range [3,100].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_nearest_insertion);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }

                //-------------------------------------------------------------------------------------------------------------------------------------
                
                if (nearest_insertion == nullptr)
                {
                    nearest_insertion = new NearestInsertion(graph);

                    // nearest_insertion->computePath() runs in another thread
                    NearestInsertionThread = std::thread(&NearestInsertion::computePath, std::ref(*nearest_insertion));
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        nearest_insertion->terminate(); // Terminates nearest_insertion->computePath() => NearestInsertionThread ends
                        NearestInsertionThread.join();
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        nearest_insertion->terminate(); // Terminates nearest_insertion->computePath() => NearestInsertionThread ends
                        NearestInsertionThread.join();

                        delete nearest_insertion;
                        nearest_insertion = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                nearest_insertion->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
            // ---------------------------------------------------------------------------------------------------------------------------------------------
            case ChristofidesAlgorithm_s: {

                if (graph.getNumberOfNodes() <= 2 || graph.getNumberOfNodes() >= 21) {

                    sf::Text valid_ChristofidesAlgorithm;
                    valid_ChristofidesAlgorithm.setFont(font);
                    valid_ChristofidesAlgorithm.setCharacterSize(18);
                    valid_ChristofidesAlgorithm.setFillColor(sf::Color::Black);
                    valid_ChristofidesAlgorithm.setStyle(sf::Text::Bold);
                    valid_ChristofidesAlgorithm.setPosition(810, 420);

                    valid_ChristofidesAlgorithm.setString("Valid number of nodes in range [3,20].");

                    window.clear(sf::Color(150, 150, 150));

                    // draw everything here...
                    graph.drawNodes(window);
                    menu.draw(window);
                    window.draw(valid_ChristofidesAlgorithm);

                    // end the current frame
                    window.display();

                    Sleep(2000);
                    currentState = Start_s;
                    goto START;

                }
       
                //-------------------------------------------------------------------------------------------------------------------------------------
                if (christofides == nullptr)
                {
                    christofides = new ChristofidesAlgorithm(graph);

                    // christofides->computePath() runs in another thread
                    ChristofidesAlgorithmThread = std::thread(&ChristofidesAlgorithm::computePath, std::ref(*christofides));
                }

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {

                        christofides->terminate(); // Terminates christofides->computePath() => ChristofidesAlgorithmThread ends
                        ChristofidesAlgorithmThread.join();
                        window.close();

                    }
                    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {

                        christofides->terminate(); // Terminates christofides->computePath() => ChristofidesAlgorithmThread ends
                        ChristofidesAlgorithmThread.join();

                        delete christofides;
                        christofides = nullptr;
                        currentState = Start_s;
                        goto START;

                    }

                }

                window.clear(sf::Color(150, 150, 150));

                // draw everything here...
                graph.drawNodes(window);
                menu.draw(window);
                christofides->display(window);
                window.draw(text_Terminate);

                // end the current frame
                window.display();

                break;
            }
                        
        }
    }

    return 0;
}