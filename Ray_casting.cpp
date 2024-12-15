#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.hpp"
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700
#define PI 3.141592654

double distance(sf::Vector2f a) {
    return sqrt((a.x * a.x) + (a.y * a.y));
}

sf::Vector2f normalize(sf::Vector2f a) {
    double len = distance(a);
    if (len == 0) return sf::Vector2f(0, 0);
    return sf::Vector2f(a.x / len, a.y / len);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray-casting");

    sf::View main_view;
    main_view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    sf::Clock dt_clock;
    float dt = 0;
    
    Player player("Textures\\player.png", sf::Vector2f(4, 4), sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 0.5f, 500.f);

    sf::Vertex line[] = {
    sf::Vertex(player.sprite.getPosition()),
    sf::Vertex(player.sprite.getPosition() + sf::Vector2f(cos(player.sprite.getRotation() * (PI / 180)) * 50, sin(player.sprite.getRotation() * (PI / 180)) * 50))
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        dt = dt_clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            float angle = player.sprite.getRotation() - player.turn_speed;
            float angle_turned = angle < 0 ? angle += 360 : angle;

            player.sprite.setRotation(angle);    
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            float angle = player.sprite.getRotation() + player.turn_speed;
            float angle_turned = angle >= 360 ? angle -= 360 : angle;

            player.sprite.setRotation(angle);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.velocity.x += cos(player.sprite.getRotation() * (PI / 180)) * dt * player.speed;
            player.velocity.y += sin(player.sprite.getRotation() * (PI / 180)) * dt * player.speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.velocity.x -= cos(player.sprite.getRotation() * (PI / 180)) * dt * player.speed;
            player.velocity.y -= sin(player.sprite.getRotation() * (PI / 180)) * dt * player.speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            float angle_strafe = player.sprite.getRotation() - 90;

            angle_strafe = angle_strafe < 0 ? angle_strafe += 360 : angle_strafe;

            player.velocity.x += cos(angle_strafe * (PI / 180)) * dt * player.speed;
            player.velocity.y += sin(angle_strafe * (PI / 180)) * dt * player.speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            float angle_strafe = player.sprite.getRotation() + 90;

            angle_strafe = angle_strafe >= 360 ? angle_strafe -= 360 : angle_strafe;

            player.velocity.x += cos(angle_strafe * (PI / 180)) * dt * player.speed;
            player.velocity.y += sin(angle_strafe * (PI / 180)) * dt * player.speed;
        }

        player.sprite.move(player.velocity);

        player.velocity.x = 0.f;
        player.velocity.y = 0.f;

        window.clear();
        window.draw(player.sprite);
        window.display();
    }

    return 0;
}
