#include "Game.h"

int main() {
    
    RecyclingGame::Game game;

    RecyclingGame::Game::setInstance(&game);

    game.run();
    
}
