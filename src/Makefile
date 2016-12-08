PROG := main
SRCS := $(wildcard ./*.c) $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)
DEFS := $(SRCS:%.c=%.gen.h)

all: $(PROG)

$(PROG): $(OBJS)
	gcc -g -Wall -Wextra -o $@ $^

.o:
	gcc -g -c $@

clean:
	rm -rf $(PROG) $(OBJS)

create_gen_h:
	node generator/generate_header.js $(SRCS)

delete_gen_h:
	rm -rf $(DEFS)