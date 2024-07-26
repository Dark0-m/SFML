#include <iostream>
#include "SFML/Graphics.hpp"

float lerp(float start, float end, float t) {
	return start * (1 - t) + end * t;
}


int main() {

	sf::RenderWindow window(sf::VideoMode(1000, 700), "LERP");

	sf::Event event;

	sf::CircleShape a(50.f);
	a.setFillColor(sf::Color::Green);
	a.setPosition(0, window.getSize().y / 2);

	sf::CircleShape b(50.f);
	b.setFillColor(sf::Color::Red);
	b.setPosition(0, 500);

	float dt;	
	sf::Clock clock;


	while (window.isOpen())
	{
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		float bX = b.getPosition().x;
		float aX = a.getPosition().x;

		float bY = b.getPosition().y;
		float aY = a.getPosition().y;

		dt = clock.restart().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			a.move(0.0f, -600.f * dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			a.move(-600.f * dt, 0.0f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			a.move(0.0f, 600.f * dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			a.move(600.f * dt, 0.0f);

		std::cout << "Position : " << abs(sqrt(pow(bX - aX, 2) + pow(bY - aY, 2))) << "\n";

		if (abs(sqrt(pow(bX - aX, 2) + pow(bY - aY, 2)) <= 100)) {
			sf::Vector2f dir = a.getPosition() - b.getPosition();
			float magnitudeDir = abs(sqrt(pow(bX - aX, 2) + pow(bY - aY, 2))) * 100 * dt;
			sf::Vector2f dirNorm = -dir / magnitudeDir;
			b.move(dirNorm);
		}


		window.clear();
		window.draw(a);
		window.draw(b);
		window.display();
	}


	return 0;
}