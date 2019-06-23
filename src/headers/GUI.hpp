#pragma once

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Bot.hpp"

class GUI
{
public:
    GUI(const std::string &name);

    void updateMap(const Map &map);
    void updateBoosters(const std::list<Booster> &boosters);
    void updateBot(const Bot &bot);
    bool checkCloseEvent();
    void draw();

private:
    const sf::Vector2f _tileSize{64.f, 64.f};
    const sf::Vector2<uint32_t> _resolution{1600, 900};

    sf::RenderWindow _window;
    sf::View _view;
    sf::VertexArray _tileMap;
    std::list<sf::CircleShape> _boostersTextures;
    sf::CircleShape _botTexture;
};
