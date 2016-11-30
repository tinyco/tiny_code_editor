#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("hello,world.\n");
  for(int i=0; i<argc; i++) {
    printf("receive > %s \n",argv[i]);
  }
  printf("don't receive > %s \n",argv[100]);
}
