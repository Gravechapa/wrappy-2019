#include "Map.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Map::Map(uint32_t x, uint32_t y,
    std::vector<std::pair<uint32_t, uint32_t>> &contourMap,
    std::vector<std::queue<std::pair<uint32_t, uint32_t>>> &obstacleMaps)
{
    _mineMap = std::vector(y, std::vector(x, short{WALL}));
    auto dotMaps = std::vector(y + 1, std::vector(x + 1, short{WALL}));

    auto fill = [&dotMaps](short target, std::vector<std::pair<uint32_t, uint32_t>> &contour){
            uint32_t intersectionDetectCounter;
            for (uint32_t i = 0; i < dotMaps.size(); ++i)
            {
                intersectionDetectCounter = 0;
                for (uint32_t j = 0; j < dotMaps[0].size(); ++j)
                {
                    if (dotMaps[i][j] == target)
                    {
                        ++intersectionDetectCounter;
                        continue;
                    }
                    if (intersectionDetectCounter == 0)
                    {
                        continue;
                    }
                    if (intersectionDetectCounter == 1 && dotMaps[i][j] == WALL)
                    {
                        dotMaps[i][j] = target;
                        continue;
                    }
                    if (intersectionDetectCounter > 1 && dotMaps[i][j] == WALL)
                    {
                        if (intersectionDetectCounter == 2)
                        {
                            intersectionDetectCounter = 0;
                            continue;
                        }
                        auto it = std::find_if(
                            contour.begin(), contour.end(),
                            [i, j](const std::pair<uint32_t, uint32_t>& x){
                            return x.first == j - 1 && x.second == i;
                        });
                        if (it ==  contour.end())
                        {
                            std::cout << "Warning: edge not found" << std::endl;
                            intersectionDetectCounter = 0;
                            continue;
                        }
                        int32_t dotNumber = std::distance(contour.begin(), it);
                        int32_t nextVectorDirection;
                        int32_t prevVectorDirection;
                        if (contour[dotNumber].second == contour[(dotNumber + 1) % contour.size()].second)
                        {
                            nextVectorDirection = contour[(dotNumber + 2) % contour.size()].second -
                                    contour[(dotNumber + 1) % contour.size()].second;
                        }
                        else
                        {
                            nextVectorDirection = contour[(dotNumber + 1) % contour.size()].second -
                                    contour[(dotNumber)].second;
                        }

                        if (contour[dotNumber].second ==
                                contour[(dotNumber - 1) < 0 ? contour.size() - dotNumber : dotNumber - 1].second)
                        {
                            prevVectorDirection =
                                    contour[(dotNumber - 1) < 0 ? contour.size() - dotNumber : dotNumber - 1].second -
                                    contour[(dotNumber - 2) < 0 ? contour.size() - dotNumber - 1 : dotNumber - 2].second;
                        }
                        else
                        {
                            prevVectorDirection = contour[dotNumber].second -
                                    contour[(dotNumber - 1) < 0 ? contour.size() - dotNumber : dotNumber - 1].second;
                        }

                        if (nextVectorDirection == 0 || prevVectorDirection == 0)
                        {
                            throw std::runtime_error("Bad vertexes");
                        }
                        nextVectorDirection = nextVectorDirection > 0 ? 1 : -1;
                        prevVectorDirection = prevVectorDirection > 0 ? 1 : -1;
                        if (nextVectorDirection + prevVectorDirection == 0)
                        {
                            if (j - 1 == contour[(dotNumber + 1) % contour.size()].first)
                            {
                                if (nextVectorDirection > 0)
                                {
                                    intersectionDetectCounter = 0;
                                }
                                else
                                {
                                    intersectionDetectCounter = 1;
                                    dotMaps[i][j] = target;
                                }
                            }
                            else
                            {
                                if (nextVectorDirection < 0)
                                {
                                    intersectionDetectCounter = 0;
                                }
                                else
                                {
                                    intersectionDetectCounter = 1;
                                    dotMaps[i][j] = target;
                                }
                            }
                        }
                        else
                        {
                            if (nextVectorDirection > 0)
                            {
                                intersectionDetectCounter = 0;
                            }
                            else
                            {
                                intersectionDetectCounter = 1;
                                dotMaps[i][j] = target;
                            }
                        }
                    }
                    }
                }
    };

    std::pair<uint32_t, uint32_t> buffer;
    for (uint32_t k = 0; k < contourMap.size(); ++k)
    {
        uint32_t i, j;
        auto next = (k + 1) % contourMap.size();
        if (contourMap[k].first == contourMap[next].first)
        {
            if (contourMap[k].second > contourMap[next].second)
            {
                i = contourMap[next].second;
                j = contourMap[k].second;
            }
            else
            {
                i = contourMap[k].second;
                j = contourMap[next].second;
            }
            for (; i <= j; ++i)
            {
                dotMaps[i][contourMap[next].first] = EMPTY;
            }
        }
        else
        {
            if (contourMap[k].first > contourMap[next].first)
            {
                i = contourMap[next].first;
                j = contourMap[k].first;
            }
            else
            {
                i = contourMap[k].first;
                j = contourMap[next].first;
            }
            for (; i <= j; ++i)
            {
                dotMaps[contourMap[next].second][i] = EMPTY;
            }
        }
    }

    fill(EMPTY, contourMap);
    for (uint32_t i = 0; i < dotMaps.size() - 1; ++i)
    {
        for (uint32_t j = 0; j < dotMaps[0].size() - 1; ++j)
        {
            bool d0 = dotMaps[i][j] == EMPTY;
            bool d1 = dotMaps[i][j + 1] == EMPTY;
            bool d2 = dotMaps[i + 1][j] == EMPTY;
            bool d3 = dotMaps[i + 1][j + 1] == EMPTY;
            if (d0 && d1 && d2 && d3)
            {
                _mineMap[i][j] = EMPTY;
            }
        }
    }

    /*for (auto &obstacleMap : obstacleMaps)
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
                    dotMaps[i][obstacleMap.front().first] = OBSTACLE;
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
                    dotMaps[obstacleMap.front().second][i] = OBSTACLE;
                }
            }
            buffer = obstacleMap.front();
            obstacleMap.pop();
        }
    }*/
}

const std::vector<std::vector<short>>& Map::getMap() const
{
    return _mineMap;
}
