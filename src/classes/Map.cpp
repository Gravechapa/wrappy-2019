#include "Map.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Map::Map(uint32_t x, uint32_t y,
    std::vector<std::pair<uint32_t, uint32_t>> &contourMap,
    std::vector<std::vector<std::pair<uint32_t, uint32_t>>> &obstacleMaps)
{
    _mineMap = std::vector(y, std::vector(x, short{WALL}));
    auto dotMaps = std::vector((y * 2) + 1, std::vector((x * 2) + 1, short{WALL}));

    auto drawContour = [&dotMaps](short target, std::vector<std::pair<uint32_t, uint32_t>> &contour){
        for (uint32_t k = 0; k < contour.size(); ++k)
        {
            uint32_t i, j;
            auto next = (k + 1) % contour.size();
            if (contour[k].first == contour[next].first)
            {
                if (contour[k].second > contour[next].second)
                {
                    i = contour[next].second;
                    j = contour[k].second;
                }
                else
                {
                    i = contour[k].second;
                    j = contour[next].second;
                }
                i *= 2;
                j *= 2;
                for (; i <= j; ++i)
                {
                    dotMaps[i][contour[next].first * 2] = target;
                }
            }
            else
            {
                if (contour[k].first > contour[next].first)
                {
                    i = contour[next].first;
                    j = contour[k].first;
                }
                else
                {
                    i = contour[k].first;
                    j = contour[next].first;
                }
                i *= 2;
                j *= 2;
                for (; i <= j; ++i)
                {
                    dotMaps[contour[next].second * 2][i] = target;
                }
            }
        }
    };

    auto fill = [&dotMaps](short target,
            const std::vector<std::vector<std::pair<uint32_t, uint32_t>>> &contours){
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
                        const std::vector<std::pair<uint32_t, uint32_t>> *contour = nullptr;
                        int32_t dotNumber;
                        for(auto &cont : contours)
                        {
                            auto it = std::find_if(
                                cont.begin(), cont.end(),
                                [i, j](const std::pair<uint32_t, uint32_t>& x){
                                return x.first == (j - 1) / 2 && x.second == i / 2;
                            });
                            if (it !=  cont.end())
                            {
                                dotNumber = std::distance(cont.begin(), it);
                                contour = &cont;
                                break;
                            }
                        }
                        if (!contour)
                        {
                            std::cout << "Warning: edge not found" << std::endl;
                            intersectionDetectCounter = 0;
                            continue;
                        }

                        int32_t nextVectorDirection;
                        int32_t prevVectorDirection;
                        if ((*contour)[dotNumber].second == (*contour)[(dotNumber + 1) % contour->size()].second)
                        {
                            nextVectorDirection = (*contour)[(dotNumber + 2) % contour->size()].second -
                                    (*contour)[(dotNumber + 1) % contour->size()].second;
                        }
                        else
                        {
                            nextVectorDirection = (*contour)[(dotNumber + 1) % contour->size()].second -
                                    (*contour)[(dotNumber)].second;
                        }

                        if ((*contour)[dotNumber].second ==
                                (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].second)
                        {
                            prevVectorDirection =
                                    (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].second -
                                    (*contour)[dotNumber < 2 ? contour->size() - 2 : dotNumber - 2].second;
                        }
                        else
                        {
                            prevVectorDirection = (*contour)[dotNumber].second -
                                    (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].second;
                        }

                        if (nextVectorDirection == 0 || prevVectorDirection == 0)
                        {
                            throw std::runtime_error("Bad vertexes");
                        }
                        nextVectorDirection = nextVectorDirection > 0 ? 1 : -1;
                        prevVectorDirection = prevVectorDirection > 0 ? 1 : -1;
                        if (nextVectorDirection + prevVectorDirection == 0)
                        {
                            if ((j - 1) / 2 == (*contour)[(dotNumber + 1) % contour->size()].first)
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

    drawContour(EMPTY, contourMap);
    fill(EMPTY, std::vector<std::vector<std::pair<uint32_t, uint32_t>>>(1, contourMap));

    for (uint32_t i = 0; i < dotMaps.size() - 2; i += 2)
    {
        for (uint32_t j = 0; j < dotMaps[0].size() - 2; j += 2)
        {
            bool result = true;
            for (uint32_t k = 0; k < 3; ++k)
            {
                for (uint32_t n = 0; n < 3; ++n)
                {
                    result = result && dotMaps[i + k][j + n] == EMPTY;
                }
            }
            if (result)
            {
                _mineMap[i / 2][j / 2] = EMPTY;
            }
        }
    }

    dotMaps = std::vector((y * 2) + 1, std::vector((x * 2) + 1, short{WALL}));

    for (auto &obstacleMap : obstacleMaps)
    {
        drawContour(OBSTACLE, obstacleMap);
    }

    fill(OBSTACLE, obstacleMaps);

    for (uint32_t i = 0; i < dotMaps.size() - 2; i += 2)
    {
        for (uint32_t j = 0; j < dotMaps[0].size() - 2; j += 2)
        {
            bool result = true;
            for (uint32_t k = 0; k < 3; ++k)
            {
                for (uint32_t n = 0; n < 3; ++n)
                {
                    result = result && dotMaps[i + k][j + n] == OBSTACLE;
                }
            }
            if (result)
            {
                _mineMap[i / 2][j / 2] = OBSTACLE;
            }
        }
    }
}

const std::vector<std::vector<short>>& Map::getMap() const
{
    return _mineMap;
}
