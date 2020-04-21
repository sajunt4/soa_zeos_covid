#include <libc.h>
#include <errno.h>

int pid;

char screen[80][25];

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  //Test put screen -------------------------------
  for(int x = 0; x < 80; x++){
    for(int y = 0; y < 25; y++){
      screen[x][y] = '0';
    }
  }

  put_screen(&screen);
  //End test put screen ----------------------------

  //Test getkey ------------------------------------
  while(1) {
    char c = ' '; 
    int res = get_key(&c);
    if(res > 0) {
      write(1, &c, 1);
    }
  }
  //End test getkey ---------------------------------
  
}
