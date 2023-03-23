#include "test_t.h"
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
}  // namespace aggregator