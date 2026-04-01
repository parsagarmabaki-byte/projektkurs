#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "network_data.h"

#define PACKET_SIZE 512

int init_server_network(UDPsocket *server_socket) {
    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 0;
    }

    *server_socket = SDLNet_UDP_Open(SERVER_PORT);
    if (!*server_socket) {
        printf("Failed to open server socket: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 0;
    }

    return 1;
}
    UDPpacket *create_packet(int size) {
    UDPpacket *packet = SDLNet_AllocPacket(size);
    if (!packet) {
        printf("Failed to allocate packet: %s\n", SDLNet_GetError());
    }
    return packet;
}

int receive_join_message(UDPsocket server_socket, UDPpacket *receive_packet, joinMessage *join) {
    if (!SDLNet_UDP_Recv(server_socket, receive_packet)) {
        return 0;
    }

    if (receive_packet->len < (int)sizeof(joinMessage)) {
        printf("Received packet too small for joinMessage\n");
        return 0;
    }

    memcpy(join, receive_packet->data, sizeof(joinMessage));
    return 1;
}

void handle_join_message(const joinMessage *join) {
    if (join->type == MSG_JOIN) {
        printf("Client connected / sent join message\n");
    }
}

void cleanup_server(UDPsocket server_socket, UDPpacket *receive_packet) {
    if (receive_packet) {
        SDLNet_FreePacket(receive_packet);
    }

    if (server_socket) {
        SDLNet_UDP_Close(server_socket);
    }

    SDLNet_Quit();
}

int main(void) {
    UDPsocket server_socket = NULL;
    UDPpacket *receive_packet = NULL;

    if (!init_server_network(&server_socket)) {
        return 1;
    }

    receive_packet = create_packet(PACKET_SIZE);
    if (!receive_packet) {
        cleanup_server(server_socket, NULL);
        return 1;
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while (1) {
        joinMessage join;

        if (receive_join_message(server_socket, receive_packet, &join)) {
            handle_join_message(&join);
        }
    }

    cleanup_server(server_socket, receive_packet);
    return 0;
}