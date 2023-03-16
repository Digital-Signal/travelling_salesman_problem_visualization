#include "ConvexHull.h"

ConvexHull::ConvexHull(Graph graph) : graph(graph)
{
	NUM_OF_CITIES = graph.getNumberOfNodes();
	final_distance = 0;
    FINISHED = false;
}

// Terminates computePath() -> If computePath() is invoked by a thread and terminate() is called, then the thread will end
void ConvexHull::terminate()
{
    FINISHED = true;
}


void ConvexHull::computePath()
{
    // Get all the nodes
    std::vector<sf::CircleShape> S = graph.getNodes();

    // Vector to hold the convex hull
    std::vector<sf::CircleShape> P;

    // Get the leftmost point in S
    sf::CircleShape pointOnHull = S[0];
    for (sf::CircleShape n : S) {

        if (n.getPosition().x < pointOnHull.getPosition().x)
            pointOnHull = n;

    }

    // Use "firstPointOnHull" to break the while loop
    sf::CircleShape firstPointOnHull = pointOnHull;

    while (true) {

        if (FINISHED)
            return;

        // Add "pointOnHull" to the convex hull
        P.push_back(pointOnHull);

        // Choose a possible endpoint (this could be the next point in the convex hull)
        // pointOnHull != endpoint
        sf::CircleShape endpoint;
        for (sf::CircleShape p : S) {
            if ((pointOnHull.getPosition().x == p.getPosition().x) && (pointOnHull.getPosition().y == p.getPosition().y)) {
                continue;
            }
            else {
                endpoint = p;
                break;
            }
        }

        // For all the points (excluding "pointOnHull" and current "endpoint")
        for (sf::CircleShape point : S) {

            // pointOnHull != point
            if ((pointOnHull.getPosition().x == point.getPosition().x) && (pointOnHull.getPosition().y == point.getPosition().y)) {
                continue;
            }

            // endpoint != point
            if ((endpoint.getPosition().x == point.getPosition().x) && (endpoint.getPosition().y == point.getPosition().y)) {
                continue;
            }

            sf::VertexArray line(sf::LinesStrip, 2);
            line[0].position = sf::Vector2f(pointOnHull.getPosition().x + 10, pointOnHull.getPosition().y + 10);
            line[0].color = sf::Color::Black;
            line[1].position = sf::Vector2f(point.getPosition().x + 10, point.getPosition().y + 10);
            line[1].color = sf::Color::Black;
            q.push({ line, 0 });

            // Compute the cross product to obtain the orientation of the two vectors
            // Use the orientation of the two vectors to update the endpoint

            // Vector A = pointOnHull to point
            // Vector B = pointOnHull to endpoint
            std::pair<float, float> vector_a = { point.getPosition().x - pointOnHull.getPosition().x, point.getPosition().y - pointOnHull.getPosition().y };
            std::pair<float, float> vector_b = { endpoint.getPosition().x - pointOnHull.getPosition().x, endpoint.getPosition().y - pointOnHull.getPosition().y };

            // A x B = a1*b2 - a2*b1
            float AxB = (vector_a.first * vector_b.second) - (vector_a.second * vector_b.first);

            // If A x B is (-): (pointOnHull to endpoint) is counterclockwise to (pointOnHull to point) [no update]
            // If A x B is (+): (pointOnHull to point) is counterclockwise to (pointOnHull to endpoint) [update endpoint]
            // If A x B is zero: (pointOnHull to point) and (pointOnHull to endpoint) are collinear.

            if (AxB > 0) {
                endpoint = point;

                sf::VertexArray line(sf::LinesStrip, 2);
                line[0].position = sf::Vector2f(pointOnHull.getPosition().x + 10, pointOnHull.getPosition().y + 10);
                line[0].color = sf::Color::Green;
                line[1].position = sf::Vector2f(point.getPosition().x + 10, point.getPosition().y + 10);
                line[1].color = sf::Color::Green;
                q.push({ line, 1 });
            }

            if (AxB == 0)
                std::cout << "(pointOnHull to point) & (pointOnHull to endpoint) are collinear";

        }

        sf::VertexArray line(sf::LinesStrip, 2);
        line[0].position = sf::Vector2f(pointOnHull.getPosition().x + 10, pointOnHull.getPosition().y + 10);
        line[0].color = sf::Color::Green;
        line[1].position = sf::Vector2f(endpoint.getPosition().x + 10, endpoint.getPosition().y + 10);
        line[1].color = sf::Color::Green;
        q.push({ line, 2 });

        // Update the next point on the convex hull
        pointOnHull = endpoint;

        // If the next point on the convex hull is the starting point, then exit the loop
        if ((pointOnHull.getPosition().x == firstPointOnHull.getPosition().x) && (pointOnHull.getPosition().y == firstPointOnHull.getPosition().y))
            break;

    }

    // Draw convex hull
    sf::VertexArray hull(sf::LinesStrip, P.size() + 1);
    for (int i = 0; i < P.size(); i++) {
        hull[i].position = sf::Vector2f(P[i].getPosition().x + 10, P[i].getPosition().y + 10);
        hull[i].color = sf::Color(0, 255, 0);
    }
    hull[P.size()].position = sf::Vector2f(P[0].getPosition().x + 10, P[0].getPosition().y + 10);
    hull[P.size()].color = sf::Color(0, 255, 0);
    q.push({ hull, 3 });

    while (P.size() != S.size()) {

        if (FINISHED)
            return;

        struct triplet {
            sf::CircleShape r;
            sf::CircleShape i;
            sf::CircleShape j;
        };

        std::vector<triplet> i_j_r;

        for (sf::CircleShape point : S) {

            if (graph.NodeInPath(point, P) == true)
                continue; // The point is in the convex hull

            // The point is not in the convex hull (sub-tour)

            float minimum_cost = std::numeric_limits<float>::max();;
            triplet rij;

            for (int i = 0; i < P.size(); i++) {

                if (i == P.size() - 1) {

                    if ((graph.Distance(P[i], point) + graph.Distance(point, P[0]) - graph.Distance(P[i], P[0])) < minimum_cost) {

                        minimum_cost = graph.Distance(P[i], point) + graph.Distance(point, P[0]) - graph.Distance(P[i], P[0]);
                        rij.r = point;
                        rij.i = P[i];
                        rij.j = P[0];

                    }

                }
                else {

                    if ((graph.Distance(P[i], point) + graph.Distance(point, P[i + 1]) - graph.Distance(P[i], P[i + 1])) < minimum_cost) {

                        minimum_cost = graph.Distance(P[i], point) + graph.Distance(point, P[i + 1]) - graph.Distance(P[i], P[i + 1]);
                        rij.r = point;
                        rij.i = P[i];
                        rij.j = P[i + 1];

                    }

                }

            }

            i_j_r.push_back(rij);

        }

        triplet i_r_j_prime;
        float minimum_cost = std::numeric_limits<float>::max();;

        for (triplet ijr : i_j_r) {

            if (((graph.Distance(ijr.i, ijr.r) + graph.Distance(ijr.r, ijr.j)) / graph.Distance(ijr.i, ijr.j)) < minimum_cost) {

                minimum_cost = ((graph.Distance(ijr.i, ijr.r) + graph.Distance(ijr.r, ijr.j)) / graph.Distance(ijr.i, ijr.j));
                i_r_j_prime.r = ijr.r;
                i_r_j_prime.i = ijr.i;
                i_r_j_prime.j = ijr.j;

            }

        }

        for (int i = 0; i < P.size(); i++) {

            if (i == P.size() - 1) {
                if ((P[i].getPosition().x == i_r_j_prime.i.getPosition().x) && (P[i].getPosition().y == i_r_j_prime.i.getPosition().y))
                    if ((P[0].getPosition().x == i_r_j_prime.j.getPosition().x) && (P[0].getPosition().y == i_r_j_prime.j.getPosition().y)) {
                        P.insert(P.begin() + i + 1, i_r_j_prime.r);
                        break;
                    }

            }
            else {
 
                if ((P[i].getPosition().x == i_r_j_prime.i.getPosition().x) && (P[i].getPosition().y == i_r_j_prime.i.getPosition().y))
                    if ((P[i + 1].getPosition().x == i_r_j_prime.j.getPosition().x) && (P[i + 1].getPosition().y == i_r_j_prime.j.getPosition().y)) {
                        P.insert(P.begin() + i + 1, i_r_j_prime.r);
                        break;
                    }

            }

        }

        // Draw updated sub-tour
        sf::VertexArray sub_tour(sf::LinesStrip, P.size() + 1);
        for (int i = 0; i < P.size(); i++) {
            sub_tour[i].position = sf::Vector2f(P[i].getPosition().x + 10, P[i].getPosition().y + 10);
            sub_tour[i].color = sf::Color(0, 255, 0);
        }
        sub_tour[P.size()].position = sf::Vector2f(P[0].getPosition().x + 10, P[0].getPosition().y + 10);
        sub_tour[P.size()].color = sf::Color(0, 255, 0);
        q.push({ sub_tour, 3 });

    }

    final_distance = graph.PathDistance(P);

}

void ConvexHull::display(sf::RenderWindow& window)
{

    if (!q.empty()) {

        std::pair<sf::VertexArray, int> ll = q.front();

        if (ll.second == 0) { // The line is to the right
            window.draw(ll.first); // B line
            window.draw(pointOnHull_to_endpoint); // G line

            for (auto &l : hull) // Draw the convex hull
                window.draw(l);
        }
        if (ll.second == 1) { // The line is to the left
            pointOnHull_to_endpoint = ll.first; // Update G line
            window.draw(ll.first); // G line
            window.draw(pointOnHull_to_endpoint); // G line

            for (auto &l : hull) // Draw the convex hull
                window.draw(l);
        }
        if (ll.second == 2) { // The leftmost line found
            hull.push_back(ll.first); // Build convex hull
            window.draw(ll.first); // G line
            window.draw(pointOnHull_to_endpoint); // G line

            for (auto &l : hull) // Draw the convex hull
                window.draw(l);
        }
        if (ll.second == 3) { // Convex Hull complete -> update sub-tour
            window.draw(ll.first);
            tour = ll.first;
        }

        q.pop();

    }
    else {

        sf::Font font;
        font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

        sf::Text text_convex_hull;
        text_convex_hull.setFont(font);
        text_convex_hull.setCharacterSize(20);
        text_convex_hull.setFillColor(sf::Color::Cyan);
        text_convex_hull.setStyle(sf::Text::Bold);
        text_convex_hull.setPosition(810, 450);
        text_convex_hull.setString("[Final Distance]\n" + std::to_string(final_distance));
        window.draw(text_convex_hull);

        window.draw(tour); // Draw TSP tour
    }

}
