#include "test_t.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <utility>

namespace aggregator {
test_t::test_t(uint32_t _id, std::string _name)
    : id(_id), name(std::move(_name)), testResult(UNKNOWN) {
}
void test_t::update_on_finish(std::chrono::milliseconds _duration, TEST_RESULT _result, std::string _error) {
    this->duration = _duration;
    this->testResult = _result;
    this->errorText = std::move(_error);
}

std::ostream &operator<<(std::ostream &os, const test_t &test) {
    os << "id=" << test.id << ", name='" << test.name << "', dur=" << test.duration.count() << ", result=" << test.testResult;
    if (!test.errorText.empty())
        os << ", error='" << test.errorText << "'";
    return os;
}
static bool case_ins_search(const std::string &text, const std::string &string) {
    return std::search(text.begin(), text.end(), string.begin(), string.end(),
                       [](const char a, const char b) {
                           return std::toupper(a) == std::toupper(b);
                       }) != text.end();
}

bool test_t::contains(std::string filter) {
    return case_ins_search(name, filter) || case_ins_search(errorText, filter);
}
}  // namespace aggregator