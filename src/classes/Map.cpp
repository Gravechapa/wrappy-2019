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
    std::queue<std::pair<uint32_t,uint32_t>> contourMapBuffer;
    uint32_t xMax = 0;
    uint32_t yMax = 0;
    while (file.get(c))
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

    std::queue<std::pair<uint32_t,uint32_t>> obstacleMapBuffer;
    file.get(c);
    if (c != '#')
    {
        do
        {
            obstacleMapBuffer.push(readCoord());

            file.get(c);
            if (c == '#')
            {
                break;
            }
            if (c != ',')
            {
                fileError();
            }

        }while (file.get(c));
    }

    std::queue<std::pair<short,std::pair<int32_t,int32_t>>> boosterMapBuffer;
    file.get(c);
    if (c != '#')
    {
        do
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
        }while (file.get(c));
    }

    Map map;
    map.mineMap = std::vector(yMax + 1, std::vector(xMax + 1, short{0}));

    while (!obstacleMapBuffer.empty())
    {

    }

    return map;
}
