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
float downlimit = 0.5f;
float uplimit = 3.0f;
float inc = 0.05f;

float spreadAngle = 70;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fragment Shader Viewer");

void loadShader(sf::Shader& shader, std::string filename) {

    if(!shader.loadFromFile(filename.c_str(),sf::Shader::Fragment)) {
        std::cerr << "Failed to load " << filename << std::endl;
    }

    sf::Vector2i mouseInt = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(window.mapPixelToCoords(mouseInt));
    shader.setParameter("center",mouse);
    shader.setParameter("radius",300.0f);
    shader.setParameter("color",sf::Color(0,255,0,255));
    shader.setParameter("bleed",0.0f);
    shader.setParameter("linearFactor",1.0f);
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
    if(!tex.loadFromFile("data/brickwall.jpg")) {
        std::cerr << "Failed to load brickwall.jpg" << std::endl;
        return -2;
    }

    sf::Texture normalTex;
    if(!normalTex.loadFromFile("data/brickwall_normal.jpg")) {
        std::cerr << "Failed to load brickwall_normal.jpg" << std::endl;
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
    sf::Sprite tex_spr(shaderTexture.getTexture());

    //shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
    sf::Vector2i mouseInt = sf::Mouse::getPosition(window);
    sf::Vector2f mouse(window.mapPixelToCoords(mouseInt));

    sf::Clock clock;

    bool outline = false;
    shader.setParameter("outline",outline);

    sf::ConvexShape triangle;
    triangle.setPointCount(4);
    triangle.setPoint(0,sf::Vector2f(WIDTH/2,HEIGHT/2));
    triangle.setPoint(2,DMUtils::sfml::rotate(triangle.getPoint(0)+sf::Vector2f(0,350.0),DMUtils::maths::degToRad(0.0),triangle.getPoint(0)));
    //triangle.setPoint(2,triangle.getPoint(0)+sf::Vector2f(0,200));
    triangle.setPoint(1,DMUtils::sfml::rotate(triangle.getPoint(2),DMUtils::maths::degToRad(-spreadAngle/2.0f),triangle.getPoint(0)));
    triangle.setPoint(3,DMUtils::sfml::rotate(triangle.getPoint(2),DMUtils::maths::degToRad(+spreadAngle/2.0f),triangle.getPoint(0)));

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
                    case sf::Keyboard::F2:
                        outline = !outline;
                        shader.setParameter("outline",outline);
                        std::cout << "outline : " << outline << std::endl;
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
        shaderTexture.clear();

        //shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
        shader.setParameter("center",sf::Vector2f(WIDTH/2,HEIGHT/2));
        shaderTexture.draw(rect,&shader);
        //shaderTexture.draw(triangle,&shader);
        window.clear(sf::Color::Blue);
        window.draw(spr);
        window.draw(tex_spr,sf::BlendMultiply);
        //window.draw(triangle,&shader);
        window.display();

    }

    return 0;
}
