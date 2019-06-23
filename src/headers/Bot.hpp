#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <map>
#include <list>
#include "Booster.hpp"

class Bot
{
public:
    enum Direction: short
    {
        RIGHT = 0,
        DOWN = 90,
        LEFT = 180,
        UP = 270
    };

    Bot(){}
    Bot(const sf::Vector2<uint32_t> &coords);

    sf::Vector2<uint32_t> getCoords() const;
    Direction getDirection() const;
    const std::list<std::pair<sf::Vector2<int32_t>, bool>>& getManipulator() const;

    void addBooster(Booster::BoosterType type);
    bool useBooster(Booster::BoosterType type, std::optional<sf::Vector2<int32_t>> coords);

private:
    sf::Vector2<uint32_t> _coords;
    char _speed{1};
    Direction _direction = RIGHT;
    std::list<std::pair<sf::Vector2<int32_t>, bool>> _manipulator;

    std::map<Booster::BoosterType, uint32_t> _boosters;
    std::list<std::pair<Booster::BoosterType, short>> _activeBoosters;
};
