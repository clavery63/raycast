#include <memory>
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "ResourcePath.hpp"

int main(int, char const**)
{
    std::unique_ptr<Game> game(new Game());
    game->run();
    return EXIT_SUCCESS;
}
