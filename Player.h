#pragma once
#include <SFML/Graphics.hpp>


class Player {
	public:
		sf::Sprite sprite;
		sf::Texture texture;

		sf::Vector2f velocity;
		float speed = 300;

		float currentY = 0.0f;
		float startY = 0.0f;
		float endY = 600.0f;
		float duration = 0.5f;
		float t = 0.0f;

		float tempY = 0.0f;

		bool isJumping = false;
		float jumpKeyTime = 40;
		int jumpTimeLimit = 60;
		float jumpHeight = 300;

		sf::Vector2f movement(sf::Vector2f& velocity, float speed, float dt) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				velocity.x -= 300 * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				velocity.x += 300 * dt;

			return velocity;
		}

		Player() {
			texture.loadFromFile("Textures\\player.png");
			sprite.setTexture(texture);
			sprite.setScale(3, 3);
		}


};