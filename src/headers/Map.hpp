#pragma once

#include <cstdint>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Bot.hpp"

class Map
{
public:
    enum soil: short
    {
        EMPTY = 0,
        PREPARED,
        WALL,
        OBSTACLE
    };

    Map(){}
    Map(uint32_t x, uint32_t y,
        std::vector<sf::Vector2<uint32_t>> &contourMap,
        std::vector<std::vector<sf::Vector2<uint32_t>>> &obstacleMaps);

    bool check(Bot &bot, sf::Vector2<int32_t> direction);
    void updateMap(Bot &bot);
    const std::vector<std::vector<short>>& getMap() const;

private:

    std::vector<std::vector<short>> _mineMap;

};
