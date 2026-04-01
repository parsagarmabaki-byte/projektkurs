#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>
#include <stdio.h>
#include "network.h"

typedef struct{
    UDPsocket socket;
    IPaddress serverAddr;
    UDPpacket *sendpacket;
} Client;

int initNetworking() {
    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 0;
    }
    return 1;
}
int openClientSocket(Client *client) {
    client->socket = SDLNet_UDP_Open(0);
    if (!client->socket) {
        printf("Failed to open client socket: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 0;
    }
    return 1;
}
int resolveServerAdress(Client *client, const char *host) {
    if (SDLNet_ResolveHost(&client->serverAddr, host, SERVER_PORT) < 0) {
        printf("Failed to resolve host: %s\n", SDLNet_GetError());
        return 0;
    }
    return 1;
}
int allocateSendPacket(Client *client, int size){
    client->sendpacket = SDLNet_AllocPacket(size);
    if (!client->sendpacket) {
        printf("Failed to allocate send packet: %s\n", SDLNet_GetError());
        return 0;
    }
    return 1;
}
int sendMessage(Client *client){
    joinMessage join;
    join.type = MSG_JOIN;

    memcpy(client->sendpacket->data, &join, sizeof(joinMessage));
    client->sendpacket->len = sizeof(joinMessage);
    client->sendpacket->address = client->serverAddr;

    if (SDLNet_UDP_Send(client->socket, -1, client->sendpacket) == 0) {
        printf("Failed to send join packet: %s\n", SDLNet_GetError());
        return 0;
    } else {
        printf("Join packet sent to server\n");
        return 1;
    }
}
void cleanClient(Client *client) {
    if (client->sendpacket){
        SDLNet_FreePacket(client->sendpacket);
        client->sendpacket = NULL;
    }
    if (client->socket) {
        SDLNet_UDP_Close(client->socket);
        client->socket = NULL;
    }
    SDLNet_Quit();
}
int main() {
    Client client = {0};
    
    if (!initNetworking()){
        return 1;
    }
    if (!openClientSocket(&client)){
        return 1;
    }
    if (!resolveServerAdress(&client, "127.0.0.1")){
        return 1;
    }
    if (!allocateSendPacket(&client, 512)){
        return 1;
    }
    if (!sendMessage(&client)){
        return 1;
    }
    
    cleanClient(&client);

    return 0;
}