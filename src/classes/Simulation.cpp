#include "Simulation.hpp"

#include <fstream>
#include <queue>


Simulation::Simulation(const fs::path &path): _gui(GUI(path.filename()))
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Can't open file: " + path.string());
    }

    auto fileError = [&path, &file](){
        throw std::runtime_error("Corrupt file: " + path.string()
                                 + " pos: " + to_string(file.tellg()));};

    auto readCoord = [&file, &fileError]()->sf::Vector2<uint32_t>{
        sf::Vector2<uint32_t> coord;
        char c;
        file.get(c);
        if (c != '(')
        {
            fileError();
        }

        file >> coord.x;

        file.get(c);
        if (c != ',')
        {
            fileError();
        }

        file >> coord.y;

        file.get(c);
        if (c != ')')
        {
            fileError();
        }
        return coord;
    };

    char c;
    std::vector<sf::Vector2<uint32_t>> contourMapBuffer;
    uint32_t xMax = 0;
    uint32_t yMax = 0;
    while (true)
    {
        auto coord = readCoord();
        xMax = std::max(coord.x, xMax);
        yMax = std::max(coord.y, yMax);

        contourMapBuffer.push_back(coord);

        file.get(c);
        if (c == '#')
        {
            break;
        }
        if (c != ',')
        {
            fileError();
        }
    }

    _bot = Bot(readCoord());

    file.get(c);
    if (c != '#')
    {
        fileError();
    }

    std::vector<std::vector<sf::Vector2<uint32_t>>> obstacleMapsBuffer(1);
    file.get(c);
    file.unget();
    if (c != '#')
    {
        while (true)
        {
            obstacleMapsBuffer.back().push_back(readCoord());

            file.get(c);
            if (c == '#')
            {
                break;
            }
            if (c == ',')
            {
                continue;
            }
            else if (c == ';')
            {
                obstacleMapsBuffer.push_back(std::vector<sf::Vector2<uint32_t>>());
                continue;
            }
            else
            {
                fileError();
            }
        }
    }

    file.get(c);
    file.unget();
    if (c != '#')
    {
        while (file.get(c))
        {
            Booster::BoosterType booster;
            if (c == 'B')
            {
                booster = Booster::MANIPULATORBUFF;
            }
            else if (c == 'C')
            {
                booster = Booster::CLONINGBUFF;
            }
            else if (c == 'F')
            {
                booster = Booster::FASTBUFF;
            }
            else if (c == 'L')
            {
                booster = Booster::DRILLBUFF;
            }
            else if (c == 'R')
            {
                booster = Booster::TELEPORTBUFF;
            }
            else if (c == 'X')
            {
                booster = Booster::BUFFX;
            }
            else
            {
                fileError();
            }
            _boosters.push_back(Booster(booster, readCoord()));

            if (!file.get(c))
            {
                break;
            }
            if (c != ';')
            {
                fileError();
            }
        }
    }
    _map = Map(xMax, yMax, contourMapBuffer, obstacleMapsBuffer);
    _map.updateMap(_bot);
    _gui.updateMap(_map);
}

void Simulation::rotate(bool clockwise)
{
    ++_time;
    _bot.rotate(clockwise);
    _map.updateMap(_bot);
    _gui.updateMap(_map);
}

void Simulation::move(Bot::Direction direction)
{
    ++_time;
    sf::Vector2<int32_t> offset{0, 0};
    switch (direction)
    {
    case Bot::UP:
        ++offset.y;
        break;
    case Bot::DOWN:
        --offset.y;
        break;
    case Bot::LEFT:
        --offset.x;
        break;
    case Bot::RIGHT:
        ++offset.x;
        break;
    }
    for (int32_t i = 0; i < _bot.getSpeed(); ++i)
    {
        if (_map.check(_bot, offset))
        {
            _bot.move(offset);
            _map.updateMap(_bot);
        }
        else
        {
            break;
        }
    }
    _gui.updateMap(_map);
}

void Simulation::useBooster(Booster::BoosterType type, std::optional<sf::Vector2<int32_t>> coords)
{
    ++_time;

}

void Simulation::run()
{
    bool stop = false;
    while(!stop)
    {
        bool stopPolling = false;
        while (!stopPolling)
        {
            switch(_gui.checkEvent())
            {
            case GUI::NEXT:
                stop = true;
                break;
            case GUI::GOUP:
                move(Bot::UP);
                break;
            case GUI::GODOWN:
                move(Bot::DOWN);
                break;
            case GUI::GOLEFT:
                move(Bot::LEFT);
                break;
            case GUI::GORIGHT:
                move(Bot::RIGHT);
                break;
            case GUI::ROTATECL:
                rotate(true);
                break;
            case GUI::ROTATECCL:
                rotate(false);
                break;
            case GUI::LASTEV:
                stopPolling = true;
                break;
            case GUI::EMPTY:
                break;
            }
        }
        _gui.updateBoosters(_boosters);
        _gui.updateBot(_bot);
        _gui.draw();
    }
}
