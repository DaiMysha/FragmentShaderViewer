#include <iostream>
#include <sstream>
#include <limits>

#include <SFML/Graphics.hpp>

#include <DMUtils/maths.hpp>
#include <DMUtils/sfml.hpp>

#define WIDTH   640
#define HEIGHT  480


sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fragment Shader Viewer");

void loadShader(sf::Shader& shader, std::string filename) {

    if(!shader.loadFromFile(filename.c_str(),sf::Shader::Fragment)) {
        std::cerr << "Failed to load " << filename << std::endl;
    }
    else
    {
        std::cerr << "Loaded shader successfully" << std::endl;
    }
}

int main(int argc, char** argv) {


    if(!sf::Shader::isAvailable()) {
        std::cerr << "It appears your graphic card can't handle shaders..." << std::endl;
        exit(-1);
    }

    sf::Shader shader;

    std::string filename("shaders/heightMap.frag");

   loadShader(shader,filename);

    sf::Texture bg;
    if(!bg.loadFromFile("data/nolight.png")) {
        std::cerr << "Failed to load nolight.png" << std::endl;
        return -2;
    }
    sf::Sprite bgSpr(bg);

    sf::Texture lightMapTex;
    if(!lightMapTex.loadFromFile("data/lightmap.png"))
    {
        std::cerr << "Failed to load lightmap.png" << std::endl;
        return -2;
    }
    sf::Sprite lightMap(lightMapTex);

    sf::Texture heightMap;
    if(!heightMap.loadFromFile("data/heightmap.png"))
    {
        std::cerr << "Failed to load heightmap.png" << std::endl;
        return -2;
    }

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
    sf::Sprite tex_spr(shaderTexture.getTexture());

    //shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
    sf::Vector2i mouseInt = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(window.mapPixelToCoords(mouseInt));

    sf::Clock clock;

    sf::RenderStates multiplySte;
    multiplySte.blendMode = sf::BlendMultiply;

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
                        loadShader(shader,filename);
                    } break;
                    default: break;
                }
            }
            mouseInt = sf::Mouse::getPosition(window);
            mouse = window.mapPixelToCoords(mouseInt);
        }
        shaderTexture.clear();
        shader.setParameter("heightMap", heightMap);
        shader.setParameter("lightMap", lightMapTex);
        shader.setParameter("width", heightMap.getSize().x);
        shader.setParameter("height", heightMap.getSize().y);
        shaderTexture.draw(rect, &shader);

        //shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
        shaderTexture.draw(rect,&shader);
        //shaderTexture.draw(triangle,&shader);
        window.clear(sf::Color::Blue);
        window.draw(bgSpr);
        //window.draw(lightMap, multiplySte);
        window.draw(tex_spr, multiplySte);
        window.display();

    }

    return 0;
}
