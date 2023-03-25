#include <ncurses.h>
#include <exception>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include "aggregator.h"
#include "test_t.h"
#include "ui.h"

//
#include <fcntl.h>
#include <ncurses.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#define deb(x) std::cerr << #x << " = " << x << std::endl;

namespace aggregator {
extern std::unordered_map<uint32_t, test_t> tests;
void process_line(std::string line);
}  // namespace aggregator

int main() {
    std::string line;
    while (getline(std::cin, line)) {
        if (std::all_of(line.begin(), line.end(), isspace))
            continue;
        aggregator::process_line(line);
    }

    aggregator::aggregator a(aggregator::tests);

    aggregator::ui gui(a);
}