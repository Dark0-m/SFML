#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>

#define MAPWIDTH 15
#define MAPHEIGHT 20
#define GRAVITY 300

class Game {
    public:
        Player player;
        sf::View mainView;
        sf::Event event;

        sf::RectangleShape ground;
        sf::Sprite wall;
        sf::Texture wallTexture;

        const int mapWidth = 10;
        const int mapHeight = 10;

        float groundY = 500;
        float gravity = 300;

        int gridSize;

        std::vector<sf::Sprite> walls;

        char map[MAPHEIGHT][MAPWIDTH] = {
            {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
            {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'},
            {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
            {'#', ' ', ' ', 'P', ' ', ' ', ' ', ' ', ' ', '#'},
            {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
        };

        sf::Clock clock;
        float dt;

        void Render(sf::RenderWindow& window, sf::View mainView, std::vector<sf::Sprite> walls, Player player) {
            window.clear();
            window.setView(mainView);
            for (const auto& wall : walls) {
                window.draw(wall);
            }
            window.draw(player.sprite);
            window.display();
        }

        // Linear interpolation function
        float lerp(float start, float end, float t) {
            return start + t * (end - start);
        }
    
	    void Start(sf::RenderWindow &window) {

            mainView.setSize(window.getSize().x, window.getSize().y);
            mainView.setCenter(player.sprite.getPosition());

            ground.setFillColor(sf::Color::Green);
            ground.setSize(sf::Vector2f(1000, 20));
            ground.setPosition(0, groundY + 50);

            wallTexture.loadFromFile("Textures\\wall.png");
            wall.setTexture(wallTexture);
            wall.setScale(9, 9);

            gridSize = wall.getTexture()->getSize().x * wall.getScale().x;
            std::cout << "Grid Size : " << gridSize << "\n";

            for (int y = 0; y < mapHeight; y++) {
                for (int x = 0; x < mapWidth; x++) {
                    switch (map[y][x])
                    {
                    case '#':
                        wall.setPosition(x * gridSize, y * gridSize);
                        walls.push_back(sf::Sprite(wall));
                        break;

                    case 'P':
                        player.sprite.setPosition(x * gridSize, y * gridSize);
                        break;

                    default:
                        break;
                    }
                }
            }
	    }

        void Update(sf::RenderWindow &window) {

            while (window.isOpen()) {

                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        window.close();
                }

                dt = clock.restart().asSeconds();

                if (player.jumpKeyTime < player.jumpTimeLimit)
                    player.jumpKeyTime += 40 * dt;



                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !player.isJumping && player.jumpKeyTime >= player.jumpTimeLimit) {
                    player.t = 0.0f;
                    player.tempY = player.sprite.getPosition().y;
                    player.isJumping = true;
                    player.jumpKeyTime = 0.0f;
                }

                if (player.isJumping) {
                    player.speed = 100;
                    player.t += dt / player.duration;

                    player.currentY = lerp(player.tempY, player.tempY - player.jumpHeight, player.t) - player.jumpHeight * 1.25 * player.t * (1 - player.t);
                    player.sprite.setPosition(player.sprite.getPosition().x, player.currentY);
                }

                player.sprite.move(0, gravity * dt);

                Collision(player, walls, player.velocity);
                

                player.movement(player.velocity, player.speed, dt);


                mainView.setCenter(player.sprite.getPosition());

                player.sprite.move(player.velocity);

                player.velocity.x = 0.0f;
                player.velocity.y = 0.0f;

                player.speed = 300;

                if (player.t > 1.0f)
                    player.isJumping = false;

                Render(window, mainView, walls, player);
            }
        }

        void Collision(Player& player, std::vector<sf::Sprite>& walls, sf::Vector2f& velocity) {
            for (int i = 0; i < walls.size(); i++)
            {
                sf::FloatRect nextPos;
                sf::FloatRect playerBounds = player.sprite.getGlobalBounds();
                sf::FloatRect wallBounds = walls[i].getGlobalBounds();

                nextPos = playerBounds;

                nextPos.left += velocity.x;
                nextPos.top += velocity.y;

                if (wallBounds.intersects(nextPos)) {


                    //Bottom Collision
                    if (playerBounds.top < wallBounds.top
                        && playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
                        && playerBounds.left < wallBounds.left + wallBounds.width
                        && playerBounds.left + playerBounds.width > wallBounds.left)
                    {
                        velocity.y = 0.0f;
                        player.sprite.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                    }

                    //Top Collision
                    if (playerBounds.top > wallBounds.top
                        && playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
                        && playerBounds.left < wallBounds.left + wallBounds.width
                        && playerBounds.left + playerBounds.width > wallBounds.left)
                    {
                        velocity.y = 0.0f;
                        player.sprite.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                    }

                    //Left Collision
                    if (playerBounds.left < wallBounds.left
                        && playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top)
                    {
                        velocity.x = 0.0f;
                        player.sprite.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                    }

                    //Right Collision
                    if (playerBounds.left > wallBounds.left
                        && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top)
                    {
                        velocity.x = 0.0f;
                        player.sprite.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                    }

                }

                sf::FloatRect nextPosBottom;
                nextPosBottom.top = nextPos.top + nextPos.height;
            }
        }

        Game(sf::RenderWindow &window) {
            Start(window);
            Update(window);
        }

};