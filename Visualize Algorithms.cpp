#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

void bubbleSort(int& i, int& j, std::vector<sf::RectangleShape>& rectangles, bool& sorting, bool& sorted) {
    if (i >= rectangles.size() - 1) {
        sorted = true;
        return;
    }

    if (std::abs(rectangles[j].getSize().y) > std::abs(rectangles[j + 1].getSize().y)) {
        std::swap(rectangles[j], rectangles[j + 1]);

        sf::Vector2f tempPos = rectangles[j].getPosition();
        rectangles[j].setPosition(rectangles[j + 1].getPosition());
        rectangles[j + 1].setPosition(tempPos);
    }

    j++;

    if (j >= rectangles.size() - i - 1) {
        j = 0;
        i++;
    }

    sf::sleep(sf::milliseconds(50));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Visualize Algorithms");

    // Vector containing the heights for the rectangles
    std::vector<int> nums;
    bool sorting = false;
    int i = 0, j = 0;

    srand(time(0));

    int limit = rand() % 15;

    while (limit < 5) {
        limit = rand() % 15;
    }

    for (int i = 0; i < limit; i++) {
        nums.push_back(rand() % 100);
    }

    int screenWidth = window.getSize().x;
    int screenHeight = window.getSize().y - 10;

    int width = screenWidth / nums.size();

    int maxNum = *std::max_element(nums.begin(), nums.end());

    float heightScale = static_cast<float>(screenHeight) / maxNum;

    sf::RectangleShape rectRef;
    rectRef.setFillColor(sf::Color::White);

    std::vector<sf::RectangleShape> rectangles;

    for (int i = 0; i < nums.size(); i++) {
        float rectHeight = heightScale * nums[i];
        rectRef.setSize(sf::Vector2f(width - 5, -rectHeight));
        rectRef.setPosition(i * width, screenHeight);
        rectangles.push_back(sf::RectangleShape(rectRef));
    }


    sf::Clock clock;
    float delay = 0.5f;
    bool sorted = false;

    std::cout << nums.size();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !sorted) {
            sorting = true;
        }

        if (sorting) {
            bubbleSort(i, j, rectangles, sorting, sorted);
            clock.restart();
        }

        if (sorted) {
            for (auto& rect : rectangles) {
                rect.setFillColor(sf::Color::Green);
            }
        }

        // Render
        window.clear();
        for (auto& rect : rectangles) {
            window.draw(rect);
        }
        window.display();
    }

    return 0;
}
