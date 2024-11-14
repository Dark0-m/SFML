#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700

double distance(sf::Vector2f a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

sf::Vector2f normalize(sf::Vector2f a) {
    return sf::Vector2f(a.x / distance(a), a.y / distance(a));
}

class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed = 500;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "360 Shooter");

    sf::CircleShape player;
    player.setRadius(25.f);
    player.setFillColor(sf::Color::White);
    player.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    player.setOrigin(player.getRadius(), player.getRadius());

    Bullet bullet;
    std::vector<Bullet> bullets;

    sf::Event event;
    sf::Clock dtClock;
    float dt;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
                window.close();
            }
        }

        dt = dtClock.restart().asSeconds();

        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::Vector2f direction = mousePosition - player.getPosition();
        sf::Vector2f directionNorm = normalize(direction);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.move(0, -500 * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.move(0, 500 * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.move(-500 * dt, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.move(500 * dt, 0);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            bullet.shape.setRadius(10.f);
            bullet.shape.setOrigin(10.f, 10.f);
            bullet.shape.setPosition(player.getPosition());
            bullet.shape.setFillColor(sf::Color::Green);
            bullet.velocity = directionNorm * bullet.speed * dt;
            bullets.push_back(Bullet(bullet));
        }


        for (auto& bullet : bullets) {
            if (bullet.shape.getPosition().x <= 0 || bullet.shape.getPosition().x >= WINDOW_WIDTH) {
                bullet.velocity.x = 0;
            }
            if (bullet.shape.getPosition().y >= WINDOW_HEIGHT || bullet.shape.getPosition().y <= 0) {
                bullet.velocity.y = 0;
            }
            bullet.shape.move(bullet.velocity);
        }

        window.clear();
        window.draw(player);
        
        for (auto& bullet : bullets) {
            window.draw(bullet.shape);
        }

        window.display();
    }

    return 0;
}
