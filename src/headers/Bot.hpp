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
        UP = 90,
        LEFT = 180,
        DOWN = 270
    };

    Bot(){}
    Bot(const sf::Vector2<uint32_t> &coords);

    sf::Vector2<uint32_t> getCoords() const;
    Direction getDirection() const;
    uint8_t getSpeed() const;
    bool hasDrill() const;
    const std::list<std::pair<sf::Vector2<int32_t>, bool>>& getManipulator() const;

    void move(sf::Vector2<int32_t> coords);
    void rotate(bool clockwise);
    void updateTimers();
    void addBooster(Booster::BoosterType type);
    bool useBooster(Booster::BoosterType type, std::optional<sf::Vector2<int32_t>> coords);

private:
    sf::Vector2<uint32_t> _coords;
    uint8_t _speed{1};
    bool _drill{false};
    Direction _direction = RIGHT;
    std::list<std::pair<sf::Vector2<int32_t>, bool>> _manipulator;

    std::map<Booster::BoosterType, uint32_t> _boosters;
    std::list<std::pair<Booster::BoosterType, short>> _activeBoosters;
};
