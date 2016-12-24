./test utf8char > test_result/utf8char.txt
./test mutable_string > test_result/mutable_string.txt
./test lines > test_result/lines.txt

cp -R test_src/texts/ test_result/
./test textfile_read_write test_result/pi.txt
./test textfile_read_write test_result/sample.txt
# TODO ./test textfile_read_write test_result/alice\'s_adventures_in_wonderland.txt
./test textfile_read_write test_result/kokoro.txt
./test textfile_read_write test_result/square_polano.txt
