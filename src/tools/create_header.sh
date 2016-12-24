node generator/generate_header.js $(SRCS) main.c test.c
find test_src | grep '\.gen\.h$' | grep -v test_cases.gen.h | sed -e 's/\(.*\)/#include "\1"/g' > test_src/test_cases.gen.h