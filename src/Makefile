PROG := main
TEST := test
SRCS := $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)
DEFS := $(SRCS:%.c=%.gen.h)
CFLAGS ?= -g -Wall -Wextra

all: $(PROG) $(TEST)

$(PROG): $(OBJS) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

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