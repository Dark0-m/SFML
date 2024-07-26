#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    sf::RectangleShape a;
    a.setSize(sf::Vector2f(50, 50));
    a.setPosition(300, 200);
    a.setFillColor(sf::Color::Green);

    sf::RectangleShape b = a;
    b.setPosition(400, 200);
    b.setFillColor(sf::Color::Red);

    sf::Font font;
    sf::Text isCollidingText;

    font.loadFromFile("C:\\Users\\Khali\\source\\repos\\AABB collision\\AABB collision\\Fonts\\Minecraft.ttf");
    isCollidingText.setFont(font);
    isCollidingText.setCharacterSize(24);
    isCollidingText.setPosition(0, 24);

    bool isColliding = false;

    sf::Vector2f velocity;

    sf::Clock clock;
    float dt;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        auto aBounds = a.getGlobalBounds();
        auto bBounds = b.getGlobalBounds();

        dt = clock.restart().asSeconds();

        velocity = sf::Vector2f(0, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            velocity.y -= 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            velocity.y += 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            velocity.x -= 300 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            velocity.x += 300 * dt;

        sf::FloatRect nextPos = aBounds;
        nextPos.left += velocity.x;
        nextPos.top += velocity.y;

        if (nextPos.intersects(bBounds))
        {
            isColliding = true;

            float deltaX = aBounds.left + aBounds.width / 2 - (bBounds.left + bBounds.width / 2);
            float deltaY = aBounds.top + aBounds.height / 2 - (bBounds.top + bBounds.height / 2);

            //calculates the intersection to see which is the least
            float intersectX = abs(deltaX) - (aBounds.width / 2 + bBounds.width / 2);
            float intersectY = abs(deltaY) - (aBounds.height / 2 + bBounds.height / 2);

            if (intersectX > intersectY)
            {
                if (deltaX > 0)
                    velocity.x = intersectX;
                else
                    velocity.x = -intersectX;
            }
            else
            {
                if (deltaY > 0)
                    velocity.y = intersectY;
                else
                    velocity.y = -intersectY;
            }
        }
        else
            isColliding = false;

        a.move(velocity);

        if (isColliding)
            isCollidingText.setString("Is Colliding : TRUE");
        else
            isCollidingText.setString("Is Colliding : FALSE");

        window.clear();
        window.draw(b);
        window.draw(a);
        window.draw(isCollidingText);
        window.display();
    }

    return 0;
}