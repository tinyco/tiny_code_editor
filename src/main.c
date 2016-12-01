#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

struct termios CookedMode;
struct termios RawMode;

typedef struct _string {
  char str[BUFFER_SIZE];
  struct _string *prev;
  struct _string *next;
} string;

string* insert(string *from) {
  string* to = malloc(sizeof(string));
  if (from->next) {
    from->next->prev = to;
    to->next = from->next;
  } else {
    to->next = NULL;
  }
  if(from){
    from->next = to;
  }
  to->prev = from;
  return to;
}

void file_read(char* filename, string* head) {
  FILE *fp;
  char buf[BUFFER_SIZE];

  if((fp = fopen(filename, "r")) == NULL) {
    printf("[error]can't open\n");
    return;
  }
  string* current = head;
  while(fgets(buf, sizeof(buf), fp)) {
    strcpy(current->str, buf);
    insert(current);
    current = current->next;
  }
  fclose(fp);
}

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
