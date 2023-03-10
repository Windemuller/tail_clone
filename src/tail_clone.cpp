#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <boost/program_options.hpp>
#include "tail_clone_config.h"

#define PROGRAM_NAME "tail_clone"
#define PROGRAM_AUTHORS "Stijn Stroeve and Stephan Windemuller"

#define DEFAULT_NEW_LINE_COUNT 10
#define DEFAULT_SLEEP_INTERVAL_MS 1000

namespace po = boost::program_options;

std::vector<char> get_basic_stream_chars(std::istream &stream) {
    std::vector<char> contents;
    char c;
    while (stream.get(c)) {
        contents.push_back(c);
    }
    return contents;
}

void read_basic_istream_bytes(std::basic_istream<char> &stream, int byte_count) {
    // Read in all characters from the stream
    auto chars = get_basic_stream_chars(stream);

    // Print the last N characters
    for (int i = chars.size() - byte_count; i < chars.size(); i++) {
        std::cout << chars[i];
    }
}

void read_basic_istream_lines(std::basic_istream<char> &stream, int line_count) {
    std::vector<std::string> lines{};

    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    int start = lines.size() - line_count;
    if (start < 0) {
        start = 0;
    }
    for (int i = start; i < lines.size(); i++) {
        std::cout << lines[i] << std::endl;
    }
}

void read_file_stream_lines(std::ifstream &file_stream, int line_count) {
    // Get the current position of the file. Because we opened it with mode ate, it is at the end of the file.
    auto file_end_pos = file_stream.tellg();

    int new_lines = 0;
    char out;
    for (int i = 1; i <= file_end_pos; i++) {
        // Move the file pointer back one character
        file_stream.seekg(-i, std::ios::end);
        file_stream.get(out);
        if (i != 1 && out == '\n') {
            new_lines++;
        }
        if (new_lines == line_count) {
            break;
        }
    }
    if (new_lines < line_count) {
        // We didn't find 10 new_lines, so we need to start at the beginning of the file
        file_stream.seekg(0, std::ios::beg);
    }

    // Output everything from the current position to the end of the file
    for (int i = file_stream.tellg(); i < file_end_pos; i++) {
        file_stream.seekg(i, std::ios::beg);
        file_stream.get(out);
        std::cout << out;
    }

    // Flush the output stream so that all output is written to the terminal
    std::cout.flush();
}

void read_file_stream_bytes(std::ifstream &file_stream, int byte_count) {
    if (byte_count > file_stream.tellg()) {
        byte_count = file_stream.tellg();
    }

    char out;
    for (int i = byte_count; i > 0; i--) {
        file_stream.seekg(-i, std::ios::end);
        file_stream.get(out);
        std::cout << out;
    }

    // Flush the output stream so that all output is written to the terminal
    std::cout.flush();
}

[[noreturn]] void tail_follow(std::ifstream &file_stream, unsigned int sleep_interval_ms) {
    int last_file_size = file_stream.tellg();
    while (true) {
        // Move the file pointer to the end of the file and get the current position
        file_stream.seekg(0, std::ios::end);
        int char_count = file_stream.tellg();

        int new_chars = char_count - last_file_size;
        if (new_chars < 0) {
            // The file has been truncated, so output the whole file
            std::cout << PROGRAM_NAME << ": file truncated" << std::endl;
            read_file_stream_bytes(file_stream, char_count);
        } else if (new_chars > 0) {
            // The file has been appended to, so output the new characters
            read_file_stream_bytes(file_stream, new_chars);
        }

        last_file_size = char_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_interval_ms));
    }
}

std::ifstream open_file(std::string& filename) {
    std::ifstream file{};
    file.open(filename, std::ios::ate);
    return file;
}

int main(int argc, char *argv[]) {
    try {
        std::stringstream ss;
        ss << "Usage: " << PROGRAM_NAME << " Usage: $ [OPTION]... [FILE]...\n"
                                           "Print the last 10 lines of each FILE to standard output.\n\n"
                                           "With no FILE, read standard input.\n\n"
                                           "Arguments";
        po::options_description desc(ss.str());
        desc.add_options()
                ("bytes,c", po::value<int>()->value_name("[+]NUM"), "output the last NUM bytes")
                ("lines,n", po::value<int>()->value_name("[+]NUM")->default_value(DEFAULT_NEW_LINE_COUNT), "output the last NUM lines")
                ("follow,f", po::bool_switch(), "output appended data as the file grows")
                ("sleep-interval,s", po::value<int>()->value_name("NUM")->default_value(DEFAULT_SLEEP_INTERVAL_MS), "with -f, sleep for approximately NUM milliseconds (default 1000) between iterations")
                ("help,h", "display this help and exit")
                ("version,v", "output version information and exit")
                ("file", "the file to read from");

        // Add position argument "file"
        po::positional_options_description p;
        p.add("file", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        if (vm.count("version")) {
            std::cout << PROGRAM_NAME << " version " << tail_clone_VERSION_MAJOR << "." << tail_clone_VERSION_MINOR << "." << tail_clone_VERSION_PATCH << std::endl
                      << "Written by " << PROGRAM_AUTHORS << "." << std::endl;
            return 0;
        }
        bool follow = vm["follow"].as<bool>();
        if (follow && !vm.count("file")) {
            throw std::invalid_argument("-f, --follow option requires a file");
        }

        if (vm.count("file")) {
            // If there is a file defined, open it
            auto file_name = vm["file"].as<std::string>();
            auto file = open_file(file_name);
            if (!file.is_open()) {
                throw std::invalid_argument("Could not open file '" + file_name + "'");
            }

            if (vm.count("bytes")) {
                read_file_stream_bytes(file, abs(vm["bytes"].as<int>()));
            } else {
                read_file_stream_lines(file, abs(vm["lines"].as<int>()));
            }

            if (follow) {
                tail_follow(file, abs(vm["sleep-interval"].as<int>()));
            } else {
                file.close();
            }
        } else {
            // There is no file, read from stdin
            if (vm.count("bytes")) {
                read_basic_istream_bytes(std::cin, abs(vm["bytes"].as<int>()));
            } else {
                read_basic_istream_lines(std::cin, abs(vm["lines"].as<int>()));
            }
        }
    } catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        std::cerr << "Error!\n";
        return 1;
    }
    return 0;
}