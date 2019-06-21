#include "Simulation.hpp"

#include <fstream>
#include <queue>

Simulation::Simulation(const fs::path &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Can't open file: " + path.string());
    }

    auto fileError = [&path, &file](){
        throw std::runtime_error("Corrupt file: " + path.string()
                                 + " pos: " + to_string(file.tellg()));};

    auto readCoord = [&file, &fileError]()->std::pair<uint32_t, uint32_t>{
        std::pair<uint32_t, uint32_t> coord;
        char c;
        file.get(c);
        if (c != '(')
        {
            fileError();
        }

        file >> coord.first;

        file.get(c);
        if (c != ',')
        {
            fileError();
        }

        file >> coord.second;

        file.get(c);
        if (c != ')')
        {
            fileError();
        }
        return coord;
    };

    char c;
    std::queue<std::pair<uint32_t, uint32_t>> contourMapBuffer;
    uint32_t xMax = 0;
    uint32_t yMax = 0;
    while (true)
    {
        auto coord = readCoord();
        xMax = std::max(coord.first, xMax);
        yMax = std::max(coord.second, yMax);

        contourMapBuffer.push(coord);

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

    /*map._botInitialLocation = */readCoord();

    file.get(c);
    if (c != '#')
    {
        fileError();
    }

    std::vector<std::queue<std::pair<uint32_t, uint32_t>>> obstacleMapsBuffer(1);
    file.get(c);
    file.unget();
    if (c != '#')
    {
        while (true)
        {
            obstacleMapsBuffer.back().push(readCoord());

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
                obstacleMapsBuffer.push_back(std::queue<std::pair<uint32_t, uint32_t>>());
                continue;
            }
            else
            {
                fileError();
            }
        }
    }

    std::queue<std::pair<short, std::pair<uint32_t, uint32_t>>> boosterMapBuffer;
    file.get(c);
    file.unget();
    if (c != '#')
    {
        while (file.get(c))
        {
            short booster;
            if (c == 'B')
            {
                booster = CODEB;
            }
            else if (c == 'F')
            {
                booster = CODEF;
            }
            else if (c == 'L')
            {
                booster = CODEL;
            }
            else if (c == 'X')
            {
                booster = CODEX;
            }
            else
            {
                fileError();
            }
            boosterMapBuffer.push(std::pair(booster, readCoord()));

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
/*
    while (!boosterMapBuffer.empty())
    {
        map._mineMap[boosterMapBuffer.front().second.second][boosterMapBuffer.front().second.first]
                += boosterMapBuffer.front().first << 2;
        boosterMapBuffer.pop();
    }*/
    _map = Map(xMax, yMax, contourMapBuffer, obstacleMapsBuffer);
    _gui.updateMap(_map);
}

void Simulation::run()
{
    while(!_gui.checkCloseEvent())
    {
        _gui.draw();
    }
}
