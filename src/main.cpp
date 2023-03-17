#include <exception>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include "test.h"

#define deb(x) std::cerr << #x << " = " << x << std::endl;

namespace aggregator {

std::unordered_map<uint32_t, test_t> tests;

void start_test(std::string id, std::string name) {
    uint32_t _id = std::stoull(id);
    if (tests.contains(_id))
        throw std::runtime_error("Ambigous test names");
    tests[_id] = test_t(_id, name);
}
void finish_test(std::string id, std::string duration, std::string result, std::string errText) {
    static std::unordered_map<std::string, RESULT> table = {
        {"SUCCESS", SUCCESS},
        {"FAIL", FAIL},
        {"UNKNOWN", UNKNOWN},
    };

    uint32_t _id = std::stoull(id);
    auto _duration = std::chrono::milliseconds(std::stoull(duration));
    RESULT _result = table[result];
    if (!tests.contains(_id))
        throw std::runtime_error("Test finished, but did not start");
    tests[_id].update_on_finish(_duration, _result, errText);
}

void process_line(std::string line) {
    enum MESSAGE_GROUP {
        ID = 1,
        NAME = 2,
        DURATION = 2,
        RESULT = 3,
        ERROR = 5
    };
    std::regex start_message("testStarted id=\"(.+)\" name=\"(.*)\"");
    std::regex finish_message(
        "testFinished id=\"(.+)\" duration=\"(.+)\" result=([A-Z]+)( error=\"(.*)\")?");

    {
        std::smatch matches;
        if (std::regex_search(line, matches, start_message)) {
            start_test(matches[ID], matches[NAME]);
            return;
        }
    }

    {
        std::smatch matches;
        if (std::regex_search(line, matches, finish_message)) {
            finish_test(
                matches[ID],
                matches[DURATION],
                matches[RESULT],
                matches[ERROR]);
            return;
        }
    }
    throw std::runtime_error("Not enougth arguments :" + line);
}
};  // namespace aggregator
int main() {
    std::string line;
    while (getline(std::cin, line)) {
        if (std::all_of(line.begin(), line.end(), isspace))
            continue;
        aggregator::process_line(line);
    }

    for (auto [id, test] : aggregator::tests) {
        std::cout << id << ": " << test << '\n';
    }
}