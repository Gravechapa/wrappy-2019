#pragma once

#ifdef UNIX
#include <filesystem>
namespace fs = std::filesystem;
#endif

#ifdef WINDOWS
#include <filesystem>
namespace fs = std::experimental::filesystem::v1;
#endif

enum Booster: short
{
    CODEB = 1,
    CODEF,
    CODEL,
    CODEX
};

enum soil: short
{
    EMPTY = 0,
    PREPARED,
    WALL,
    OBSTACLE
};

class Map
{
public:
    static Map parse(const fs::path &path);

private:
    Map(){}

    std::vector<std::vector<short>> mineMap;
};
