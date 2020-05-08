#include <libc.h>
#include <io.h>
#include <random.h>

char map[NUM_ROWS][NUM_COLUMNS];

int pid;

#define DELAY 15 //TODO: Cambiar por fps (hay que ver en que unidades está zeos_ticks)

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
} Enemy;

typedef struct {
    Enemy enemies[10];
    int numEnemies;
    int minX, maxX;
    int maxY;
    direction dir;
    int movementDelay;
    int nextMovement;
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
    DEAD
};

int shipsLeft = 3;
Random random;
enum GameStatus gameStatus = PLAYING;

//TODO: Colisiones

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    initRandom(&random, gettime());

    Character player = {NUM_COLUMNS / 2, NUM_ROWS - 2, 1, 0,
                        {{' ', 'U', ' '}, {0x2F, ' ', '\\'}, {'0', ' ', '0'}},
                        {NUM_COLUMNS / 2, NUM_ROWS - 6, 1, ' ', false}
    };

    EnemyGroup enemyGroup;
    initEnemyGroup(&enemyGroup, 7, 10, 30);

    Barriers barriers = {5, NUM_ROWS - 6, 6, 2, 'O'};

    int nextUpdate = gettime() + DELAY;

    while(1) {
        if(gameStatus == PLAYING)
        {
            if(gettime() >= nextUpdate) {
                clearMap();
                updateCharacter(&player, true);
                putCharacter(&player);
                updateEnemyGroup(&enemyGroup);
                putEnemyGroup(&enemyGroup);
                putBarriers(&barriers);
                draw();
                nextUpdate = gettime() + DELAY;
            }
        }
    }
}

void putBarriers(Barriers * barriers)
{
    for(int i = 0; i < barriers->amount; i++)
    {
        int posX = barriers->width + i * NUM_COLUMNS / barriers->amount;
        for(int y = 0; y < barriers->height; y++)
        {
            for(int x = -barriers->width/2; x < barriers->width/2; x++)
            {
                map[barriers->posY + y][posX + x] = barriers->drawing;
            }
        }
    }
}

int initEnemyGroup(EnemyGroup * enemyGroup, int numOfEnemies, int movementDelay, int shootDelay)
{
    if(numOfEnemies > 10) return -1;

    int minX = 999;
    int maxX = 0;
    for(int i = 0; i < numOfEnemies; i++)
    {
        int posX = 5 + i * (NUM_COLUMNS - 10) / numOfEnemies;
        int posY = 3;
        enemyGroup->enemies[i] = (Enemy){(Character){posX, posY, 1, 1, {{' ', ' ', ' '}, {'U', 'U', 'U'}, {'U', ' ', 'U'}}, (Projectile){0, 0, 1, 'Y', false}},
                      500, gettime() + 500 + nextInt(&random, 100, 1000)};

        minX = min(posX, minX);
        maxX = max(posX, maxX);
    }

    enemyGroup->numEnemies = numOfEnemies;
    enemyGroup->minX = minX - 1;
    enemyGroup->maxX = maxX + 1;
    enemyGroup->maxY = 5;
    enemyGroup->dir = positive;
    enemyGroup->movementDelay = 60;
    enemyGroup->nextMovement = gettime() + 60;
    return 1;
}

void putEnemyGroup(EnemyGroup * enemyGroup)
{
    for(int i = 0; i < enemyGroup->numEnemies; i++)
    {
        putCharacter(&enemyGroup->enemies[i].parent);
    }
}

void updateEnemyGroup(EnemyGroup * enemyGroup)
{
    if(gettime() >= enemyGroup->nextMovement){
        for(int i = 0; i < enemyGroup->numEnemies; i++)
        {
            if(!enemyGroup->enemies[i].parent.projectile.active && gettime() >= enemyGroup->enemies[i].nextShoot)
            {
                shootProjectile(&enemyGroup->enemies[i].parent, positive, 'Y');
                enemyGroup->enemies[i].nextShoot = gettime() + enemyGroup->enemies[i].shootDelay;
            }

            enemyGroup->enemies[i].parent.posX += enemyGroup->dir * enemyGroup->enemies[i].parent.speedX;
            enemyGroup->nextMovement = gettime() + enemyGroup->movementDelay;

            if(enemyGroup->enemies[i].parent.projectile.active)
            {
                updateProjectile(&enemyGroup->enemies[i].parent.projectile);
            }
        }

        enemyGroup->maxX += enemyGroup->dir;
        enemyGroup->minX += enemyGroup->dir;

        if(enemyGroup->dir == positive && enemyGroup->maxX >= NUM_COLUMNS - 1)
        {
            enemyGroup->dir = negative;
            for(int i = 0; i < enemyGroup->numEnemies; i++)
            {
                enemyGroup->enemies[i].parent.posY += 1;
            }
            enemyGroup->maxY += 1;

            if(enemyGroup->maxY >= NUM_ROWS - 6) gameStatus = DEAD;
        }
        else if(enemyGroup->dir == negative && enemyGroup->minX <= 0)
        {
            enemyGroup->dir = positive;
            for(int i = 0; i < enemyGroup->numEnemies; i++)
            {
                enemyGroup->enemies[i].parent.posY += 1;
            }
            enemyGroup->maxY += 1;

            if(enemyGroup->maxY >= NUM_ROWS - 6) gameStatus = DEAD;
        }
    }
}

void shootProjectile(Character * c, direction dir, char drawing)
{
    c->projectile = (Projectile){c->posX, c->posY + dir, dir, drawing, true};
}

void updateProjectile(Projectile * p)
{
    if(!p->active) return;

    if(p->posY < 0 || p->posY >= NUM_ROWS) {
        p->active = false;
        return;
    }

    p->posY += p->speed;
}

void userMovement(Character * c)
{
    char input;
    if(get_key(&input) < 0) {
        return;
    }

    if(input == 0x4B){
        updateCharacterPosition(c, negative, none);
    } else if(input == 0x4D){
        updateCharacterPosition(c, positive, none);
    } else if(input == 0x48){
        if(!c->projectile.active)
            shootProjectile(c, negative, 0x5E);
    }
}

void updateCharacterPosition(Character * c, direction dirX, direction dirY)
{
    c->posX += c->speedX * dirX;
    c->posY += c->speedY * dirY;
}

void updateCharacter(Character * c, bool userDriven)
{
    if(userDriven) {
        userMovement(c);
        solveCharacterOutOfBounds(c);
    }
    updateProjectile(&c->projectile);
}

void putCharacter(Character * c)
{
    for(int x = -1; x < 2; x++)
        for(int y = -1; y < 2; y++)
            map[c->posY + y][c->posX + x] = c->shape[y + 1][x + 1];

    if(c->projectile.active) map[c->projectile.posY][c->projectile.posX] = c->projectile.drawing;
}

void solveCharacterOutOfBounds(Character * c)
{
    if(c->posX <= 0) c->posX = 1;
    if(c->posX >= NUM_COLUMNS - 2) c->posX = NUM_COLUMNS - 2;
}

void clearMap()
{
    for(int x = 0; x < NUM_COLUMNS; x++){
        for(int y = 0; y < NUM_ROWS; y++){
            map[y][x] = ' ';
        }
    }

    map[0][0] = 'S';
    map[0][1] = 'H';
    map[0][2] = 'I';
    map[0][3] = 'P';
    map[0][4] = 'S';

    if(shipsLeft > 0)
        map[0][6] = 'O';
    if(shipsLeft > 1)
        map[0][7] = 'O';
    if(shipsLeft > 2)
        map[0][8] = 'O';
}

void draw()
{
    put_screen(&map);
}
