#include "Bot.hpp"

Bot::Bot(const sf::Vector2<uint32_t> &coords): _coords(coords)
{}

sf::Vector2<uint32_t> Bot::getCoords() const
{
    return _coords;
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

bool Bot::useBooster(Booster::BoosterType type)
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
