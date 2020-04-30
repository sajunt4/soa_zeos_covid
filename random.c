#include <random.h>
#include <libc.h>

void initRandom(Random * r, int seed)
{
    r->seed = seed;
}

float nextRandom(Random * r)
{
    r->seed = (1103515245 * r->seed + 12345) % (1 << 31);
    return 1.0 * r->seed / (1 << 31);
}

int nextInt(Random * r, int min, int max)
{
    float value = nextRandom(r);

    return min + (max - min) * value;
}