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

uint8_t Bot::getSpeed() const
{
    return _speed;
}

bool Bot::hasDrill() const
{
    return _drill;
}

const std::list<std::pair<sf::Vector2<int32_t>, bool>>& Bot::getManipulator() const
{
    return _manipulator;
}

void Bot::move(sf::Vector2<int32_t> coords)
{
    _coords.x += coords.x;
    _coords.y += coords.y;
}

void Bot::rotate(bool clockwise)
{
    if (!clockwise)
    {
        _direction = static_cast<Direction>((_direction + 90) % 360);
    }
    else
    {
        if (_direction == 0)
        {
            _direction = DOWN;
        }
        else
        {
            _direction = static_cast<Direction>(_direction - 90);
        }
    }
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
            it->second += time;
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
            case Booster::MANIPULATORBUFF:
            {
                auto it = std::find_if(_manipulator.begin(), _manipulator.end(),
                             [coords](const std::pair<sf::Vector2<int32_t>, bool>& el){
                    auto &coordsVal = coords.value();
                    bool result = el.first == coordsVal + sf::Vector2<int32_t>(1, 0);
                    result = result || el.first == coordsVal + sf::Vector2<int32_t>(-1, 0);
                    result = result || el.first == coordsVal + sf::Vector2<int32_t>(0, 1);
                    result = result || el.first == coordsVal + sf::Vector2<int32_t>(0, -1);
                    return result;
                });
                if (it == _manipulator.end())
                {
                    return false;
                }
                _manipulator.push_back(std::pair(coords.value(), false));
                break;
            }
            case Booster::CLONINGBUFF:
                //todo
                break;
            case Booster::FASTBUFF:
                activateBooster(Booster::FASTTIME);
                _speed = 2;
                break;
            case Booster::DRILLBUFF:
                activateBooster(Booster::DRILLTIME);
                _drill = true;
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
