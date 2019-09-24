#include <arpa/inet.h>
#include <curses.h>
#include <iostream>
#include <unistd.h>

#include "state.h"

const char *skins[4] = {
    "al",
    "ev",
    "rb",
    "ik"
};

void render_state(GameState state) {
    clear();
    int i = 0;
    for (auto& player : state.players) {
        mvprintw(player.y, player.x, skins[i++]);
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: ./client server_address port\n";
        return -1;
    }
    const char *addr = argv[1];
    const unsigned short port = atoi(argv[2]);

    int sockfd;
    GameState state;
    int client_input;
    struct sockaddr_in sin;
    socklen_t addrlen = sizeof(sin);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, addr, &(sin.sin_addr));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    initscr();
    // polls a little faster than 64 fps (tickrate)
    timeout(15);
    noecho();
    curs_set(0);

    // send an empty message to tell the server that there's a new client
    sendto(sockfd, 0, 0,
        0, (const struct sockaddr *)&sin,
        sizeof(sin));

    while(1) {
        client_input = getch();
        if (client_input != -1) {
            sendto(sockfd, &client_input, sizeof(client_input),
                0, (const struct sockaddr *)&sin,
                sizeof(sin));
        }
        recvfrom(sockfd, &state, sizeof(state),
            0, (struct sockaddr *)&sin, &addrlen);
        render_state(state);
    }

    endwin();

    close(sockfd);

    return 0;
}
