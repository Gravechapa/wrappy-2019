#include "GUI.hpp"

GUI::GUI(): _window(sf::RenderWindow (sf::VideoMode(_resolution.x, _resolution.y), "wrappy-2019", sf::Style::Close))
{
    _window.setFramerateLimit(120);
    _tileMap.setPrimitiveType(sf::Triangles);
}

void GUI::updateMap(Map &map)
{
    const auto& mineMap = map.getMap();
    if ((_resolution.x / _resolution.y) * mineMap.size() >= mineMap[0].size())
    {
        float scale = (mineMap.size() * _tileSize.y) / static_cast<float>(_resolution.y);
        _view.setSize({_resolution.x * scale, static_cast<float>(mineMap.size() * _tileSize.y)});
    }
    else
    {
        float scale = (mineMap[0].size() * _tileSize.x) / static_cast<float>(_resolution.x);
        _view.setSize({static_cast<float>(mineMap[0].size() * _tileSize.x), _resolution.y * scale});
    }
    _view.setCenter(mineMap[0].size() * _tileSize.x / 2.f, mineMap.size() * _tileSize.y / 2.f);

    for (uint32_t i = 0; i < mineMap.size(); ++i)
    {
        for (uint32_t j = 0; j < mineMap[0].size(); ++j)
        {
            sf::Color color;
            //sf::Color color2 = sf::Color::Red;
            switch (mineMap[(mineMap.size() - i - 1)][j])
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

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>(j * _tileSize.x),
                static_cast<float>(i * _tileSize.y)), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>((j + 1) * _tileSize.x),
                static_cast<float>(i * _tileSize.y)), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>((j + 1) * _tileSize.x),
                static_cast<float>((i + 1) * _tileSize.y)), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>((j + 1) * _tileSize.x),
                static_cast<float>((i + 1) * _tileSize.y)), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>(j * _tileSize.x),
                static_cast<float>((i + 1) * _tileSize.y)), color));

            _tileMap.append(sf::Vertex(sf::Vector2f(static_cast<float>(j * _tileSize.x),
                static_cast<float>(i * _tileSize.y)), color));
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
    _window.display();
}
