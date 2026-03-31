#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <string.h>
#include "network.h"
#include "network_data.h"
int main(){

    if (SDLNet_Init() < 0){
        printf("SDLNet_Init failed: %s\n", SDLNet_GetError());
        return 1;
    }
    UDPsocket serverSocket = SDLNet_UDP_Open(SERVER_PORT);
    if (!serverSocket){
        printf("Failed to open server socket: %s\n", SDLNet_GetError());
        SDLNet_Quit();
        return 1;
    }
    
    UDPpacket *sendPacket = SDLNet_AllocPacket(512);
    UDPpacket *receivePacket = SDLNet_AllocPacket(512);
    if (!receivePacket){
        printf("Failed to allocate packet: %s\n", SDLNet_GetError());
        SDLNet_UDP_Close(serverSocket);
        SDLNet_Quit();
        return 1;
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    while(1){
        if (SDLNet_UDP_Recv(serverSocket, receivePacket)){
            joinMessage join;
            memcpy(&join, receivePacket->data, sizeof(joinMessage));
            
            if (join.type == MSG_JOIN){
                printf("Client connected / sent join message\n");
            }
        }
    }
    
    SDLNet_FreePacket(receivePacket);
    SDLNet_UDP_Close(serverSocket);
    SDLNet_Quit();
    return 0;
}