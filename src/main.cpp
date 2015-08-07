#include <iostream>
#include <sstream>
#include <limits>

#include <SFML/Graphics.hpp>

#include <DMUtils/maths.hpp>
#include <DMUtils/sfml.hpp>

#define WIDTH   640
#define HEIGHT  480

const char varyingParamName[] = "";
int delay = 100;
float downlimit = 0.0f;
float uplimit = 1.0f;
float inc = 0.05f;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fragment Shader Viewer");

void loadShader(sf::Shader& shader, std::string filename) {

    if(!shader.loadFromFile(filename.c_str(),sf::Shader::Fragment)) {
        std::cerr << "Failed to load " << filename << std::endl;
        getchar();
        exit( -1);
    }

    sf::Vector2i mouseInt = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(window.mapPixelToCoords(mouseInt));
    shader.setParameter("center",mouse);
    shader.setParameter("radius",300.0f);
    shader.setParameter("color",sf::Color(255,0,0,255));
    shader.setParameter("directionAngle",0.0f);
    shader.setParameter("spreadAngle",360.0f);
    shader.setParameter("intensity",1.0f);
    shader.setParameter("size",1.0f);
    shader.setParameter("bleed",0.0f);
    shader.setParameter("linearFactor",0.5f);
}

int main(int argc, char** argv) {


    if(!sf::Shader::isAvailable()) {
        std::cerr << "It appears your graphic card can't handle shaders..." << std::endl;
        exit(-1);
    }

    sf::Shader shader;

    std::string filename("shaders/spotLightRender.frag");

   loadShader(shader,filename);

    sf::Texture tex;
    if(!tex.loadFromFile("data/map.jpg")) {
        std::cerr << "Failed to load map.jpg" << std::endl;
        return -2;
    }

    sf::Sprite spr(tex);

    sf::VertexArray rect(sf::Quads,4);
    rect[0].position = sf::Vector2f(0,0);
    rect[1].position = sf::Vector2f(0,HEIGHT);
    rect[2].position = sf::Vector2f(WIDTH,HEIGHT);
    rect[3].position = sf::Vector2f(WIDTH,0);
    rect[0].texCoords = sf::Vector2f(0.0,0.0);
    rect[1].texCoords = sf::Vector2f(0.0,1.0);
    rect[2].texCoords = sf::Vector2f(1.0,1.0);
    rect[3].texCoords = sf::Vector2f(1.0,0.0);

    sf::RenderTexture shaderTexture;
    shaderTexture.create(WIDTH,HEIGHT);
    shaderTexture.clear();

    //shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
    sf::Vector2i mouseInt = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(window.mapPixelToCoords(mouseInt));

    float varying = 250.0f;

    sf::Clock clock;

    bool useBeta = false;
    shader.setParameter("useBeta",useBeta);

    window.setMouseCursorVisible(false);

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
                    case sf::Keyboard::F1:
                        useBeta = !useBeta;
                        shader.setParameter("useBeta",useBeta);
                        std::cout << "beta : " << useBeta << std::endl;
                        break;
                    case sf::Keyboard::F5 :
                    {
                        loadShader(shader,filename);
                    } break;
                    default: break;
                }
            }
            mouseInt = sf::Mouse::getPosition(window);
            mouse = window.mapPixelToCoords(mouseInt);
        }

        shader.setParameter("center",mouse);
        shaderTexture.draw(rect,&shader);
        window.clear(sf::Color::Blue);
        window.draw(spr);
        window.draw(sf::Sprite(shaderTexture.getTexture()));
        window.display();

        if(clock.getElapsedTime().asMilliseconds() > delay) {
            varying += inc;
            shader.setParameter(varyingParamName,varying);
            if(varying > uplimit) varying = downlimit;
            std::cout << "[" << downlimit << "] " << varying << " [" << uplimit << "]"
            << " | bleed / dist² : " <<  (varying/DMUtils::maths::power<2>::of(DMUtils::sfml::norm2(sf::Vector2f(15,15)-mouse)))
            << " | linearFactor / radius :" << (0.5f/300.0f)
            << std::endl;
            clock.restart();
        }
    }

    return 0;
}
