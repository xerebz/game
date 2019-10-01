#include <stdlib.h>
#include <time.h>

#include "state.h"

GameState::GameState() {
    srand(time(NULL));
    for (auto &player:players) {
        player.x = rand() % WORLD_SIZE;
        player.y = rand() % WORLD_SIZE;
    }
}

void GameState::process_input(unsigned player_id, int client_input) {
    auto& player = this->players[player_id];
    switch(client_input) {
        case 'w':
            if (player.y > 0) player.y--;
            break;
        case 'a':
            if (player.x > 0) player.x--;
            break;
        case 's':
            if (player.y < WORLD_SIZE) player.y++;
            break;
        case 'd':
            if (player.x < WORLD_SIZE) player.x++;
            break;
        default:
            break;
    }
}
