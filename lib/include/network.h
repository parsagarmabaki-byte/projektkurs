#ifndef NETWORK
#define NETWORK

#define MAX_PLAYERS 6

// BARA RÖRELSER OCH ROLLER IMPLEMENTERADE
// MER SKA LÄGGAS TILL

typedef struct { // Info som användaren klickar in
    int player_id;
    int up;
    int down;
    int left;
    int right;
} clientInput; 

typedef struct { // Info vart alla är
    int active;
    int player_id;
    int x;
    int y;

    int isAlive;
    int isImpostor;
    int isDoingTask;
} playerState;

typedef enum{
    gameLobby,
    gameRunning,
    gameMeeting
} gamePhase;

typedef struct {
    playerState players[MAX_PLAYERS];
    gamePhase phase;
} gameState;

#endif