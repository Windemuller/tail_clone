# tail_clone
A CLI program that reads a file and prints its contents to stdout. It is a clone of the coreutils tail program.
This clone does not support all the options of the original tail program. 

The options available in this clone are:
* -c, --bytes [+]NUM - output the last NUM bytes
* -n, --lines [+]NUM (=10) - output the last NUM lines
* -f, --follow - output appended data as the file grows
* -s, --sleep-interval NUM (=1000) - with -f, sleep for approximately NUM milliseconds (default 1000) between iterations
* -h, --help - display this help and exit
* -v, --version - output version information and exit
* --file - the file to read from

Source code original tail: https://github.com/coreutils/coreutils/blob/master/src/tail.c