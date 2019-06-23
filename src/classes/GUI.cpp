#include "GUI.hpp"
#include <cmath>

GUI::GUI(const std::string &name):
    _window(sf::RenderWindow (sf::VideoMode(_resolution.x, _resolution.y),
                              "wrappy-2019 " + name, sf::Style::Close))
{
    _window.setFramerateLimit(120);
    _tileMap.setPrimitiveType(sf::Triangles);
    _botTexture.setRadius(std::min(_tileSize.x, _tileSize.y) / 4);
    _botTexture.setFillColor(sf::Color::Red);
}

void GUI::updateMap(const Map &map)
{
    _tileMap.clear();
    auto floatingResolution = sf::Vector2f(_resolution);
    const auto& mineMap = map.getMap();
    if ((floatingResolution.x / floatingResolution.y) * mineMap.size()
            >= mineMap[0].size())
    {
        float scale = (mineMap.size() * _tileSize.y) / floatingResolution.y;
        _view.setSize({floatingResolution.x * scale, mineMap.size() * _tileSize.y});
    }
    else
    {
        float scale = (mineMap[0].size() * _tileSize.x) / floatingResolution.x;
        _view.setSize({mineMap[0].size() * _tileSize.x, floatingResolution.y * scale});
    }
    _view.setCenter(mineMap[0].size() * _tileSize.x / 2.f, mineMap.size() * -_tileSize.y / 2.f);

    for (uint32_t i = 0; i < mineMap.size(); ++i)
    {
        for (uint32_t j = 0; j < mineMap[0].size(); ++j)
        {
            sf::Color color;
            //sf::Color color2 = sf::Color::Red;
            switch (mineMap[i][j])
            {
                case Map::WALL:
                    color = sf::Color::Black;
                    break;
                case Map::OBSTACLE:
                    color = sf::Color(60, 60, 60);
                    break;
                case Map::EMPTY:
                    color = sf::Color::White;
                    break;
                case Map::PREPARED:
                    color = sf::Color::Yellow;
                    break;
            }

            _tileMap.append(sf::Vertex(sf::Vector2f(j * _tileSize.x,
                                                    i * -_tileSize.y), color));

            _tileMap.append(sf::Vertex(sf::Vector2f((j + 1) * _tileSize.x,
                                                    i * -_tileSize.y), color));

            _tileMap.append(sf::Vertex(sf::Vector2f((j + 1) * _tileSize.x,
                                                    (i + 1) * -_tileSize.y), color));

            _tileMap.append(sf::Vertex(sf::Vector2f((j + 1) * _tileSize.x,
                                                    (i + 1) * -_tileSize.y), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(j * _tileSize.x,
                                                    (i + 1) * -_tileSize.y), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(j * _tileSize.x,
                                                    i * -_tileSize.y), color));
        }
    }
}

void GUI::updateBoosters(const std::list<Booster> &boosters)
{
    _boostersTextures.clear();
    for (auto &booster : boosters)
    {
        auto rad = std::min(_tileSize.x, _tileSize.y) / 4;
        sf::CircleShape circle(rad);

        switch (booster.getType())
        {
            case Booster::MANIPULATORBUFFB:
                circle.setFillColor(sf::Color::Green);
                break;
            case Booster::CLONINGBUFF:
                circle.setFillColor(sf::Color(255, 0, 255));
                break;
            case Booster::FASTBUFF:
                circle.setFillColor(sf::Color(70, 130, 180));
                break;
            case Booster::DRILLBUFF:
                circle.setFillColor(sf::Color(60, 60, 60));
                break;
            case Booster::TELEPORTBUFF:
                circle.setFillColor(sf::Color(218, 165, 32));
                break;
            case Booster::BUFFX:
                circle.setFillColor(sf::Color(0, 0, 0));
                break;
        }

        circle.setPosition((booster.getCoords().x * _tileSize.x) + _tileSize.x / 2 - rad,
                           (booster.getCoords().y * -_tileSize.y) - _tileSize.y / 2 - rad);
        _boostersTextures.push_back(circle);
    }
}

void GUI::updateBot(const Bot &bot)
{
    _botTexture.setPosition((bot.getCoords().x * _tileSize.x) + _tileSize.x / 2 - _botTexture.getRadius(),
                            (bot.getCoords().y * -_tileSize.y) - _tileSize.y / 2 - _botTexture.getRadius());
    _manipulatorTextures.clear();
    for (auto& dot : bot.getManipulator())
    {
        if (dot.second)
        {
            double rad = bot.getDirection() * M_PI / 180;
            int32_t manX = (dot.first.x * std::cos(rad) - dot.first.y * std::sin(rad)) + bot.getCoords().x;
            int32_t manY = (dot.first.y * std::cos(rad) - dot.first.x * std::sin(rad)) + bot.getCoords().y;

            sf::RectangleShape rectangle(_tileSize);
            rectangle.setPosition(manX * _tileSize.x, (manY + 1) * -_tileSize.y);
            rectangle.setFillColor(sf::Color(220, 20, 60, 90));
            _manipulatorTextures.push_back(rectangle);
        }

    }
}

bool GUI::checkCloseEvent()
{
    bool result = false;
    sf::Event event;
    while (_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            result = true;
            _window.close();
        }
    }
    return result;
}

void GUI::draw()
{
    _window.clear(sf::Color::Black);
    _window.setView(_view);
    _window.draw(_tileMap);
    for (auto &booster : _boostersTextures)
    {
        _window.draw(booster);
    }
    _window.draw(_botTexture);
    for (auto &manipulator : _manipulatorTextures)
    {
        _window.draw(manipulator);
    }
    _window.display();
}
