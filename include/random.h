typedef struct {
    int seed;
} Random;

void initRandom(Random * r, int seed);
float nextFloat(Random * r);
int nextInt(Random * r, int min, int max);