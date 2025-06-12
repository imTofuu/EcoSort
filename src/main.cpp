#include "Game.h"

int main() {
    
    EcoSort::Game game;

    EcoSort::Game::setInstance(&game);

    game.run();
    
}
