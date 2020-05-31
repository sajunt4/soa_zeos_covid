#include <game.h>

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
                                         500, gettime() + 500 + nextInt(&random, 100, 1000), true};

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
    enemyGroup->aliveEnemies = numOfEnemies;
    return 1;
}


void putEnemyGroup(EnemyGroup * enemyGroup)
{
    for(int i = 0; i < enemyGroup->numEnemies; i++)
    {
        if(enemyGroup->enemies[i].alive)
            putCharacter(&enemyGroup->enemies[i].parent);
    }
}

void updateEnemyGroup(EnemyGroup * enemyGroup, Character * player, Barriers * barriers)
{
    if(gettime() >= enemyGroup->nextMovement){
        for(int i = 0; i < enemyGroup->numEnemies; i++)
        {
            if(enemyGroup->enemies[i].alive) {
                if (!enemyGroup->enemies[i].parent.projectile.active && gettime() >= enemyGroup->enemies[i].nextShoot) {
                    shootProjectile(&enemyGroup->enemies[i].parent, positive, 'Y');
                    enemyGroup->enemies[i].nextShoot = gettime() + enemyGroup->enemies[i].shootDelay;
                }

                enemyGroup->enemies[i].parent.posX += enemyGroup->dir * enemyGroup->enemies[i].parent.speedX;
                enemyGroup->nextMovement = gettime() + enemyGroup->movementDelay;

                if (enemyGroup->enemies[i].parent.projectile.active) {
                    updateProjectile(&enemyGroup->enemies[i].parent.projectile, player, barriers, NULL);
                }
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

void updateProjectile(Projectile * p, Character * character, Barriers * barriers,  EnemyGroup * enemyGroup)
{
    if(!p->active) return;

    if(p->posY < 0 || p->posY >= NUM_ROWS) {
        p->active = false;
        return;
    }

    p->posY += p->speed;

    if(barriers != NULL) {
        for (int i = 0; i < barriers->amount; i++) {
            int posX = barriers->width + i * NUM_COLUMNS / barriers->amount;
            for (int y = 0; y < barriers->height; y++) {
                int posY = barriers->posY + y;
                if (posY == p->posY) {
                    for (int x = -barriers->width / 2; x < barriers->width / 2; x++) {
                        if (p->posX == posX + x) {
                            p->active = false;
                        }
                    }
                }
            }
        }
    }

    if(character != NULL) {
        for(int x = -1; x < 2; x++)
            if(p->posX == character->posX + x) {
                for (int y = -1; y < 2; y++) {
                    if(p->posY == character->posY + y){
                        p->active = false;
                        shipsLeft--;
                    }
                }
            }
    }

    if(enemyGroup != NULL){
        for(int i = 0; i < enemyGroup->numEnemies; i++)
        {
            Enemy * enemy = &enemyGroup->enemies[i];
            if(enemy->alive)
            for(int x = -1; x < 2; x++)
                if(p->posX == enemy->parent.posX + x) {
                    for (int y = -1; y < 2; y++) {
                        if(p->posY == enemy->parent.posY + y){
                            p->active = false;
                            enemy->alive = false;
                            enemyGroup->aliveEnemies--;
                        }
                    }
                }
        }
    }
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

void updateCharacter(Character * c, EnemyGroup * enemyGroup, bool userDriven)
{
    if(userDriven) {
        userMovement(c);
        solveCharacterOutOfBounds(c);
    }
    updateProjectile(&c->projectile, NULL, NULL, enemyGroup);
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

void gameOver()
{
    int beginX = 10;
    map[NUM_ROWS/2][beginX] = 'G';
    map[NUM_ROWS/2][beginX + 1] = 'A';
    map[NUM_ROWS/2][beginX + 2] = 'M';
    map[NUM_ROWS/2][beginX + 3] = 'E';
    map[NUM_ROWS/2][beginX + 5] = 'O';
    map[NUM_ROWS/2][beginX + 6] = 'V';
    map[NUM_ROWS/2][beginX + 7] = 'E';
    map[NUM_ROWS/2][beginX + 8] = 'R';

    map[NUM_ROWS/2 + 1][beginX] = 'P';
    map[NUM_ROWS/2 + 1][beginX + 1] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 2] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 3] = 'S';
    map[NUM_ROWS/2 + 1][beginX + 4] = 'S';

    map[NUM_ROWS/2 + 1][beginX + 6] = 'A';
    map[NUM_ROWS/2 + 1][beginX + 7] = 'N';
    map[NUM_ROWS/2 + 1][beginX + 8] = 'Y';

    map[NUM_ROWS/2 + 1][beginX + 10] = 'K';
    map[NUM_ROWS/2 + 1][beginX + 11] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 12] = 'Y';

    map[NUM_ROWS/2 + 1][beginX + 14] = 'T';
    map[NUM_ROWS/2 + 1][beginX + 15] = 'O';

    map[NUM_ROWS/2 + 1][beginX + 17] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 18] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 19] = 'T';
    map[NUM_ROWS/2 + 1][beginX + 20] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 21] = 'Y';
}

void win()
{
    int beginX = 10;
    map[NUM_ROWS/2][beginX] = 'Y';
    map[NUM_ROWS/2][beginX + 1] = 'O';
    map[NUM_ROWS/2][beginX + 2] = 'U';

    map[NUM_ROWS/2][beginX + 5] = 'W';
    map[NUM_ROWS/2][beginX + 6] = 'I';
    map[NUM_ROWS/2][beginX + 7] = 'N';

    map[NUM_ROWS/2 + 1][beginX] = 'P';
    map[NUM_ROWS/2 + 1][beginX + 1] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 2] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 3] = 'S';
    map[NUM_ROWS/2 + 1][beginX + 4] = 'S';

    map[NUM_ROWS/2 + 1][beginX + 6] = 'A';
    map[NUM_ROWS/2 + 1][beginX + 7] = 'N';
    map[NUM_ROWS/2 + 1][beginX + 8] = 'Y';

    map[NUM_ROWS/2 + 1][beginX + 10] = 'K';
    map[NUM_ROWS/2 + 1][beginX + 11] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 12] = 'Y';

    map[NUM_ROWS/2 + 1][beginX + 14] = 'T';
    map[NUM_ROWS/2 + 1][beginX + 15] = 'O';

    map[NUM_ROWS/2 + 1][beginX + 17] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 18] = 'E';
    map[NUM_ROWS/2 + 1][beginX + 19] = 'T';
    map[NUM_ROWS/2 + 1][beginX + 20] = 'R';
    map[NUM_ROWS/2 + 1][beginX + 21] = 'Y';
}

void draw()
{
    put_screen(&map);
}

char map[NUM_ROWS][NUM_COLUMNS];

void gameLoop(){
    initRandom(&random, gettime());

    Character player = {NUM_COLUMNS / 2, NUM_ROWS - 2, 1, 0,
                        {{' ', 'U', ' '}, {0x2F, ' ', '\\'}, {'0', ' ', '0'}},
                        {NUM_COLUMNS / 2, NUM_ROWS - 6, 1, ' ', false}
    };

    EnemyGroup enemyGroup;
    initEnemyGroup(&enemyGroup, 7, 10, 30);

    Barriers barriers = {5, NUM_ROWS - 6, 6, 2, 'O'};

    int nextUpdate = gettime() + DELAY;

    shipsLeft = 3;
    gameStatus = PLAYING;

    while(gameStatus == PLAYING)
    {
        if(gettime() >= nextUpdate) {
            clearMap();
            updateCharacter(&player, &enemyGroup, true);
            putCharacter(&player);
            updateEnemyGroup(&enemyGroup, &player, &barriers);
            putEnemyGroup(&enemyGroup);
            putBarriers(&barriers);
            draw();
            nextUpdate = gettime() + DELAY;

            if(shipsLeft <= 0){
                gameStatus = DEAD;
                clearMap();
                gameOver();
                draw();
            }

            if(enemyGroup.aliveEnemies <= 0)
            {
                gameStatus = WIN;
                clearMap();
                win();
                draw();
            }
        }
    }

    while(gameStatus == DEAD || gameStatus == WIN)
    {
        char input;
        if(get_key(&input) >= 0) {
            gameLoop();
        }
    }
}
