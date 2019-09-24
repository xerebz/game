#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>

#include "state.h"

// ticks per second
#define TICK_RATE 64
#define TICK_US (1000000 / TICK_RATE)

using std::unordered_map;

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::high_resolution_clock;

void process_input(unsigned player_id, int client_input, GameState &state) {
    auto& player = state.players[player_id];
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

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: server [port]\n";
        return 0;
    }
    const unsigned short port = atoi(argv[1]);

    int sockfd;
    struct sockaddr_in client_addr[MAX_PLAYERS];
    struct sockaddr_in current_client;
    socklen_t addrlen = sizeof(current_client);
    unordered_map<unsigned, unsigned> players;
    GameState state;
    unsigned pid;
    int client_input;
    fd_set set;

    struct sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (const struct sockaddr *)&sin, sizeof(sin));

    auto next_tick = high_resolution_clock::now();
    microseconds tick_us = microseconds(TICK_US);

    struct timeval tv = {};

    while(1) {
        next_tick += tick_us;
        // while waiting for tick, collect user input and update game state
        while (high_resolution_clock::now() < next_tick) {
            FD_ZERO(&set);
            FD_SET(sockfd, &set);
            tv.tv_usec = duration_cast<microseconds>(next_tick - high_resolution_clock::now()).count();
            // wait for input
            if (select(sockfd+1, &set, NULL, NULL, &tv) > 0) {
                recvfrom(sockfd, &client_input, sizeof(client_input),
                    0, (struct sockaddr *)&current_client, &addrlen);
                pid = current_client.sin_port;
                if (players.find(pid) == players.end()) {
                    // new player
                    players[pid] = players.size();
                    client_addr[players[pid]] = current_client;
                }
                // change game state
                process_input(players[pid], client_input, state);
            }
        }
        // broadcast game state to all players once per tick
        for (unsigned i = 0; i < players.size(); i++) {
            sendto(sockfd, &state, sizeof(state), 0,
                (const struct sockaddr *)&client_addr[i], addrlen);
        }
    }

    close(sockfd);

    return 0;
}
