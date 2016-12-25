mkdir -p analyze-out
find . | grep '\.c$' | xargs infer -- clang -c
find . | grep '\.c$' | xargs clang-check -analyze 2> analyze-out/clang_check.txt
cppcheck --enable=all . 2> analyze-out/cppcheck.txt
cp infer-out/bugs.txt analyze-out/infer.txt