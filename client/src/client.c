#include <SDL2/SDL.h>
#include "network.h"

int main() {
    if (SDLNET_Init() < 0) {
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 1;
    }
    UDPsocket clientSocket = SDLNet_UDP_Open(0);
    if (!clientSocket) {
        printf("Failed to open client socket: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 1;
    }
    
    IPaddress serverAddr;
    if (SDLNet_ResolveHost(&serverAddr, "127.0.0.1", SERVER_PORT) < 0) {
        printf("Failed to resolve host: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(clientSocket);
        SDLNet_Quit();
        return 1;
    }

    UDPpacket *sendPacket = SDLNet_AllocPacket(512);
    if (!sendPacket) {
        printf("Failed to allocate send packet: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(clientSocket);
        SDLNet_Quit();
        return 1;
    }
    joinMessage join;
    join.type = MSG_JOIN;

    memcpy(sendPacket->data, &join, sizeof(joinMessage));
    sendPacket->len = sizeof(joinMessage);
    sendPacket->address = serverAddr;

    if (SDLNet_UDP_Send(clientSocket, -1, sendPacket) == 0) {
        printf("Failed to send join packet: %s\n", SDLNet_GetError());
    } else {
        printf("Join packet sent to server\n");
    }
    
    SDLNet_FreePacket(sendPacket);
    SDLNet_UDP_Close(clientSocket);
    SDL_Quit();
    return 0;
}