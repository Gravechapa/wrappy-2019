#pragma once

#include <SFML/System.hpp>

enum BoosterType: short
{
    MANIPULATORBUFFB = 0,
    CLONINGBUFF,
    FASTBUFF,
    DRILLBUFF,
    TELEPORTBUFF,
    BUFFX
};

class Booster
{
public:
    Booster(BoosterType type, const sf::Vector2<uint32_t> &coords);

    BoosterType getType() const;
    sf::Vector2<uint32_t> getCoords() const;
private:

    const BoosterType _type;
    const sf::Vector2<uint32_t> _coords;
};
