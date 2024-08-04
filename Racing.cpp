#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <cmath>

float DegToRad(float deg) {
    return deg * (3.141592653 / 180);
}

class Player {

    public:
        sf::Texture spriteTexture;
        sf::Sprite sprite;

        float angle = 0;

        float turnSpeed = 0.1;
        float turnSpeedDirt = 0.025;

        float speed = 100;
        float speedDirt = 75;

        sf::Vector2f velocity;

        Player(std::string path, sf::Vector2f position, sf::Vector2f scale) {
            spriteTexture.loadFromFile(path);
            sprite.setTexture(spriteTexture);
            sprite.setScale(scale);
            sprite.setPosition(position);
        }
};


int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Racing Game");

    sf::Event event;

    Player car("Textures\\raceCar.png", sf::Vector2f(500, 350), sf::Vector2f(3, 3));

    car.sprite.setOrigin((sf::Vector2f)car.spriteTexture.getSize() / 3.f);

    sf::Texture roadTexture;
    sf::Sprite road;
    roadTexture.loadFromFile("Textures\\road.png");
    road.setTexture(roadTexture);
    road.setPosition(100, 100);
    road.setScale(20, 20);
    

    float dt = 0;
    sf::Clock clock;


    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        dt = clock.restart().asSeconds();

        //On Road
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && car.sprite.getGlobalBounds().intersects(road.getGlobalBounds()))
            car.angle += car.turnSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && car.sprite.getGlobalBounds().intersects(road.getGlobalBounds()))
            car.angle -= car.turnSpeed;

        //On Dirt
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && car.sprite.getGlobalBounds().intersects(road.getGlobalBounds()) == false)
            car.angle += car.turnSpeedDirt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && car.sprite.getGlobalBounds().intersects(road.getGlobalBounds()) == false)
            car.angle -= car.turnSpeedDirt;

        car.sprite.setRotation(car.angle);

        //Bound the angle between 0 and 360
        if (car.angle >= 360) car.angle -= 360;
        if (car.angle < 0) car.angle += 360;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            car.velocity.x += cos(DegToRad(car.angle)) * dt * car.speed;
            car.velocity.y += sin(DegToRad(car.angle)) * dt * car.speed;
        }

        car.sprite.move(car.velocity);

        car.velocity = sf::Vector2f(0, 0);


        window.clear();
        window.draw(road);
        window.draw(car.sprite);
        window.display();
    }

    return 0;
}
