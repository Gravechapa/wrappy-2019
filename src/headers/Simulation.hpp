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

    void rotate(bool clockwise);
    void move(Bot::Direction direction);
    void useBooster(Booster::BoosterType type, std::optional<sf::Vector2<int32_t>> coords);

    Map _map;
    GUI _gui;
    std::list<Booster> _boosters;
    Bot _bot;
    uint32_t _time{0};
};
