#include <iostream>
#include <sstream>
#include <limits>

#ifdef __WIN32__
#include "mingw.thread.h"
#else
#include <thread>
#endif // __WIN32__
#include <mutex>

#include <SFML/Graphics.hpp>
#define WIDTH   640
#define HEIGHT  480

sf::Shader shader;

bool done = false;

void cinListener() {
    std::string buffer;

    while(!done) {
        char c = std::cin.peek();
        if(c == std::ios::eofbit || c == EOF) break;
        std::cin >> buffer;
        std::cout << "Read buffer : " << buffer;
    }
}

int main(int argc, char** argv) {

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fragment Shader Viewer");

    if(!sf::Shader::isAvailable()) {
        std::cerr << "It appears your graphic card can't handle shaders..." << std::endl;
        exit(-1);
    }

    std::string shaderFilePath;
    std::thread cinThread(cinListener);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed){
                switch(event.key.code) {
                    case sf::Keyboard::Escape:
                    {
                        window.close();
                    } break;
                    case sf::Keyboard::F5 :
                    {
                    } break;
                    default: break;
                }
            }
        }
    }

    done = true;
    cinThread.join();

    return 0;
}
