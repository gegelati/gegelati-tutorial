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
#include <cinttypes>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "renderer.h"

struct sfmlDisplay {
    sf::RenderWindow* window;
    sf::Texture texturePendulum;
    sf::Texture textureArrow;
    sf::Font font;
};

static sfmlDisplay display;
static std::chrono::high_resolution_clock::time_point previousFrameTime;

void Renderer::renderInit() {
    display.window = new sf::RenderWindow(sf::VideoMode({DISPLAY_W, DISPLAY_H}), "Environment_Display");

    if (!display.font.openFromFile(PATH_TTF)) {
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
    sf::Text sfText(display.font);
    sfText.setString(text);
    sfText.setCharacterSize(20);
    sfText.setFillColor(sf::Color::Green);
    sfText.setPosition({static_cast<float>(posX), static_cast<float>(posY)});
    display.window->draw(sfText);
}

int Renderer::renderEnv(double state, double torque, uint64_t frame, uint64_t generation, double timeDelta) {
    display.window->clear(sf::Color::White);

    // Position of the pendulum in the window
    sf::Sprite spritePendulum(display.texturePendulum);
    spritePendulum.setPosition({DISPLAY_W / 2.f, DISPLAY_H / 2.f});
    spritePendulum.setOrigin({25.f, 15.f});

    // Convert the angle to degree with the offset to match the python training
    double angle = 180.f - state * 180.f / ((float)M_PI);
    spritePendulum.setRotation(sf::degrees(angle));

    // Display the pendulum
    display.window->draw(spritePendulum);

    if (std::fabs(torque) > 0.0) {
        double scale = std::sqrt(std::fabs(torque));
        // Position of the arrow in the window
        const int arrowWidth = 178 * scale;
        // unused: const int arrowHeight = 69 * scale;

        sf::Sprite spriteArrow(display.textureArrow);

        if (torque < 0.0) {
            spriteArrow.setScale({static_cast<float>(scale), static_cast<float>(scale)});
            spriteArrow.setPosition({static_cast<float>((DISPLAY_W - arrowWidth) / 2), static_cast<float>(DISPLAY_H / 2 + 14 + scale * 50.0)});
        } else {
            spriteArrow.setScale({static_cast<float>(-scale), static_cast<float>(scale)});
            spriteArrow.setPosition({static_cast<float>((DISPLAY_W + arrowWidth) / 2), static_cast<float>(DISPLAY_H / 2 + 14 + scale * 50.0)});
        }

        // Display arrow
        display.window->draw(spriteArrow);
    }

    // Print Generation text
    char generationString[100];
    std::snprintf(generationString, sizeof(generationString), "   gen: %04" PRId64 , generation);
    Renderer::displayText(generationString, 0, 0);

    // Print FrameNumber text
    char frameNumber[17];
    std::snprintf(frameNumber, sizeof(frameNumber), "frame: %4" PRId64, frame);
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

    // Grab all next events off the queue.
    while (const auto event = display.window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            action = INT_MIN;
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::Q:
                action = INT_MIN;
                break;
            case sf::Keyboard::Key::S:
                action = -3;
                break;
            case sf::Keyboard::Key::D:
                action = -2;
                break;
            case sf::Keyboard::Key::F:
                action = -1;
                break;
            case sf::Keyboard::Key::J:
                action = 1;
                break;
            case sf::Keyboard::Key::K:
                action = 2;
                break;
            case sf::Keyboard::Key::L:
                action = 3;
                break;
            default:
                break;
            }
        } else if (event->is<sf::Event::KeyReleased>()) {
            action = 0;
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
            angleDisplay = std::get<1>(localReplay.front());
            torqueDisplay = std::get<2>(localReplay.front());
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
    std::printf("\nProgram will terminate at the end of next generation.\n");
    std::cout.flush();
}

void Renderer::renderFinalize() {
    delete display.window;
}
