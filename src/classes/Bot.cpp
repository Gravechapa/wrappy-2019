#include "Bot.hpp"

Bot::Bot(const sf::Vector2<uint32_t> &coords): _coords(coords)
{
    _manipulator.push_back(std::pair(sf::Vector2<int32_t>{0, 0}, false));
    _manipulator.push_back(std::pair(sf::Vector2<int32_t>{1, 0}, false));
    _manipulator.push_back(std::pair(sf::Vector2<int32_t>{1, 1}, false));
    _manipulator.push_back(std::pair(sf::Vector2<int32_t>{1, -1}, false));
}

sf::Vector2<uint32_t> Bot::getCoords() const
{
    return _coords;
}

Bot::Direction Bot::getDirection() const
{
    return _direction;
}

const std::list<std::pair<sf::Vector2<int32_t>, bool>>& Bot::getManipulator() const
{
    return _manipulator;
}

void Bot::addBooster(Booster::BoosterType type)
{
    auto it = _boosters.find(type);
    if (it != _boosters.end())
    {
        ++it->second;
    }
    else
    {
        _boosters[type] = 1;
    }
}

bool Bot::useBooster(Booster::BoosterType type, std::optional<sf::Vector2<int32_t>> coords)
{
    auto activateBooster = [type, this](short time){
        auto it = std::find_if(_activeBoosters.begin(), _activeBoosters.end(),
            [type](const std::pair<Booster::BoosterType, short>& el){return el.first == type;});
        if (it != _activeBoosters.end())
        {
            it->second = time;
        }
        else
        {
            _activeBoosters.push_back(std::pair(type, time));
        }
    };

    auto it = _boosters.find(type);
    if (it != _boosters.end())
    {
        --it->second;
        switch (type)
        {
            case Booster::MANIPULATORBUFFB:
                //todo
                break;
            case Booster::CLONINGBUFF:
                //todo
                break;
            case Booster::FASTBUFF:
                activateBooster(Booster::FASTTIME);
                break;
            case Booster::DRILLBUFF:
                activateBooster(Booster::DRILLTIME);
                break;
            case Booster::TELEPORTBUFF:
                //todo
                break;
            case Booster::BUFFX:
                return false;
                break;
        }
        return true;
    }
    return false;
}
