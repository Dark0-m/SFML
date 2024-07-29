#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <cmath>

float Magnitude(sf::Vector2f a) {
    return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

sf::Vector2f Normalize(sf::Vector2f a) {
    float magnitudeA = Magnitude(a);
    return a / magnitudeA;
}

float dotProduct(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Dot Product");

    sf::Event event;

    sf::RectangleShape finishLine;
    finishLine.setPosition(500, 350);
    finishLine.setSize(sf::Vector2f(10, 100));
    finishLine.setFillColor(sf::Color::Red);

    sf::RectangleShape car;
    car.setPosition(200, 350);
    car.setSize(sf::Vector2f(20, 20));
    car.setOrigin(10, 10);
    car.setFillColor(sf::Color::Green);

    float dt = 0;
    sf::Clock clock;

    sf::Font font;
    if (!font.loadFromFile("Fonts/Minecraft.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    sf::Text dp;
    dp.setCharacterSize(25.f);
    dp.setPosition(0.0f, 10.f);
    dp.setFont(font);
    dp.setFillColor(sf::Color::White);

    sf::Vector2f targetDirection(1.0f, 0.0f); // Assume the correct entry direction is to the right

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        dt = clock.restart().asSeconds();

        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::Vector2f carDirection = mousePos - car.getPosition();
        float magnitudeDirection = Magnitude(carDirection);

        carDirection /= magnitudeDirection;
        car.move(carDirection.x * 300 * dt, carDirection.y * 300 * dt);

        sf::Vector2f carNorm = Normalize(carDirection);

        float dotProd = dotProduct(carNorm, targetDirection);
        dp.setString("Dot Product: " + std::to_string(dotProd));

        // Check if car is entering the finish line correctly
        if (dotProd > 0.9f) { // Adjust threshold as needed
            dp.setFillColor(sf::Color::Green);
        } else {
            dp.setFillColor(sf::Color::Red);
        }

        window.clear();
        window.draw(dp);
        window.draw(finishLine);
        window.draw(car);
        window.display();
    }

    return 0;
}
