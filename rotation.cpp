#include <iostream>
#include "SFML/Graphics.hpp"

float DegToRad(float deg) {
    return deg * (3.141592653 / 180);
}

float RadToDeg(float rad) {
    return rad * (180 / 3.141592653);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Test Angle");

    sf::Event event;

    sf::Texture playerTexture;
    sf::Sprite player;

    playerTexture.loadFromFile("Textures\\circle.png");
    player.setTexture(playerTexture);
    player.setPosition(500, 350);
    player.setScale(2, 2);
    player.setOrigin((sf::Vector2f)playerTexture.getSize() / 2.f);

    sf::Vector2f velocity = { 0.0f, 0.0f };
    float speed = 300;
    float playerAngle = 0.0f;
    float turnSpeed = 2.f;

    sf::Texture wallTexture;
    sf::Sprite wall;

    wallTexture.loadFromFile("Textures\\wall.png");
    wall.setTexture(wallTexture);
    wall.setScale(6, 6);

    std::vector<sf::Sprite> walls;

    char map[8][10] = {
        "#########",
        "#.......#",
        "#..#.#..#",
        "#.#...#.#",
        "#.#...#.#",
        "#..#.#..#",
        "#.......#",
        "#########"
    };

    sf::View mainView;
    mainView.setSize(1000, 700);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 10; x++) {
            switch (map[y][x]) {
            case '#':
                wall.setPosition(x * 16 * wall.getScale().x, y * 16 * wall.getScale().y);
                walls.push_back(sf::Sprite(wall));
                break;
            }
        }
    }

    float dt;
    sf::Clock clock;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        dt = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            playerAngle -= turnSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            playerAngle += turnSpeed;

        player.setRotation(playerAngle);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            velocity.x += cos(DegToRad(playerAngle)) * speed * dt;
            velocity.y += sin(DegToRad(playerAngle)) * speed * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            velocity.x -= cos(DegToRad(playerAngle)) * speed * dt;
            velocity.y -= sin(DegToRad(playerAngle)) * speed * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocity.x += cos(DegToRad(playerAngle - 90)) * speed * dt;
            velocity.y += sin(DegToRad(playerAngle - 90)) * speed * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocity.x += cos(DegToRad(playerAngle + 90)) * speed * dt;
            velocity.y += sin(DegToRad(playerAngle + 90)) * speed * dt;
        }

        if (playerAngle >= 360) playerAngle -= 360;
        if (playerAngle < 0) playerAngle += 360;

        player.move(velocity.x, velocity.y);

        mainView.setCenter(player.getPosition());

        velocity.x = 0.0f;
        velocity.y = 0.0f;

        window.setView(mainView);

        window.clear();
        window.draw(player);

        for (auto& wall : walls)
            window.draw(wall);

        window.display();
    }

    return 0;
}
