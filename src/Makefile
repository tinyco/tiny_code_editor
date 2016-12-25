PROG := main
TEST := test
SRCS := $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)
DEFS := $(SRCS:%.c=%.gen.h)
CFLAGS ?= -g -Wall -Wextra -fno-omit-frame-pointer -fsanitize=address

all: $(PROG) $(TEST)

$(PROG): $(OBJS) main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(PROG) $(TEST) $(OBJS) *.dSYM

clean_force:
	find . | grep -v '^\./node_modules/' | grep '\.o$$' | xargs rm
	find . | grep -v '^\./node_modules/' | grep \.gen\.h$$ | xargs rm
	rm -rf $(PROG) $(TEST) *.dSYM

create_header:
	node generator/generate_header.js $(SRCS) main.c test.c
	find test_src | grep '\.gen\.h' | grep -v test_cases.gen.h | sed -e 's/test_src\/\(.*\)/#include "\1"/g' > test_src/test_cases.gen.h

code_format:
	sh tools/code_format.sh

test_run:
	sh tools/test_runner.sh
