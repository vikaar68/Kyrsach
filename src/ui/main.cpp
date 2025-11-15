#include <SFML/Graphics.hpp>
#include "ScrumBoard.h"

int main() {
    setlocale(LC_ALL, "Russian");
    const unsigned int WINDOW_WIDTH = 1920;
    const unsigned int WINDOW_HEIGHT = 1080;
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Scrum Board");
    window.setFramerateLimit(60);

    ScrumBoard scrumBoard;
    if (!scrumBoard.initialize()) {
        return -1;
    }

    sf::Clock clock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            scrumBoard.handleEvent(event, window);
        }

        float deltaTime = clock.restart().asSeconds();
        scrumBoard.update(deltaTime);

        window.clear(sf::Color(240, 245, 255));
        scrumBoard.draw(window);
        window.display();
    }
    return 0;
}