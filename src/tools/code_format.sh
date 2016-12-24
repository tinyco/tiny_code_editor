find . | grep '\.c$' | sed 's/\(.*\)/ expand -t 2 \1 > \1.bak /g' | sh
find . | grep '\.c\.bak$' | sed 's/\(.*\)\.bak/ mv \1.bak \1 /g' | sh
find . | grep '\.c$' | sed 's/\(.*\)/ echo "" >> \1 /g' | sh
find . | grep '\.c$' | sed 's/\(.*\)/ clang-format -i \1 /g' | sh
