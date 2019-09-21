#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>

#define PORT 4242
#define TICK_RATE 64
#define TICK_US (1000000 / TICK_RATE)
#define WORLD_SIZE 8

using std::cout;
using std::endl;
using std::unordered_map;

using std::chrono::microseconds;
using std::chrono::high_resolution_clock;

void process_input(unsigned player_id, int client_input, game_state *state) {
    player = state.players[player_id];
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

int main()
{
    int sockfd;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    unordered_map<unsigned, unsigned> players;
    char game_state[4][2] = {0};
    unsigned player_id;
    int client_input;

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr = {
            .s_addr = INADDR_ANY
        },
        .sin_zero = {0}
    };

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    auto next_tick = high_resolution_clock::now();
    microseconds tick_us = microseconds(TICK_US);

    cout << "Server online!" << endl;

    while(1) {
        next_tick += tick_us;
        while (high_resolution_clock::now() < next_tick) {
            recvfrom(sockfd, &client_input, sizeof(client_input),
                0, (struct sockaddr *)&client_addr, &addrlen);
            player_id = client_addr.sin_port;
            if (players.find(player_id) == players.end()) {
                players[player_id] = players.size();
                cout << players[player_id] << endl;
            }
            process_input(players[player_id], client_input, game_state);
            cout << client_addr.sin_port << endl;
        }
        // convert input to game state changes
        // send game state to clients
        sendto(sockfd, game_state, sizeof(game_state), 0,
            (const struct sockaddr *)&client_addr, addrlen);
    }

    close(sockfd);

    return 0;
}
