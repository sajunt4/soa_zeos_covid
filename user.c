#include <game.h>
#include <menu.h>

//TODO: Colisiones

void test_sbrk(){
    write(1, "Test: SBRK\n", strlen("Test: SBRK\n"));
    int *res = sbrk(1024*4*1);
    if((int)res < 0){
        write(1, "Error found!!!!", strlen("Error found!!!!"));
        perror();
        exit(1);
    }
    for (int i = 0; i < 1024 * 1; ++i) {
        res[i] = i;
    }
    write(1, "Test PASSED: SBRK\n", strlen("Test PASSED: SBRK\n"));
}

void test_get_key(){
    write(1, "Test: GET_KEY\n", strlen("Test: GET_KEY\n"));
    char *c;
    for (int i = 0; i < 10; ++i) {
        int res = get_key(c);
        if (res < 0) perror();
        else{
            write(1, "Key Found\n", strlen("Key Found\n"));
        }
    }

    write(1, "Test PASSED: GET_KEY\n", strlen("Test PASSED: GET_KEY\n"));
}

void test_put_screen(){
    write(1, "Test: PUT_SCREEN\n", strlen("Test: PUT_SCREEN\n"));
    char buffer[25][80];
    for(int x = 0; x < 80; x++)
        for(int y = 0; y < 25; y++)
            buffer[y][x] = ' ';

    char wrongBuffer[1][1];
    int res = put_screen(wrongBuffer);
    if(res < 0) perror();

    for(int x = 0; x < 80/2; x++){
        for(int y = 0; y < 25/2; y++){
            buffer[y][x] = 'O';
            buffer[25-y][80-x] = 'O';

            int res = put_screen(buffer);
            if(res < 0) perror();
        }
    }

    write(1, "Test PASSED: PUT_SCREEN\n", strlen("Test PASSED: PUT_SCREEN\n"));
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    int result;
    while((result = updateMenu()) == -1);

    if(result == 0){
        write(1, "Test STARTING\n", strlen("Test STARTING\n"));
        test_put_screen();
        test_sbrk();
        test_get_key();
        write(1, "All Tests PASSED\n", strlen("All Tests PASSED\n"));
    }
    else if(result == 1){
        gameLoop();
    }

    while(1) {

    }
}