#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

unsigned int window_size = 1280;
const int count = 640;

/**
 * Draws a white line from the given start point to the given end point
 *
 * Runs in \f$O(1)\f$
 * @param window The RenderWindow in which the line gets drawn
 * @param start The starting point of the line
 * @param end  The end point of the line
 */
void line(sf::RenderWindow *window, sf::Vector2i *start, sf::Vector2i *end) {
    // Create vertex
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(end->x, end->y)),
                    sf::Vertex(sf::Vector2f(start->x, start->y))
            };

    line->color = sf::Color::White;

    window->draw(line, 2, sf::Lines); // Draw a line with 2 segments
}

/**
 * Calculates the coordinates \f$(x,y)\f$ of a point on a circle, offset by an angle \f$\varphi\f$.
 *
 * Runs in \f$O(1)\f$
 * @param angle The angle \f$\varphi\f$ in which the angle is offset clockwise from \f$(c_x, c_y - r)\f$
 * @param radius The circle's radius \f$r\f$
 * @param center The center coordinates of the circle \f$(c_x,c_y)\f$
 * @return The coordinates of the point
 */
sf::Vector2i *pointOnCircle(double angle, const float radius, sf::Vector2i *center) {
    // Calculate point on unit circle
    sf::Vector2f unitCirclePoint(static_cast<float>(sin(M_PI - angle)), static_cast<float>(sin(1.5 * M_PI - angle)));

    // Add the radius
    unitCirclePoint.x *= radius;
    unitCirclePoint.y *= radius;

    // And the center offset
    unitCirclePoint.x += center->x;
    unitCirclePoint.y += center->y;

    delete center;

    return new sf::Vector2i(static_cast<int>(unitCirclePoint.x), static_cast<int>(unitCirclePoint.y));
}

/**
 * Draws all the lines
 *
 * Runs in \f$O(n)\f$, where \f$n\f$ is the count, i.e., the number of divisions of the circle.
 * @param window The window the lines get drawn in
 * @param radius The radius of the circle
 * @param center The center coordinates of the circle, relative to the top-left point of the window
 * @param count The count, i.e., the number of divisions of the circle.
 * @param multiplier The multiplier
 */
void drawLines(sf::RenderWindow *window, float radius, sf::Vector2i *center, const int count, const double multiplier) {
    double angleIncrement = 2 * M_PI / count;

    for (int i = 0; i < count; i++) {
        auto currentPos = pointOnCircle(i * angleIncrement, radius, center);
        auto toPos = pointOnCircle(fmod(i * (multiplier), count) * angleIncrement, radius, center);

        sf::CircleShape point(4);
        point.setFillColor(sf::Color::White);
        point.setPosition(currentPos->x - 2, currentPos->y - 2);
        window->draw(point);
        line(window, currentPos, toPos);

        // Extremely important (otherwise, RAM will quickly get eaten up by the app ;-)):
        delete currentPos;
        delete toPos;
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(window_size, window_size), "SFML shapes", sf::Style::Default, settings);

    double multiplier = 2.0;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
                multiplier = 20.0 * globalPosition.y / window_size + 1.1;
                // std::cout << "Multiplier: " << multiplier << "\n";
                delete &globalPosition;
            }
        }

        sf::sleep(sf::milliseconds(5));
        window.clear(sf::Color::Black);

        float radius = window_size / 2 - 25; // NOLINT(bugprone-integer-division)

        auto *circle = new sf::CircleShape(radius, 60);
        circle->setOutlineColor(sf::Color::White);
        circle->setFillColor(sf::Color::Black);
        circle->setOutlineThickness(2.0f);
        circle->setPosition(25, 25);

        window.draw(*circle);

        delete circle;

        auto center = new sf::Vector2i(window_size / 2, window_size / 2);
        drawLines(&window, radius, center, count, multiplier);
        delete center;

        window.display();
    }

    return EXIT_SUCCESS;
}