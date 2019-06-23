#include "Booster.hpp"

Booster::Booster(BoosterType type, const sf::Vector2<uint32_t> &coords):
    _type(type), _coords(coords)
{}

BoosterType Booster::getType() const
{
    return _type;
}

sf::Vector2<uint32_t> Booster::getCoords() const
{
    return _coords;
}
