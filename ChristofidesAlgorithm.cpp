#include "ChristofidesAlgorithm.h"

// For std::unordered_map<sf::CircleShape, std::vector<sf::CircleShape>> adjList----------------------------------------------------
namespace std {

    template<> struct hash<sf::CircleShape> {

        std::size_t operator()(sf::CircleShape const& obj) const {
            size_t rowHash = std::hash<float>()(obj.getPosition().x);
            size_t colHash = std::hash<float>()(obj.getPosition().y) << 1;
            return rowHash ^ colHash;
        }

    };

    bool operator==(const sf::CircleShape lhs, const sf::CircleShape rhs) {
        return (lhs.getPosition().x == rhs.getPosition().x) && (lhs.getPosition().y == rhs.getPosition().y);
    }

}
// -------------------------------------------------------------------------------------------------------------------------------

ChristofidesAlgorithm::ChristofidesAlgorithm(Graph graph) : graph(graph)
{
    NUM_OF_CITIES = graph.getNumberOfNodes();
    FINISHED = false;

    // ------------------------------------------------

    PerfectMatchingCount = 0;
    NumberOfPerfectMatchings = 0;
    FinalDistance = 0;

}

void ChristofidesAlgorithm::computePath()
{
    // Get a vector of nodes (cities)
    std::vector<sf::CircleShape> nodes = graph.getNodes();

    //------------------------------------------------------------------------------------------------------------------------------------
    // 1:  Minimum spanning tree

    // Adjacency list for MST
    std::unordered_map<sf::CircleShape, std::vector<sf::CircleShape>> adjList;

    std::unordered_set<sf::CircleShape> visited;
    visited.insert(nodes[0]);

    std::unordered_set<sf::CircleShape> not_visited(nodes.begin() + 1, nodes.end());

    while (visited.size() != nodes.size()) {

        sf::CircleShape from;
        sf::CircleShape to;
        float min_distance = std::numeric_limits<float>::max();;

        for (sf::CircleShape node_in_mst : visited) {
            for (sf::CircleShape node : not_visited) {

                if (graph.Distance(node_in_mst, node) < min_distance) {

                    from = node_in_mst;
                    to = node;
                    min_distance = graph.Distance(node_in_mst, node);

                }

            }
        }

        adjList[from].push_back(to);
        adjList[to].push_back(from);
        visited.insert(to);
        not_visited.erase(to);

    }

    for (sf::CircleShape k : nodes) {

        for (sf::CircleShape v : adjList[k]) {

            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(k.getPosition().x + 10, k.getPosition().y + 10);
            line[0].color = sf::Color(0, 255, 0);
            line[1].position = sf::Vector2f(v.getPosition().x + 10, v.getPosition().y + 10);
            line[1].color = sf::Color(0, 255, 0);
            q_mst.push(line);

        }

    }

    Sleep(1000);

    // 2:  Find vertices with odd degree

    std::vector<sf::CircleShape> odd;

    for (sf::CircleShape k : nodes) {

        if (adjList[k].size() % 2 == 1) {
            odd.push_back(k);
            k.setFillColor(sf::Color(0, 0, 255));
            q_odd_circle.push(k);
        }

    }

    Sleep(1000);

    // 3:  Find a minimum-weight perfect matching
    std::vector<std::pair<sf::CircleShape, sf::CircleShape>> pm;

    float minimum_weight = std::numeric_limits<float>::max();;
    std::vector<std::pair<sf::CircleShape, sf::CircleShape>> min_weight_pm;

    NumberOfPerfectMatchings = number_of_perfect_matchings(static_cast<int>(odd.size()));
    
    backtracking(graph, odd, pm, minimum_weight, min_weight_pm);
    
    if (FINISHED)
        return;

    for (std::pair<sf::CircleShape, sf::CircleShape> e : min_weight_pm) {

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(e.first.getPosition().x + 10, e.first.getPosition().y + 10);
        line[0].color = sf::Color(0, 0, 255);
        line[1].position = sf::Vector2f(e.second.getPosition().x + 10, e.second.getPosition().y + 10);
        line[1].color = sf::Color(0, 0, 255);
        q_pm.push(line);

    }

    Sleep(1000);

    // 4:  Combine 1 and 3
    for (std::pair<sf::CircleShape, sf::CircleShape> e : min_weight_pm) {

        sf::CircleShape from = e.first;
        sf::CircleShape to = e.second;
        adjList[from].push_back(to);
        adjList[to].push_back(from);

    }

    for (sf::CircleShape c : nodes) {

        c.setFillColor(sf::Color(0, 0, 0));
        q_clear_circle.push(c);

    }

    for (sf::CircleShape k : nodes) {

        for (sf::CircleShape v : adjList[k]) {

            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(k.getPosition().x + 10, k.getPosition().y + 10);
            line[0].color = sf::Color(255, 255, 255);
            line[1].position = sf::Vector2f(v.getPosition().x + 10, v.getPosition().y + 10);
            line[1].color = sf::Color(255, 255, 255);
            q_clear_merge.push(line);

        }

    }

    Sleep(1000);

    // 5:  Form an Eulerian circuit
    sf::CircleShape current = nodes[0]; // Starting node
    sf::CircleShape next;

    std::stack<sf::CircleShape> s;
    s.push(current);

    std::vector<sf::CircleShape> EulerianCircuit;

    while (true) {

        if (adjList[current].empty()) {

            EulerianCircuit.insert(EulerianCircuit.begin(), s.top());
            s.pop();

            if (s.empty())
                break;
            else
                current = s.top();

        }
        else {

            // Get the next node
            next = adjList[current].front();

            // Remove (current-next) edge from adjacency list
            //--------------------------------------------------------------------------------------------------------------
            adjList[current].erase(adjList[current].begin());

            for (int i = 0; i < adjList[next].size(); i++) {

                if (adjList[next][i].getPosition().x == current.getPosition().x && adjList[next][i].getPosition().y == current.getPosition().y) {

                    adjList[next].erase(adjList[next].begin() + i);
                    break;

                }

            }
            //--------------------------------------------------------------------------------------------------------------

            current = next;
            s.push(current);

        }

    }

    // 6:  Use 5 to create a Hamiltonian circuit
    std::vector<sf::CircleShape> HamiltonianCircuit;
    std::unordered_set<sf::CircleShape> seen;

    for (sf::CircleShape n : EulerianCircuit) {

        if (seen.count(n) == 0) {
            HamiltonianCircuit.push_back(n);
            seen.insert(n);
        }
        else {
            continue;
        }

    }

    for (int i = 0; i < HamiltonianCircuit.size(); i++) {

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(HamiltonianCircuit[i].getPosition().x + 10, HamiltonianCircuit[i].getPosition().y + 10);
        line[0].color = sf::Color(0, 255, 0);
        line[1].position = sf::Vector2f(HamiltonianCircuit[(i + 1) % HamiltonianCircuit.size()].getPosition().x + 10, HamiltonianCircuit[(i + 1) % HamiltonianCircuit.size()].getPosition().y + 10);
        line[1].color = sf::Color(0, 255, 0);
        q_hamiltonian.push(line);

        Sleep(200);

    }

    Sleep(1000);
    finished = true;

    FinalDistance = graph.PathDistance(HamiltonianCircuit);

}

void ChristofidesAlgorithm::display(sf::RenderWindow& window)
{

    sf::Font font;
    font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

    sf::Text text_ChristofidesAlgorithm;
    text_ChristofidesAlgorithm.setFont(font);
    text_ChristofidesAlgorithm.setCharacterSize(16);
    text_ChristofidesAlgorithm.setFillColor(sf::Color::Cyan);
    text_ChristofidesAlgorithm.setPosition(810, 442);
    
    std::string s;
    s += "[Final Distance]\n" + std::to_string(FinalDistance);
    s += "\n[Perfect Matching #] / [# Of Perfect Matchings]\n" + std::to_string(PerfectMatchingCount) + " / " + std::to_string(NumberOfPerfectMatchings);

    text_ChristofidesAlgorithm.setString(s);
    window.draw(text_ChristofidesAlgorithm);

    if (finished == false) {

        // MST
        if (!q_mst.empty()) {
            mst.push_back(q_mst.front());
            q_mst.pop();
        }
        else {
            for (sf::VertexArray l : mst)
                window.draw(l);
        }

        // Odd degree nodes
        if (!q_odd_circle.empty()) {
            odd.push_back(q_odd_circle.front());
            q_odd_circle.pop();
        }
        else {
            for (sf::CircleShape c : odd)
                window.draw(c);
        }

        // Perfect matching
        if (!q_pm.empty()) {
            matching.push_back(q_pm.front());
            q_pm.pop();
        }
        else {
            for (sf::VertexArray l : matching)
                window.draw(l);
        }

        // Clear circles
        if (!q_clear_circle.empty()) {
            clear_circle.push_back(q_clear_circle.front());
            q_clear_circle.pop();
        }
        else {
            for (sf::CircleShape c : clear_circle)
                window.draw(c);
        }

        // Clear edges and merge all edges
        if (!q_clear_merge.empty()) {
            merge.push_back(q_clear_merge.front());
            q_clear_merge.pop();
        }
        else {
            for (sf::VertexArray l : merge)
                window.draw(l);
        }

        // Hamiltonian
        if (!q_hamiltonian.empty()) {
            hamiltonian.push_back(q_hamiltonian.front());
            q_hamiltonian.pop();
        }
        else {
            for (sf::VertexArray l : hamiltonian)
                window.draw(l);
        }

    }
    else {

        for (sf::VertexArray l : hamiltonian)
            window.draw(l);

    }

}

void ChristofidesAlgorithm::terminate()
{
    FINISHED = true;
}

// Compute all perfect matchings
// Time Complexity = (n-1)(n-3)(n-5)(n-7)***(7)(5)(3)(1) = O(n!)
void ChristofidesAlgorithm::backtracking(Graph graph, std::vector<sf::CircleShape>& odd, std::vector<std::pair<sf::CircleShape, sf::CircleShape>>& pm, float& minimum_weight, std::vector<std::pair<sf::CircleShape, sf::CircleShape>>& min_weight_pm)
{
    if (FINISHED)
        return;

    if (odd.size() == 2) {

        pm.push_back({ odd[0], odd[1] });

        float weight_sum = 0;
        for (int i = 0; i < pm.size(); i++)
            weight_sum += graph.Distance(pm[i].first, pm[i].second);

        if (weight_sum < minimum_weight) {
            minimum_weight = weight_sum;
            min_weight_pm = pm;
        }

        pm.pop_back();

        PerfectMatchingCount++;
        return;
    }

    int i = 0;
    for (int j = 1; j < odd.size(); j++) {

        sf::CircleShape one = odd[i];
        sf::CircleShape two = odd[j];

        pm.push_back({ one, two });
        odd.erase(odd.begin() + j);
        odd.erase(odd.begin() + i);

        backtracking(graph, odd, pm, minimum_weight, min_weight_pm);

        odd.insert(odd.begin() + i, one);
        odd.insert(odd.begin() + j, two);
        pm.pop_back();

    }

}

int ChristofidesAlgorithm::number_of_perfect_matchings(int number_of_odd_degree_nodes)
{
    if (number_of_odd_degree_nodes - 1 == 1)
        return 1;
    return (number_of_odd_degree_nodes - 1) * number_of_perfect_matchings(number_of_odd_degree_nodes - 2);
}