#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <climits>
#include <float.h>
#include <cstdio>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <chrono>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "renderer.h"

typedef struct sfmlDisplay {
    sf::RenderWindow* window;
    sf::Texture texturePendulum;
    sf::Texture textureArrow;
    sf::Font font;
} sfmlDisplay;

static sfmlDisplay display;
static std::chrono::high_resolution_clock::time_point previousFrameTime;

void Renderer::renderInit() {
    display.window = new sf::RenderWindow(sf::VideoMode(DISPLAY_W, DISPLAY_H), "Environment_Display");

    if (!display.font.loadFromFile(PATH_TTF)) {
        std::cerr << "Failed to load font" << std::endl;
        exit(1);
    }

    if (!display.texturePendulum.loadFromFile(PATH_PENDULUM)) {
        std::cerr << "Failed to load pendulum texture" << std::endl;
        exit(1);
    }

    if (!display.textureArrow.loadFromFile(PATH_ARROW)) {
        std::cerr << "Failed to load arrow texture" << std::endl;
        exit(1);
    }

    previousFrameTime = std::chrono::high_resolution_clock::now();
}

void Renderer::displayText(const char* text, int posX, int posY) {
    sf::Text sfText;
    sfText.setFont(display.font);
    sfText.setString(text);
    sfText.setCharacterSize(20);
    sfText.setFillColor(sf::Color::Green);
    sfText.setPosition(posX, posY);
    display.window->draw(sfText);
}

int Renderer::renderEnv(double state, double torque, uint64_t frame, uint64_t generation, double timeDelta) {
    static long int i = 0;
    static double max_fps = 0.;
    static double avg_fps = 0.;
    static double min_fps = DBL_MAX;

    display.window->clear(sf::Color::White);

    // Position of the pendulum in the window
    sf::Sprite spritePendulum(display.texturePendulum);
    spritePendulum.setPosition(DISPLAY_W / 2, DISPLAY_H / 2);
    spritePendulum.setOrigin(25, 15);

    // Convert the angle to degree with the offset to match the python training
    double angle = 180.f - state * 180.f / ((float)M_PI);
    spritePendulum.setRotation(angle);

    // Display the pendulum
    display.window->draw(spritePendulum);

    if (fabs(torque) > 0.0) {
        double scale = std::sqrt(std::fabs(torque));
        // Position of the arrow in the window
        const int arrowWidth = (int)(178 * scale);
        const int arrowHeight = (int)(69 * scale);

        sf::Sprite spriteArrow(display.textureArrow);
		
        
        if (torque < 0.0) {
            spriteArrow.setScale(scale, scale);
			spriteArrow.setPosition((DISPLAY_W - arrowWidth) / 2, (int)(DISPLAY_H / 2 + 14 + scale * 50.0));
        } else {
			spriteArrow.setScale(-scale, scale);
			spriteArrow.setPosition((DISPLAY_W + arrowWidth) / 2, (int)(DISPLAY_H / 2 + 14 + scale * 50.0));
		}
		

        // Display arrow
        display.window->draw(spriteArrow);
    }

    // Print Generation text
    char generationString[100];
    sprintf(generationString, "   gen: %04lld", generation);
    Renderer::displayText(generationString, 0, 0);

    // Print FrameNumber text
    char frameNumber[17];
    sprintf(frameNumber, "frame: %4lld", frame);
    Renderer::displayText(frameNumber, 0, 22);

    // Proceed to the actual display
    display.window->display();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - previousFrameTime;
    double frameTime = elapsed.count();

    // Sleep to maintain the desired frame rate
    if (frameTime < timeDelta) {
        sf::sleep(sf::seconds(timeDelta - frameTime));
    }

    previousFrameTime = std::chrono::high_resolution_clock::now();

    // Static action selected. 
    // Action is reset only when key is released.
    // This is needed because repeated action are not grabbed at every frame 
    // even when the key remains pressed.
    static int action = 0;

    sf::Event event;
    // Grab all next events off the queue.
    while (display.window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
            case sf::Keyboard::Q:
                action = INT_MIN;
                break;
            case sf::Keyboard::S:
                action = -3;
                break;
            case sf::Keyboard::D:
                action = -2;
                break;
            case sf::Keyboard::F:
                action = -1;
                break;
            case sf::Keyboard::J:
                action = 1;
                break;
            case sf::Keyboard::K:
                action = 2;
                break;
            case sf::Keyboard::L:
                action = 3;
                break;
            }
            break;
        case sf::Event::Closed:
            action = INT_MIN;
            break;
        case sf::Event::KeyReleased:
            action = 0;
            break;
        default:
            break;
        }
        // Exit while loop on exit
        if (action == INT_MIN) {
            break;
        }
    }

    return action;
}

void Renderer::replayThread(std::atomic<bool>& exit, std::atomic<bool>& doDisplay, std::atomic<uint64_t>& generation, double delta, std::deque<std::tuple<uint64_t, double, double>>& replay) {
    // Init Display
    renderInit();

    // Initialize shared var
    exit = false;
    double angleDisplay = M_PI;
    double torqueDisplay = 0.0;
    uint64_t frame = 0;
    std::deque<std::tuple<uint64_t, double, double>> localReplay;

    while (!exit) {

        // Was a replay requested?
        if (doDisplay) {
            // copy the replay 
            localReplay = replay;
            doDisplay = false;
        }

        if (!localReplay.empty()) {
            angleDisplay = (double)std::get<1>(localReplay.front());
            torqueDisplay = (double)std::get<2>(localReplay.front());
            frame = std::get<0>(localReplay.front());
            localReplay.pop_front();
        }

        int event = Renderer::renderEnv(angleDisplay, torqueDisplay, frame, generation, delta);
        switch (event) {
        case INT_MIN:
            exit = true;
            doDisplay = false;
            break;
        case 0:
        default:
            // Nothing to do
            break;
        }
    }
    Renderer::renderFinalize();
    printf("\nProgram will terminate at the end of next generation.\n");
    std::cout.flush();
}

void Renderer::renderFinalize() {
    delete display.window;
}
