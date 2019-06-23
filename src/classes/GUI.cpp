#include "GUI.hpp"

GUI::GUI(const std::string &name):
    _window(sf::RenderWindow (sf::VideoMode(_resolution.x, _resolution.y),
                              "wrappy-2019 " + name, sf::Style::Close))
{
    _window.setFramerateLimit(120);
    _tileMap.setPrimitiveType(sf::Triangles);
}

void GUI::updateMap(Map &map)
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
                case WALL:
                    color = sf::Color::Black;
                    break;
                case OBSTACLE:
                    color = sf::Color(60, 60, 60);
                    break;
                case EMPTY:
                    color = sf::Color::White;
                    break;
                case PREPARED:
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
            case MANIPULATORBUFFB:
                circle.setFillColor(sf::Color::Green);
                break;
            case CLONINGBUFF:
                circle.setFillColor(sf::Color(255, 0, 255));
                break;
            case FASTBUFF:
                circle.setFillColor(sf::Color(70, 130, 180));
                break;
            case DRILLBUFF:
                circle.setFillColor(sf::Color(60, 60, 60));
                break;
            case TELEPORTBUFF:
                circle.setFillColor(sf::Color(218, 165, 32));
                break;
            case BUFFX:
                circle.setFillColor(sf::Color(0, 0, 0));
                break;
        }

        circle.setPosition((booster.getCoords().x * _tileSize.x) + _tileSize.x / 2 - rad,
                           (booster.getCoords().y * -_tileSize.y) - _tileSize.y / 2 - rad);
        _boostersTextures.push_back(circle);
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
    _window.display();
}
