#define MAX_PLAYERS 4
#define WORLD_SIZE 8

struct player {
    char x;
    char y;
};

class GameState {
public:
    player players[MAX_PLAYERS];
    GameState() {
        srand(time(NULL));
        for (auto &player:players) {
            player.x = rand() % WORLD_SIZE;
            player.y = rand() % WORLD_SIZE;
        }
    }
};
