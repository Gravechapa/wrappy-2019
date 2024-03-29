#include <regex>
#include "Simulation.hpp"

#include <SFML/Graphics.hpp>

int main()
{

    auto outputDir = fs::path("solutions");
    if (fs::exists(outputDir))
            {
                fs::remove_all(outputDir);
            }
    fs::create_directory(outputDir);

    for(auto& mapSetEntry : fs::directory_iterator("maps"))
    {
        auto outputSetDir = outputDir.string() + "/" + mapSetEntry.path().filename().string();
        fs::create_directory(outputSetDir);

        for(auto& mapEntry : fs::directory_iterator(mapSetEntry.path()))
        {
            std::string mapName = mapEntry.path().filename().string();
            if (std::regex_match(mapName, std::regex(R"(^prob-\d+\.desc)")))
            {
                Simulation sim = Simulation(mapEntry.path());
                sim.run();
            }
        }

    }

    return 0;
}
