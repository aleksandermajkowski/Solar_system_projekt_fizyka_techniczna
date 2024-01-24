#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;

const sf::Color WHITE(255, 255, 255);
const sf::Color YELLOW(255, 255, 0);
const sf::Color BLUE(0, 0, 255);
const sf::Color RED(255, 0, 0);
const sf::Color GREEN(0, 255, 0);

class Planet : public sf::Drawable, public sf::Transformable {
public:
    Planet(const std::string& name, sf::Color color, float radius, float distance, float speed) :
        m_name(name),
        m_color(color),
        m_radius(radius),
        m_distance(distance),
        m_speed(speed),
        m_angle(0)
    {
        m_circle.setRadius(radius);
        m_circle.setFillColor(color);
        m_circle.setOrigin(radius, radius);

        m_orbit.setRadius(distance);
        m_orbit.setFillColor(sf::Color(0, 0, 0, 0));  
        m_orbit.setOutlineColor(sf::Color(100, 100, 100));
        m_orbit.setOutlineThickness(1);
        m_orbit.setOrigin(distance, distance);
    }

    void update() {
        m_angle += m_speed;
        float x = WIDTH / 2 + m_distance * std::cos(3.14 * m_angle / 180);
        float y = HEIGHT / 2 + m_distance * std::sin(3.14 * m_angle / 180);
        setPosition(x, y);
    }

private:
    std::string m_name;
    sf::Color m_color;
    float m_radius;
    float m_distance;
    float m_speed;
    float m_angle;
    sf::CircleShape m_circle;
    sf::CircleShape m_orbit;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(m_orbit, states);

        target.draw(m_circle, states);

        sf::Text text;
        text.setString(m_name);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        text.setPosition(getPosition().x - m_radius, getPosition().y - m_radius - 15);
        target.draw(text, states);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Solar System Simulation");
    window.setFramerateLimit(FPS);

    sf::CircleShape sun(25);
    sun.setFillColor(YELLOW);
    sun.setOrigin(25, 25);
    sun.setPosition(WIDTH / 2, HEIGHT / 2);

    std::vector<Planet> planets;
    std::vector<std::tuple<std::string, sf::Color, float, float, float>> planetData = {
        std::make_tuple("Mercury", BLUE, 5, 50, 3),
        std::make_tuple("Venus", WHITE, 8, 80, 2),
        std::make_tuple("Earth", GREEN, 10, 120, 1.5),
        std::make_tuple("Mars", RED, 7, 180, 1),
        std::make_tuple("Jupiter", sf::Color::Yellow, 20, 250, 0.5),
    };

    for (const auto& data : planetData) {
        std::string name;
        sf::Color color;
        float radius, distance, speed;
        std::tie(name, color, radius, distance, speed) = data;
        planets.emplace_back(name, color, radius, distance, speed);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        for (auto& planet : planets) {
            planet.update();
        }

        window.clear(sf::Color::Black);

        window.draw(sun);

        // Draw planet orbits
        for (const auto& planet : planets) {
            window.draw(planet);
        }

        window.display();
    }

    return 0;
}
