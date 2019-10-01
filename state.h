#define MAX_PLAYERS 4
#define WORLD_SIZE 8

struct player {
    char x;
    char y;
};

class GameState {
public:
    player players[MAX_PLAYERS];
    GameState();
    void process_input(unsigned player_id, int client_input);
};
