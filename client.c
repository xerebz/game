#include <arpa/inet.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_MSG_LEN 100
#define MAX_PLAYERS 4
#define PORT 4242

void render_state(game_state state) {
    clear();
    mvprintw(game_state[0][0], game_state[0][1], "x");
};

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./client [server_address]\n");
        exit(0);
    }

    int sockfd;
    char game_state[MAX_PLAYERS][2] = {};
    int client_input;
    struct sockaddr_in server_addr;
    socklen_t addrlen;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));
    server_addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    initscr();
    timeout(0);
    noecho();
    curs_set(0);

    while(1) {
        client_input = getch();
        if (client_input != -1) {
            sendto(sockfd, &client_input, sizeof(client_input),
                0, (const struct sockaddr *)&server_addr,
                sizeof(server_addr));
        }
        recvfrom(sockfd, game_state, sizeof(game_state),
            MSG_DONTWAIT, (struct sockaddr *)&server_addr, &addrlen);
        render_state(state);
    }

    endwin();

    close(sockfd);

    return 0;
}
