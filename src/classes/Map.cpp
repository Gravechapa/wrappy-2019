#include "Map.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cmath>

Map::Map(uint32_t x, uint32_t y,
    std::vector<sf::Vector2<uint32_t>> &contourMap,
    std::vector<std::vector<sf::Vector2<uint32_t>>> &obstacleMaps)
{
    _mineMap = std::vector(y, std::vector(x, short{WALL}));
    auto dotMaps = std::vector((y * 2) + 1, std::vector((x * 2) + 1, short{WALL}));

    auto drawContour = [&dotMaps](short target, std::vector<sf::Vector2<uint32_t>> &contour){
        for (uint32_t k = 0; k < contour.size(); ++k)
        {
            uint32_t i, j;
            auto next = (k + 1) % contour.size();
            if (contour[k].x == contour[next].x)
            {
                if (contour[k].y > contour[next].y)
                {
                    i = contour[next].y;
                    j = contour[k].y;
                }
                else
                {
                    i = contour[k].y;
                    j = contour[next].y;
                }
                i *= 2;
                j *= 2;
                for (; i <= j; ++i)
                {
                    dotMaps[i][contour[next].x * 2] = target;
                }
            }
            else
            {
                if (contour[k].x > contour[next].x)
                {
                    i = contour[next].x;
                    j = contour[k].x;
                }
                else
                {
                    i = contour[k].x;
                    j = contour[next].x;
                }
                i *= 2;
                j *= 2;
                for (; i <= j; ++i)
                {
                    dotMaps[contour[next].y * 2][i] = target;
                }
            }
        }
    };

    auto fill = [&dotMaps](short target,
            const std::vector<std::vector<sf::Vector2<uint32_t>>> &contours){
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
                        const std::vector<sf::Vector2<uint32_t>> *contour = nullptr;
                        int32_t dotNumber;
                        for(auto &cont : contours)
                        {
                            auto it = std::find_if(
                                cont.begin(), cont.end(),
                                [i, j](const sf::Vector2<uint32_t>& el){
                                return el.x == (j - 1) / 2 && el.y == i / 2;
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
                        if ((*contour)[dotNumber].y == (*contour)[(dotNumber + 1) % contour->size()].y)
                        {
                            nextVectorDirection = (*contour)[(dotNumber + 2) % contour->size()].y -
                                    (*contour)[(dotNumber + 1) % contour->size()].y;
                        }
                        else
                        {
                            nextVectorDirection = (*contour)[(dotNumber + 1) % contour->size()].y -
                                    (*contour)[(dotNumber)].y;
                        }

                        if ((*contour)[dotNumber].y ==
                                (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].y)
                        {
                            prevVectorDirection =
                                    (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].y -
                                    (*contour)[dotNumber < 2 ? contour->size() - 2 : dotNumber - 2].y;
                        }
                        else
                        {
                            prevVectorDirection = (*contour)[dotNumber].y -
                                    (*contour)[dotNumber == 0 ? contour->size() - 1 : dotNumber - 1].y;
                        }

                        if (nextVectorDirection == 0 || prevVectorDirection == 0)
                        {
                            throw std::runtime_error("Bad vertexes");
                        }
                        nextVectorDirection = nextVectorDirection > 0 ? 1 : -1;
                        prevVectorDirection = prevVectorDirection > 0 ? 1 : -1;
                        if (nextVectorDirection + prevVectorDirection == 0)
                        {
                            if ((j - 1) / 2 == (*contour)[(dotNumber + 1) % contour->size()].x)
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
    fill(EMPTY, std::vector<std::vector<sf::Vector2<uint32_t>>>(1, contourMap));

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

bool Map::check(Bot &bot, sf::Vector2<int32_t> direction)
{

    int32_t targetX = static_cast<int32_t>(bot.getCoords().x) + direction.x;
    int32_t targetY = static_cast<int32_t>(bot.getCoords().y) + direction.y;

    if (targetY < _mineMap.size() && targetX < _mineMap[0].size() && targetY >= 0 && targetX >= 0)
    {
        if (_mineMap[targetY][targetX] == WALL || _mineMap[targetY][targetX] == OBSTACLE)
        {
            if (bot.hasDrill())
            {
                _mineMap[targetY][targetX] = EMPTY;
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

void Map::updateMap(Bot &bot)
{
    double rad = bot.getDirection() * M_PI / 180;
    for (auto& dot : bot.getManipulator())
    {

        int32_t manX = std::round((dot.first.x * std::cos(rad) - dot.first.y * std::sin(rad))) + bot.getCoords().x;
        int32_t manY = std::round((dot.first.y * std::cos(rad) + dot.first.x * std::sin(rad))) + bot.getCoords().y;

        if (manY < _mineMap.size() && manX < _mineMap[0].size() && manY >= 0 && manX >= 0)
        {
            if (_mineMap[manY][manX] == EMPTY)
            {
                _mineMap[manY][manX] = PREPARED;
                const_cast<bool&>(dot.second) = true;
                continue;
            }
            if (_mineMap[manY][manX] == PREPARED)
            {
                const_cast<bool&>(dot.second) = true;
                continue;
            }
            const_cast<bool&>(dot.second) = false;
        }
        else
        {
            const_cast<bool&>(dot.second) = false;
        }
    }
}

const std::vector<std::vector<short>>& Map::getMap() const
{
    return _mineMap;
}
