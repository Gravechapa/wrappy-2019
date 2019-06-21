#include "Map.hpp"

Map::Map(uint32_t x, uint32_t y,
    std::queue<std::pair<uint32_t, uint32_t>> &contourMap,
    std::vector<std::queue<std::pair<uint32_t, uint32_t>>> &obstacleMaps)
{
    _mineMap = std::vector(y + 1, std::vector(x + 1, short{WALL}));

    std::pair<uint32_t, uint32_t> buffer;
    if (!contourMap.empty())
    {
        buffer = contourMap.front();
        contourMap.push(contourMap.front());
        contourMap.pop();
    }
    while (!contourMap.empty())
    {
        uint32_t i, j;
        if (buffer.first == contourMap.front().first)
        {
            if (buffer.second > contourMap.front().second)
            {
                i = contourMap.front().second;
                j = buffer.second;
            }
            else
            {
                i = buffer.second;
                j = contourMap.front().second;
            }
            for (; i <= j; ++i)
            {
                _mineMap[i][contourMap.front().first] = EMPTY;
            }
        }
        else
        {
            if (buffer.first > contourMap.front().first)
            {
                i = contourMap.front().first;
                j = buffer.first;
            }
            else
            {
                i = buffer.first;
                j = contourMap.front().first;
            }
            for (; i <= j; ++i)
            {
                _mineMap[contourMap.front().second][i] = EMPTY;
            }
        }
        buffer = contourMap.front();
        contourMap.pop();
    }

    for (auto &obstacleMap : obstacleMaps)
    {
        if (!obstacleMap.empty())
        {
            buffer = obstacleMap.front();
            obstacleMap.push(obstacleMap.front());
            obstacleMap.pop();
        }
        while (!obstacleMap.empty())
        {
            uint32_t i, j;
            if (buffer.first == obstacleMap.front().first)
            {
                if (buffer.second > obstacleMap.front().second)
                {
                    i = obstacleMap.front().second;
                    j = buffer.second;
                }
                else
                {
                    i = buffer.second;
                    j = obstacleMap.front().second;
                }
                for (; i <= j; ++i)
                {
                    _mineMap[i][obstacleMap.front().first] = OBSTACLE;
                }
            }
            else
            {
                if (buffer.first > obstacleMap.front().first)
                {
                    i = obstacleMap.front().first;
                    j = buffer.first;
                }
                else
                {
                    i = buffer.first;
                    j = obstacleMap.front().first;
                }
                for (; i <= j; ++i)
                {
                    _mineMap[obstacleMap.front().second][i] = OBSTACLE;
                }
            }
            buffer = obstacleMap.front();
            obstacleMap.pop();
        }
    }
}

const std::vector<std::vector<short>>& Map::getMap() const
{
    return _mineMap;
}
