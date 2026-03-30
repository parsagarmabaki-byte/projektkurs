#ifndef NETWORK_DATA
#define NETWORK_DATA

#define MAX_PLAYERS 6
#define SERVER_PORT 2000

// BARA RÖRELSER OCH ROLLER IMPLEMENTERADE
// MER SKA LÄGGAS TILL

typedef enum{
    MSG_JOIN,
    MSG_INPUT,
    MSG_STATE
} MessageType;

typedef struct { // Info som användaren klickar in
    MessageType type;
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
    MessageType type;
    playerState players[MAX_PLAYERS];
    gamePhase phase;
    int your_player_id;
} gameState;

#endif