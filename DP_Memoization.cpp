#include "DP_Memoization.h"

DP_Memoization::DP_Memoization(Graph graph) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    total_number_of_subproblems = (NUM_OF_CITIES - 1) * (pow(2, NUM_OF_CITIES - 2));
    subproblem_count = 0;
    FINISHED = false;
    best_distance = 0;
}

// Convert a set to a string
std::string DP_Memoization::encode(std::set<int> s)
{

    std::string code = "";
    for (int n : s)
        code += std::to_string(n) + " ";

    return code;
    
}

// Held–Karp algorithm - Dynamic Programming - Top Down Approach - Memoization
// i = Current node
// s = Set of nodes to choose next
// dp = Cache for subproblem solution
//      (node, set of nodes (in string format)) -> min path length from node to end of tree
// p = Memory for retrieving the optimal path
//     (node, set of nodes (in string format)) -> next node in optimal path

float DP_Memoization::g(int i, std::set<int> s, std::unordered_map<int, std::unordered_map<std::string, float>>& dp, std::unordered_map<int, std::unordered_map<std::string, int>>& p)
{
    // Terminates thread
    if (FINISHED)
        return 0;

    std::vector<std::vector<float>> adjm = graph.getAdjacencyMatrix();

    // If subproblem solution exists in memory, then return the solution
    if (dp[i][encode(s)] != 0)
        return dp[i][encode(s)];
    
    subproblem_count++;

    // Base case
    // If the set is empty, then save and return the distance between the current node and the start node
    if (s.empty()) {
        p[i][encode(s)] = 0; // p(node, {}) = starting node (the next node in the optimal path)
        return dp[i][encode(s)] = adjm[i][0];
    }

    // "res" is the solution to the subproblem
    float res = INT_MAX;

    // g(i, S) = min (k is an element of S) { c_ik + g(k, S - {k}) }

    // To simplify the set difference
    std::set<int> set_minus_k(s.begin(), s.end());

    // For each node in the set
    for (int k : s) {
        set_minus_k.erase(k); // S - {k}

        // g(i, S) = min (k is an element of S) { c_ik + g(k, S - {k}) }
        if (adjm[i][k] + g(k, set_minus_k, dp, p) < res) {

            // min (k is an element of S) { c_ik + g(k, S - {k}) }
            res = adjm[i][k] + g(k, set_minus_k, dp, p);

            // Save the path
            // (current node, set of nodes to visit) -> next node in path
            p[i][encode(s)] = k;

        }

        set_minus_k.insert(k); //  S + {k}
    }

    // Save and return the solution
    return dp[i][encode(s)] = res;

}

void DP_Memoization::computePath()
{

    // Set of nodes to visit
    std::set<int> s;
    for (int i = 1; i < NUM_OF_CITIES; i++) {
        s.insert(i);
    }

    // Cache for subproblem solutions
    std::unordered_map<int, std::unordered_map<std::string, float>> dp;

    // Memory for retrieving the optimal path
    std::unordered_map<int, std::unordered_map<std::string, int>> p;

    // Solve the TSP (starting at node 0)
    best_distance = g(0, s, dp, p);

    // Terminates thread
    if (FINISHED)
        return;

    std::vector<int> path;

    int node = 0;
    while (true) {
        path.push_back(node);
        node = p[node][encode(s)];
        if (!s.empty())
            s.erase(node);
        else
            break;
    }

    sf::VertexArray route(sf::LinesStrip, (NUM_OF_CITIES + 1));
    for (int i = 0; i < NUM_OF_CITIES; i++) {
        int n = path[i];
        route[i].position = sf::Vector2f(graph.getNodes()[n].getPosition().x + 10, graph.getNodes()[n].getPosition().y + 10);
        route[i].color = sf::Color(0, 255, 0);
    }
    route[NUM_OF_CITIES].position = sf::Vector2f(graph.getNodes()[path[0]].getPosition().x + 10, graph.getNodes()[path[0]].getPosition().y + 10);
    route[NUM_OF_CITIES].color = sf::Color(0, 255, 0);
    
    solution = route;
    
}

void DP_Memoization::display(sf::RenderWindow& window) {
    
    if (subproblem_count > total_number_of_subproblems)
        subproblem_count = total_number_of_subproblems;

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_dp;
    text_dp.setFont(font);
    text_dp.setCharacterSize(15);
    text_dp.setFillColor(sf::Color::Cyan);
    text_dp.setPosition(810, 450);

    std::string s1 = "[# Of Subproblems Solved] / [Total # Of Subproblems]\n" + std::to_string(subproblem_count) + " / " + std::to_string(total_number_of_subproblems);
    std::string s2;
    if(best_distance == 0)
        s2 = "";
    else
        s2 = "[Best Distance]\n" + std::to_string(best_distance);
    
    text_dp.setString(s1 + "\n\n" + s2);
    window.draw(text_dp);

    window.draw(solution);

}

void DP_Memoization::terminate()
{
    FINISHED = true;
}
