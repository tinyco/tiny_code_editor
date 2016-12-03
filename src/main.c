#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

#include "lib/string.c"
#include "lib/file_read.c"

struct termios CookedMode;
struct termios RawMode;

int main(int argc, char *argv[]) {
  string* head = malloc(sizeof(string));
  head->prev = NULL;
  head->next = NULL;
  if(argc != 2) {
    printf("[error]illegal args\n");
  } else {
    file_read(argv[1], head);
    string* current = head;

    int input_key;
    tcgetattr(STDIN_FILENO, &CookedMode);
    cfmakeraw(&RawMode);

    tcsetattr(STDIN_FILENO, 0, &RawMode);
    while(current) {
      printf("\e[2J\e[H");//clear
      printf("%s", current->str);

      input_key = getchar();
      if(input_key == 110/*110 is n*/) {
        current = current->next;
      }
      if(input_key == 112/*110 is p*/) {
        current = current->prev;
      }
      if(input_key == 101/*101 is e*/) {
        printf("\e[2J\e[H");//clear
        break;
      }
    }
    tcsetattr(STDIN_FILENO, 0, &CookedMode);

  }
  return EXIT_SUCCESS;
}
