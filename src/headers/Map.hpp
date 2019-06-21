#pragma once

#include <queue>
#include <cstdint>

enum Booster: short
{
    CODEB = 1,
    CODEF,
    CODEL,
    CODEX
};

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
        std::queue<std::pair<uint32_t, uint32_t>> &contourMap,
        std::vector<std::queue<std::pair<uint32_t, uint32_t>>> &obstacleMaps);

    const std::vector<std::vector<short>>& getMap() const;

private:

    std::vector<std::vector<short>> _mineMap;

};
