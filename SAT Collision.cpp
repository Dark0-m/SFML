#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

sf::Vector2f perpendicular(const sf::Vector2f& v) {
    return sf::Vector2f(-v.y, v.x);
}

void projectShapeOntoAxis(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& axis, float& min, float& max) {
    float projection = dotProduct(vertices[0], axis);
    min = projection;
    max = projection;

    for (size_t i = 1; i < vertices.size(); i++) {
        projection = dotProduct(vertices[i], axis);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
}

bool projectionsOverlap(float minA, float maxA, float minB, float maxB) {
    return !(minA > maxB || minB > maxA);
}

bool SATCollision(const std::vector<sf::Vector2f>& shape1, const std::vector<sf::Vector2f>& shape2) {
    std::vector<sf::Vector2f> axes;

    for (size_t i = 0; i < shape1.size(); i++) {
        sf::Vector2f p1 = shape1[i];
        sf::Vector2f p2 = shape1[i + 1 < shape1.size() ? i + 1 : 0];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = perpendicular(edge);
        axes.push_back(axis);
    }

    for (size_t i = 0; i < shape2.size(); i++) {
        sf::Vector2f p1 = shape2[i];
        sf::Vector2f p2 = shape2[i + 1 < shape2.size() ? i + 1 : 0];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = perpendicular(edge);
        axes.push_back(axis);
    }

    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;

        projectShapeOntoAxis(shape1, axis, minA, maxA);
        projectShapeOntoAxis(shape2, axis, minB, maxB);

        if (!projectionsOverlap(minA, maxA, minB, maxB)) {
            return false;
        }
    }

    return true;
}

// Helper function to get the transformed vertices of the shape
std::vector<sf::Vector2f> getTransformedVertices(const sf::ConvexShape& shape) {
    std::vector<sf::Vector2f> transformedVertices;
    for (size_t i = 0; i < shape.getPointCount(); i++) {
        transformedVertices.push_back(shape.getTransform().transformPoint(shape.getPoint(i)));
    }
    return transformedVertices;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SAT Collision Detection with Movement");

    sf::ConvexShape shape1;
    shape1.setPointCount(4);
    shape1.setPoint(0, sf::Vector2f(400, 300));
    shape1.setPoint(1, sf::Vector2f(450, 300));
    shape1.setPoint(2, sf::Vector2f(450, 250));
    shape1.setPoint(3, sf::Vector2f(400, 250));


    sf::ConvexShape shape2;
    shape2.setPointCount(4);
    shape2.setPoint(0, sf::Vector2f(200, 200));
    shape2.setPoint(1, sf::Vector2f(250, 200));
    shape2.setPoint(2, sf::Vector2f(250, 150));
    shape2.setPoint(3, sf::Vector2f(200, 150));

    sf::Vector2f velocity(0, 0);
    float dt = 0;
    sf::Clock clock;

    float turnSpeed = 50;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        dt = clock.restart().asSeconds();

        velocity = sf::Vector2f(0, 0);

        sf::FloatRect bounds1 = shape1.getGlobalBounds();
        shape1.setOrigin(bounds1.width / 2, bounds1.height / 2);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            shape1.rotate(turnSpeed * dt);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            shape1.rotate(-turnSpeed * dt);
        }

        // Move shape1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            velocity.y += -0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            velocity.y += 0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocity.x += -0.1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocity.x += 0.1;
        }

        shape1.move(velocity);

        std::vector<sf::Vector2f> axesShape1 = getTransformedVertices(shape1);
        std::vector<sf::Vector2f> axesShape2 = getTransformedVertices(shape2);

        bool collision = SATCollision(axesShape1, axesShape2);

        window.clear();

        shape1.setFillColor(collision ? sf::Color::Red : sf::Color::White);

        window.draw(shape1);
        window.draw(shape2);

        window.display();
    }

    return 0;
}
