PROG := main
SRCS := $(wildcard ./*.c) $(wildcard ./**/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(PROG)

$(PROG): $(OBJS)
	gcc -o $@ $^

.o:
	gcc -c $@

clean:
	rm -rf $(PROG) $(OBJS)
