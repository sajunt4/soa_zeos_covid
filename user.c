#include <libc.h>
#include <errno.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  write(1, "\n\n", 2);
  char data[10];
  while(1) {
    char c = ' '; 
    int res = get_key(&c);
    if(res > 0) {
      write(1, &c, 1);
    }
    //else write(1, "NOK", 3);
    //if(errno == ENULLBUF || errno == EEMPTBUF) write(1, "ERRNO", 5);

  }
}
