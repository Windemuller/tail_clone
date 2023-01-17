import sys
import time


def log_file(name):
    with open(name, 'w') as f:
        sys.stdout = f  # Change the standard output to the file we created.
        while True:
            time.sleep(1)
            now = time.localtime()
            print("Current time: " + time.strftime("%H:%M:%S", now))


if __name__ == '__main__':
    log_file('logfile.txt')
