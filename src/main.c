#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("\e[2J\e[1;1H");//clear
  printf("\e[31m\e[47m");//color
  printf("hello,world.\n");
  printf("\e[39m\e[49m");//reset
  for(int i=0; i<argc; i++) {
    printf("receive > %s \n",argv[i]);
  }
}
