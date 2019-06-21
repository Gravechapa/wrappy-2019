#include "Map.hpp"

#include <fstream>
#include <queue>

Map Map::parse(const fs::path &path)
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

    std::pair<uint32_t, uint32_t> botCoord = readCoord();

    file.get(c);
    if (c != '#')
    {
        fileError();
    }

    std::queue<std::pair<uint32_t, uint32_t>> obstacleMapBuffer;
    file.get(c);
    file.unget();
    if (c != '#')
    {
        while (true)
        {
            obstacleMapBuffer.push(readCoord());

            file.get(c);
            if (c == '#')
            {
                break;
            }
            if (c != ',' && c != ';')
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

    Map map;
    map.mineMap = std::vector(yMax + 1, std::vector(xMax + 1, short{0}));

    while (!contourMapBuffer.empty())
    {
        map.mineMap[contourMapBuffer.front().second][contourMapBuffer.front().first] = WALL;
        contourMapBuffer.pop();
    }

    while (!obstacleMapBuffer.empty())
    {
        map.mineMap[obstacleMapBuffer.front().second][obstacleMapBuffer.front().first] = OBSTACLE;
        obstacleMapBuffer.pop();
    }

    while (!boosterMapBuffer.empty())
    {
        map.mineMap[boosterMapBuffer.front().second.second][boosterMapBuffer.front().second.first]
                += boosterMapBuffer.front().first << 2;
        boosterMapBuffer.pop();
    }

    return map;
}
