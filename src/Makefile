PROG := main
TEST := test
SRCS := $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)
DEFS := $(SRCS:%.c=%.gen.h)

all: $(PROG) $(TEST)

$(PROG): $(OBJS) main.c
	gcc -g -Wall -Wextra -o $@ $^

$(TEST): $(OBJS) test.c
	gcc -g -Wall -Wextra -o $@ $^

.c.o:
	gcc -g -Wall -Wextra -c -o $@ $<

clean:
	rm -rf $(PROG) $(OBJS) *.dSYM

create_gen_h:
	node generator/generate_header.js $(SRCS) main.c test.c

delete_gen_h:
	rm -rf $(DEFS)

code_format:
	sh code_format.sh

test_run:
	sh test_runner.sh
