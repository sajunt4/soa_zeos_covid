#include <game.h>
#include <menu.h>

//TODO: Colisiones

void test_sbrk(){
    int size = 1024*4;
    int *res = sbrk(size*3);
    int pid = fork();
    int *res2 = sbrk(size*4);
    for (int i = 0; i < size*3; ++i) {
        res[i] = i;
    }
    for (int j = 0; j < size*4; ++j) {
        res2[j] = j;
    }
    res = sbrk(-20);
    if ((int) res < 0) perror();
    write(1, "Test PASSED: SBRK\n", strlen("Test PASSED: SBRK\n"));
}

void test_get_key(){
    char *c;
    for (int i = 0; i < 10; ++i) {
        int res = get_key(c);
        if (res < 0) perror();
        else{
            write(1, "Key Found\n", strlen("Key Found\n"));
        }
    }
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    int result;
    while((result = updateMenu()) == -1);

    if(result == 0){
        //Run tests here.
    }
    else if(result == 1){
        gameLoop();
    }

    while(1) {

    }
}