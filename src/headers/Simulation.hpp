#ifdef UNIX
#include <filesystem>
namespace fs = std::filesystem;
#endif

#ifdef WINDOWS
#include <filesystem>
namespace fs = std::experimental::filesystem::v1;
#endif

#include "GUI.hpp"

class Simulation
{
public:
    Simulation(const fs::path &path);
    void run();

private:

    Map _map;
    GUI _gui;
    std::list<Booster> _boosters;
    Bot _bot;
};
