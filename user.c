#include <game.h>
#include <menu.h>

//TODO: Colisiones

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