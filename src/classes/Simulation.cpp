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
                booster = Booster::MANIPULATORBUFFB;
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
    _gui.updateMap(_map);
}

void Simulation::run()
{
    while(!_gui.checkCloseEvent())
    {
        _gui.updateBoosters(_boosters);
        _gui.updateBot(_bot);
        _gui.draw();
    }
}
