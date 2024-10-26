#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Simple dot product function for projections
float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b) {
    return a.x * b.x + a.y * b.y;
}

// Function to calculate the perpendicular vector (normal) to an edge
sf::Vector2f perpendicular(const sf::Vector2f& v) {
    return sf::Vector2f(-v.y, v.x); // Rotate the vector by 90 degrees
}

// Function to project a shape's vertices onto an axis and get min/max projection
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

// Function to check if two projections overlap
bool projectionsOverlap(float minA, float maxA, float minB, float maxB) {
    return !(minA > maxB || minB > maxA);
}

// Main function to check SAT collision between two convex shapes
bool SATCollision(const std::vector<sf::Vector2f>& shape1, const std::vector<sf::Vector2f>& shape2) {
    std::vector<sf::Vector2f> axes;

    // Generate axes for shape1 (normals of its edges)
    for (size_t i = 0; i < shape1.size(); i++) {
        sf::Vector2f p1 = shape1[i];
        sf::Vector2f p2 = shape1[(i + 1) % shape1.size()];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = perpendicular(edge);
        axes.push_back(axis);
    }

    // Generate axes for shape2 (normals of its edges)
    for (size_t i = 0; i < shape2.size(); i++) {
        sf::Vector2f p1 = shape2[i];
        sf::Vector2f p2 = shape2[(i + 1) % shape2.size()];
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f axis = perpendicular(edge);
        axes.push_back(axis);
    }

    // Check for each axis
    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;

        // Project both shapes onto the axis
        projectShapeOntoAxis(shape1, axis, minA, maxA);
        projectShapeOntoAxis(shape2, axis, minB, maxB);

        // If the projections don't overlap, there's no collision
        if (!projectionsOverlap(minA, maxA, minB, maxB)) {
            return false; // No collision
        }
    }

    return true; // All projections overlap, so there is a collision
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SAT Collision Detection with Movement");

    // Define two convex shapes
    std::vector<sf::Vector2f> shape1 = { sf::Vector2f(100, 100), sf::Vector2f(150, 100), sf::Vector2f(125, 150) };
    std::vector<sf::Vector2f> shape2 = { sf::Vector2f(300, 300), sf::Vector2f(350, 300), sf::Vector2f(325, 350) };

    sf::Vector2f velocity(0, 0); // Velocity of the second shape

    // Run the main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Handle keyboard input for movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) velocity.y = -0.1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) velocity.y = 0.1;
        else velocity.y = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) velocity.x = -0.1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) velocity.x = 0.1;
        else velocity.x = 0;

        // Move shape 2 with velocity
        for (auto& vertex : shape2) {
            vertex += velocity;
        }

        // Detect collision using SAT
        bool collision = SATCollision(shape1, shape2);

        // Clear the window
        window.clear();

        // Draw shapes (you can use sf::ConvexShape to draw)
        sf::ConvexShape poly1;
        poly1.setPointCount(3);
        for (int i = 0; i < 3; i++) poly1.setPoint(i, shape1[i]);
        poly1.setFillColor(sf::Color::Green);

        sf::ConvexShape poly2;
        poly2.setPointCount(3);
        for (int i = 0; i < 3; i++) poly2.setPoint(i, shape2[i]);
        poly2.setFillColor(collision ? sf::Color::Red : sf::Color::Blue);

        // Draw shapes
        window.draw(poly1);
        window.draw(poly2);

        // Display
        window.display();
    }

    return 0;
}
