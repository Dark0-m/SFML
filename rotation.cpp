#include <iostream>
#include "SFML/Graphics.hpp"


int main() {

	sf::RenderWindow window(sf::VideoMode(1000, 700), "Test Angle");

	sf::Event event;

	sf::Texture triangleTexture;
	sf::Sprite triangle;

	triangleTexture.loadFromFile("Textures\\triangle.png");
	triangle.setTexture(triangleTexture);
	triangle.setPosition(500, 350);
	triangle.setScale(2, 2);
	triangle.setOrigin((sf::Vector2f)triangleTexture.getSize() / 2.f);

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

		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

		sf::Vector2f dir = sf::Vector2f(mousePosition) - triangle.getPosition();
		float magnitudeDir = abs(sqrt(pow(mousePosition.x - triangle.getPosition().x, 2) + pow(mousePosition.y - triangle.getPosition().y, 2))) * 100000 * dt;
		sf::Vector2f dirNorm = dir / magnitudeDir;

		float angle = atan2(dirNorm.y, dirNorm.x) * (180 / 3.141592653);
		triangle.setRotation(angle);

		triangle.move(dirNorm);

		window.clear();
		window.draw(triangle);
		window.display();
	}

	return 0;
}