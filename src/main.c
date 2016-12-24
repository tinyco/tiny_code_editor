#include "_consts_for_main.h" //PUBLIC
#include "command/command.gen.h"
#include "command/command_parse.gen.h"
#include "command/command_perform.gen.h"
#include "keyboard/keyboard.gen.h"
#include "render/context.gen.h"
#include "render/render.gen.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    while (1) {
      utf8char k = utf8char_malloc();
      keyboard_scan(&k);
      printf("%2X\n", k[0]);
      if (k[0] == 4)
        break;
    }
    printf("[error]illegal args\n");
    return (EXIT_FAILURE);
  } else {
    context context;
    context_read_file(&context, argv[1]);
    context.cursor.start_position_x = 1;
    context.cursor.start_position_y = 1;
    context.cursor.end_position_x = 1;
    context.cursor.end_position_y = 1;
    context.render_start_height = 0;
    utf8char key = utf8char_malloc();
    command cmd_none;
    cmd_none.command_key = NONE;
    command_perform(cmd_none, &context);
    while (1) {
      render_setting(&context);
      render(context);
      keyboard_scan(&key);
      command cmd = command_parse(key);
      command_perform(cmd, &context);
    }
    utf8char_free(key);
    return EXIT_SUCCESS;
  }
}
