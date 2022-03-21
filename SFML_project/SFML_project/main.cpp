#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

const int WIDTH = 800;
const int HEIGHT = 800;

class Line {
private:
    sf::Vertex shape[2];
    Line* parent = NULL;
    
    void rotate() {
        if (parent) {
            start = parent->end;
        }
        direction = {
            direction.x * cos(speed) - direction.y * sin(speed),
            direction.x * sin(speed) + direction.y * cos(speed)
        };
        end = start + direction;
    }

    void init(sf::Vector2f _start, float _len, float _speed) {
        start = _start;
        direction = { 0., _len };
        end = start + direction;

        speed = _speed * 0.0174532;

        shape[1].color = shape[0].color = sf::Color::White;
    }
public:
    sf::Vector2f start, end, direction;
    float speed = 0;
    Line() {}
    Line(sf::Vector2f _start, float _len, float _speed) {
        init(_start, _len, _speed);
    }
    Line(Line& _parent, float _len, float _speed) {
        parent = &_parent;
        init(_parent.end, _len, _speed);
    }

    void update(sf::RenderWindow& win) {
        rotate();

        shape[0].position = start;
        shape[1].position = end;
        
        win.draw(shape, 2, sf::Lines);
    }
};

int main() {
    int lines_count = 2;
    //std::cout << "Enter number of lines: ";
    //std::cin >> lines_count;

    std::vector<Line> lines(lines_count);
    lines[0] = Line(sf::Vector2f(WIDTH / 2, HEIGHT / 2), 100, 3.14159265358979);
    lines[1] = Line(lines[0], 200, 1/3.14159265358979);
    //lines[2] = Line(lines[1], 120, 12);

    std::vector<sf::Color> colors;
    colors = { sf::Color::Transparent, sf::Color::White, sf::Color::White };
    /*for (int i = 0; i < lines_count; i++) {
        float speed, len;
        std::cout << i + 1 << '\n';
        std::cout << "length: ";
        std::cin >> len;
        std::cout << "speed: ";
        std::cin >> speed;

        if (!i) lines[i] = Line(sf::Vector2f(WIDTH / 2, HEIGHT / 2), len, speed);
        else lines[i] = Line(lines[i - 1], len, speed);
    }*/

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Spirograph", sf::Style::Default, settings);
    //window.setFramerateLimit(120);

    std::vector<sf::VertexArray> figures(lines_count, sf::VertexArray(sf::LineStrip));
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        for (int i = 0; i < lines_count; i++) {
            lines[i].update(window);
            figures[i].append(sf::Vertex(lines[i].start, colors[0]));
            figures[i].append(sf::Vertex(lines[i].end, colors[i]));
            window.draw(figures[i]);
        }

        window.display();
    }

    return 0;
}