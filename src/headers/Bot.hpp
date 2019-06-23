#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <map>
#include <list>
#include "Booster.hpp"

class Bot
{
public:
    Bot(){}
    Bot(const sf::Vector2<uint32_t> &coords);

    sf::Vector2<uint32_t> getCoords() const;

    void addBooster(Booster::BoosterType type);
    bool useBooster(Booster::BoosterType type);

private:
    sf::Vector2<uint32_t> _coords;
    short speed{1};

    std::map<Booster::BoosterType, uint32_t> _boosters;
    std::list<std::pair<Booster::BoosterType, short>> _activeBoosters;
};
