echo "PIPES: Last 16 bytes of the file:"
echo "----------"
cat test.txt | ./cmake-build-debug/tail_clone --bytes 16
echo "----------"
cat test.txt | tail -c 16
echo "----------\n\n"

echo "PIPES: Last 5 lines of the file:"
echo "----------"
cat test.txt | ./cmake-build-debug/tail_clone --lines 5
echo "----------"
cat test.txt | tail -n 5
echo "----------\n\n"

echo "FILE: Last 16 bytes of the file:"
echo "----------"
./cmake-build-debug/tail_clone --bytes 16 test.txt
echo "----------"
tail -c 16 test.txt
echo "----------\n\n"

echo "FILE: Last 5 lines of the file:"
echo "----------"
./cmake-build-debug/tail_clone --lines 5 test.txt
echo "----------"
tail -n 5 test.txt
echo "----------"