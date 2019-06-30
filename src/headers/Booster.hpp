#pragma once

#include <SFML/System.hpp>

class Booster
{
public:
    enum BoosterType: short
    {
        MANIPULATORBUFF = 0,
        CLONINGBUFF,
        FASTBUFF,
        DRILLBUFF,
        TELEPORTBUFF,
        BUFFX
    };

    static const short FASTTIME;
    static const short DRILLTIME;

    Booster(BoosterType type, const sf::Vector2<uint32_t> &coords);

    BoosterType getType() const;
    sf::Vector2<uint32_t> getCoords() const;
private:

    const BoosterType _type;
    const sf::Vector2<uint32_t> _coords;
};
