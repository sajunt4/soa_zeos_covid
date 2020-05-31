//
// Created by abel on 31/5/20.
//

#ifndef SOA_ZEOS_COVID_GAME_H
#define SOA_ZEOS_COVID_GAME_H

#include <libc.h>
#include <io.h>
#include <random.h>

extern char map[NUM_ROWS][NUM_COLUMNS];

int pid;

#define DELAY 15

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef enum {negative = -1, none = 0, positive = 1} direction;
typedef enum {false, true} bool;

typedef struct {
    int posX;
    int posY;
    int speed;
    char drawing;
    bool active;
} Projectile;

typedef struct {
    int posX;
    int posY;
    int speedX;
    int speedY;
    char shape[3][3];
    Projectile projectile;
} Character;

typedef struct {
    Character parent;
    int shootDelay;
    int nextShoot;
    bool alive;
} Enemy;

typedef struct {
    Enemy enemies[10];
    int numEnemies;
    int minX, maxX;
    int maxY;
    direction dir;
    int movementDelay;
    int nextMovement;
    int aliveEnemies;
} EnemyGroup;

typedef struct {
    int amount;
    int posY;
    int width;
    int height;
    char drawing;
} Barriers;

enum GameStatus{
    MENU,
    PLAYING,
    DEAD,
    WIN
};

int shipsLeft;
Random random;
enum GameStatus gameStatus;

int nextUpdate;


void gameLoop();

//TODO: Colisiones

#endif //SOA_ZEOS_COVID_GAME_H
