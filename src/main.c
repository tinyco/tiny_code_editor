#include <stdio.h>

void file_print(char* filename) {
  FILE *fp;
  char str[10];
  if((fp = fopen(filename, "r")) == NULL) {
    printf("[error]can't open\n");
    return;
  }
  while(fgets(str, sizeof(str), fp)) {
    printf("%s", str);
  }
  fclose(fp);
}

int main(int argc, char *argv[]) {
  printf("\e[2J\e[1;1H");//clear
  if(argc != 2) {
    printf("[error]illegal args\n");
  } else {
    file_print(argv[1]);
  }
}
