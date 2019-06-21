#pragma once

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class GUI
{
public:
    GUI();

    void updateMap(Map &map);
    bool checkCloseEvent();
    void draw();

private:
    const sf::Vector2<uint32_t> _tileSize{64, 64};
    const sf::Vector2<uint32_t> _resolution{1200, 675};

    sf::RenderWindow _window;
    sf::View _view;
    sf::VertexArray _tileMap;
};
