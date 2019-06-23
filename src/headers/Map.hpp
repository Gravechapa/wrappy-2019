#pragma once

#include <cstdint>
#include <vector>
#include <SFML/System.hpp>

enum soil: short
{
    EMPTY = 0,
    PREPARED,
    WALL,
    OBSTACLE
};

class Map
{
public:
    Map(){}
    Map(uint32_t x, uint32_t y,
        std::vector<sf::Vector2<uint32_t>> &contourMap,
        std::vector<std::vector<sf::Vector2<uint32_t>>> &obstacleMaps);

    const std::vector<std::vector<short>>& getMap() const;

private:

    std::vector<std::vector<short>> _mineMap;

};
