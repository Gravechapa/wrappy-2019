#include "Booster.hpp"

const short Booster::FASTTIME = 50;
const short Booster::DRILLTIME = 30;


Booster::Booster(BoosterType type, const sf::Vector2<uint32_t> &coords):
    _type(type), _coords(coords)
{}

Booster::BoosterType Booster::getType() const
{
    return _type;
}

sf::Vector2<uint32_t> Booster::getCoords() const
{
    return _coords;
}
